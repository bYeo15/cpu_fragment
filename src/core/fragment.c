#include "core/fragment.h"


// -----===[ Globals ]===-----

framebuf *render_frame = NULL;

unsigned int n_threads = 4;

unsigned int n_jobs = 8;

unsigned int n_frames = 1;


// -----===[ Global Uniforms ]===-----

framebuf *BACKBUF = NULL;

unsigned long FRAME_COUNT = 0;

unsigned long long CLOCK_NS = 0;
struct timespec prev_t;

float CONST_RAND = 0.0;


// -----===[ Exposed Functions ]===-----

void create_render_frame(unsigned int dimx, unsigned int dimy)
{
    if (render_frame == NULL)
    {
        render_frame = framebuf_init(dimx, dimy);

        if (render_frame == NULL)
        {
            fprintf(stderr, "[ ERROR ] : Not enough memory to create render framebuffer of size %d x %d\n",
                    dimx, dimy);

            exit(1);
        }

        BACKBUF = framebuf_init(dimx, dimy);

        if (BACKBUF == NULL)
        {
            framebuf_delete(render_frame);

            fprintf(stderr, "[ ERROR ] : Not enough memory to create render framebuffer of size %d x %d\n",
                    dimx, dimy);

            exit(1);
        }
    }
    else
    {
        fputs("[ ERROR ] : Render framebuffer already exists\n", stderr);

        exit(1);
    }
}


// -----===[ Internal Functions ]===-----

void *fragment_thread_main(void *args)
{
    job_queue *jq;
    render_job *job;
    int quit = 0;
    tup3 active_uv = vec3_zero;

    jq = (job_queue *)args;

    // Repeatedly try to acquire a render job
    while (!quit)
    {
        job = jobq_dequeue(jq);

        if (job->quit)
        {
            quit = 1;
        }
        else
        {
            for (unsigned int y = job->y_start; y < job->y_end; y++)
            {
                for (unsigned int x = job->x_start; x < job->x_end; x++)
                {
                    active_uv.x = x;
                    active_uv.y = y;

                    render_frame->buf[x + y * render_frame->dimy] = fragment(&active_uv);
                }
            }
        }

        job_delete(job);

        // Report the completion of the job
        jobq_report_complete(jq);
    }

    return NULL;
}


int fragment_main(job_queue *jq)
{
    // Update CLOCK_NS uniform
    unsigned long long diff_s = prev_t.tv_sec;
    unsigned long long diff_ns = prev_t.tv_nsec;

    clock_gettime(CLOCK_MONOTONIC, &prev_t);

    diff_s = prev_t.tv_sec - diff_s;
    diff_ns = prev_t.tv_nsec - diff_ns;

    CLOCK_NS += diff_ns + (diff_s * 1e9);

    // Enqueue all render jobs
    // Compute the size of each job (as a horizontal slice of the frame)
    unsigned int job_xsize = render_frame->dimx;

    // Take the ceiling of the division
    unsigned int job_ysize = render_frame->dimy / n_jobs + (render_frame->dimy % n_jobs != 0);

    unsigned int remaining_y = render_frame->dimy;
    unsigned int job_n = 0;

    while (remaining_y)
    {
        render_job *job;
        unsigned int actual_ysize = job_ysize;

        if (actual_ysize > remaining_y)
        {
            actual_ysize = remaining_y;
        }

        job = job_init(0, job_xsize, job_ysize * job_n, job_ysize * job_n + actual_ysize);

        remaining_y -= actual_ysize;
        job_n++;

        // Panic if there is insufficient memory for a new job
        if (job == NULL)
        {
            exit(1);
        }

        jobq_enqueue(jq, job);
    }

    // Wait for render jobs to be complete
    jobq_wait_complete(jq);

    // Save current frame TODO

    // Copy current frame into BACKBUF
    framebuf_copy(BACKBUF, render_frame);

    // Update FRAME_COUNT uniform
    FRAME_COUNT++;

    // End if all frames are complete
    if (FRAME_COUNT >= n_frames)
    {
        return 0;
    }

    return 1;
}



int main(int argc, char **argv)
{
    job_queue *jq;
    pthread_t *thread_pool;
    unsigned int active_threads = 0;

    // Compute CONST_RAND uniform
    srand(time(NULL));
    CONST_RAND = (float) rand() / (float) RAND_MAX;

    // Initialise CLOCK_NS
    clock_gettime(CLOCK_MONOTONIC, &prev_t);
    CLOCK_NS = 0;

    // Initialise based on user handler and arguments
    frag_init(argc, argv);

    // Validate frame, thread and job information
    if (render_frame == NULL || n_threads < 1 || n_jobs < 1 || n_frames < 1)
    {
        // Clean goto - jumps forward, to greater or equal scope
        goto user_cleanup;
    }

    // Create queue of render jobs
    if ((jq = jobq_init()) == NULL)
    {
        goto user_cleanup;
    }

    // Dispatch all threads
    thread_pool = malloc(sizeof(pthread_t) * n_threads);

    if (thread_pool == NULL)
    {
        goto jobqueue_cleanup;
    }

    for (unsigned int i = 0; i < n_threads; i++)
    {
        if (pthread_create(thread_pool + i, NULL, fragment_thread_main, jq))
        {
            goto thread_cleanup;
        }

        active_threads++;
    }

    // Enter main loop
    while (fragment_main(jq));

    // Enqueue "quit" jobs once the rendering is done
thread_cleanup:
    for (unsigned int j = 0; j < active_threads; j++)
    {
        render_job *quit_job = job_quit_init();

        if (quit_job == NULL)
        {
            // Panic if there is insufficient memory to send a quit job
            exit(1);
        }

        jobq_enqueue(jq, quit_job);
    }

    // Join all threads
    for (unsigned int k = 0; k < active_threads; k++)
    {
        pthread_join(thread_pool[k], NULL);
    }

    free(thread_pool);

    // Delete the job queue
jobqueue_cleanup:
    jobq_delete(jq);


    // Clean up user resources
user_cleanup:
    frag_cleanup();

    // Delete the framebuffers (if they exist)
    if (render_frame != NULL)
    {
        framebuf_delete(render_frame);
    }
    if (BACKBUF != NULL)
    {
        framebuf_delete(BACKBUF);
    }
}
