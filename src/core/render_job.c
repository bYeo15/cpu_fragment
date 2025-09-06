#include "core/render_job.h"

// -----===[ Job Queue Functions ]===-----

job_queue *jobq_init(void)
{
    job_queue *new_jq;

    new_jq = malloc(sizeof(job_queue));

    if (new_jq == NULL)
    {
        goto error_exit;
    }

    new_jq->head = NULL;
    new_jq->tail = NULL;
    new_jq->jobs_outstanding = 0;

    if (pthread_cond_init(&(new_jq->is_nonempty), NULL))
    {
        goto error_free;
    }

    if (pthread_cond_init(&(new_jq->is_complete), NULL))
    {
        goto error_destroy_cond;
    }

    pthread_mutex_init(&(new_jq->access_lock), NULL);
    pthread_mutex_init(&(new_jq->jobc_lock), NULL);

    return new_jq;

error_destroy_cond:
    pthread_cond_destroy(&(new_jq->is_nonempty));
error_free:
    free(new_jq);
error_exit:
    return NULL;
}


void jobq_delete(job_queue *jq)
{
    pthread_mutex_destroy(&(jq->access_lock));
    pthread_mutex_destroy(&(jq->jobc_lock));

    pthread_cond_destroy(&(jq->is_nonempty));
    pthread_cond_destroy(&(jq->is_complete));

    // Delete any remaining jobs
    render_job *next_job = NULL;
    for (render_job *job = jq->head; job != NULL; job = next_job)
    {
        next_job = job->next;

        job_delete(job);
    }

    free(jq);
}


void jobq_enqueue(job_queue *jq, render_job *job)
{
    pthread_mutex_lock(&(jq->access_lock));

    pthread_mutex_lock(&(jq->jobc_lock));
    jq->jobs_outstanding++;
    pthread_mutex_unlock(&(jq->jobc_lock));

    if (jq->head == NULL)
    {
        jq->head = job;
        jq->tail = job;
    }
    else
    {
        jq->tail->next = job;
        jq->tail = job;
    }

    pthread_cond_signal(&(jq->is_nonempty));
    pthread_mutex_unlock(&(jq->access_lock));
}


render_job *jobq_dequeue(job_queue *jq)
{
    render_job *job;

    pthread_mutex_lock(&(jq->access_lock));

    while (jq->head == NULL)
    {
        pthread_cond_wait(&(jq->is_nonempty), &(jq->access_lock));
    }

    job = jq->head;

    jq->head = jq->head->next;

    pthread_mutex_unlock(&(jq->access_lock));

    return job;
}


void jobq_report_complete(job_queue *jq)
{
    pthread_mutex_lock(&(jq->jobc_lock));

    if (jq->jobs_outstanding > 0)
    {
        jq->jobs_outstanding--;
    }

    if (jq->jobs_outstanding == 0)
    {
        pthread_cond_broadcast(&(jq->is_complete));
    }

    pthread_mutex_unlock(&(jq->jobc_lock));
}


void jobq_wait_complete(job_queue *jq)
{
    pthread_mutex_lock(&(jq->jobc_lock));

    while (jq->jobs_outstanding != 0)
    {
        pthread_cond_wait(&(jq->is_complete), &(jq->jobc_lock));
    }

    pthread_mutex_unlock(&(jq->jobc_lock));
}


// -----===[ Job Functions ]===-----

render_job *job_init(unsigned int x_start, unsigned int x_end, unsigned int y_start, unsigned int y_end)
{
    render_job *new_job;

    new_job = malloc(sizeof(render_job));

    if (new_job == NULL)
    {
        return NULL;
    }

    new_job->x_start = x_start;
    new_job->x_end = x_end;
    new_job->y_start = y_start;
    new_job->y_end = y_end;
    new_job->quit = 0;

    new_job->next = NULL;

    return new_job;
}


render_job *job_quit_init(void)
{
    render_job *new_job;

    new_job = malloc(sizeof(render_job));

    if (new_job == NULL)
    {
        return NULL;
    }

    new_job->x_start = 0;
    new_job->x_end = 0;
    new_job->y_start = 0;
    new_job->y_end = 0;
    new_job->quit = 1;

    new_job->next = NULL;

    return new_job;
}


void job_delete(render_job *job)
{
    free(job);
}
