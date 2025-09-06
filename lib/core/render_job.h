/*
 * A simple queue of render jobs
 *
 * Each job defines either a region of the framebuffer to compute,
 * or is a quit signal
 */

#ifndef RENDER_JOB_H
#define RENDER_JOB_H

#include <pthread.h>
#include <stdlib.h>

// -----===[ Structures ]===-----

/*
 * A render job
 *
 * x_start [unsigned int] - the starting x coordinate of the render job
 * x_end [unsigned int] - the ending x coordinate (exclusive) of the render job
 * y_start [unsigned int] - the starting y coordinate of the render job
 * y_end [unsigned int] - the ending y coordinate (exclusive) of the render job
 * next [render_job *] - a link to the next job in the queue (for internal use only)
 * quit [int] - a flag for if this job is signalling that the job handler should quit
 */
typedef struct render_job {
    unsigned int x_start;
    unsigned int x_end;
    unsigned int y_start;
    unsigned int y_end;
    struct render_job *next;
    unsigned int quit: 1;
} render_job;


/*
 * A queue of jobs
 *
 * head [render_job *] - the first job in the queue
 * tail [render_job *] - the last job in the queue
 * access_lock [pthread_mutex_t] - a lock upon enqueuing/dequeuing
 * is_nonempty [pthread_cond_t] - a condition variable signaling when there are jobs in the queue
 * jobs_outstanding [unsigned int] - the number of jobs known to be incomplete
 *                                   to be used when jobs are dispatched in batches
 * jobc_lock [pthread_mutex_t] - a lock upon updating the number of outstanding jobs
 * is_complete [pthread_cond_t] - a condition variable siqnaling when there are no outstanding jobs
 */
typedef struct job_queue {
    struct render_job *head;
    struct render_job *tail;
    pthread_mutex_t access_lock;
    pthread_cond_t is_nonempty;
    unsigned int jobs_outstanding;
    pthread_mutex_t jobc_lock;
    pthread_cond_t is_complete;
} job_queue;


// -----===[ Job Queue Functions ]===-----

/*
 * Creates a new, empty job queue
 *
 * IN: N/A
 *
 * OUT: [job_queue * | NULL] - the newly created job queue
 *                             NULL on error
 */
job_queue *jobq_init(void);


/*
 * Deletes a job_queue and any existing jobs
 *
 * Does not respect synchronisation, should only be used at cleanup
 *
 * IN:
 *      [job_queue *] - the job queue to delete
 *
 * OUT: N/A
 */
void jobq_delete(job_queue *);


/*
 * Enqueues a job
 *
 * Has no waiting condition, will succeed as soon as the mutex is acquired
 *
 * Signals the "non-empty" condition (may wake up waiting threads)
 *
 * Also increments the number of outstanding jobs
 *
 * IN:
 *      [job_queue *] - the job queue to enqueue to
 *      [render_job *] - the job to enqueue
 *
 * OUT: N/A
 */
void jobq_enqueue(job_queue *, render_job *);


/*
 * Dequeues a job
 *
 * Waits until a job is available
 *
 * Does not decrement outstanding jobs - this should be done once
 * the job is actually completed
 *
 * IN:
 *      [job_queue *] - the job queue to dequeue from
 *
 * OUT: [render_job *] - the dequeued job (must be freed)
 */
render_job *jobq_dequeue(job_queue *);


/*
 * Decrements the "outstanding jobs" counter to signal that a job has been completed
 *
 * Will signal all waiting threads if this results in zero outstanding jobs
 *
 * IN:
 *      [job_queue *] - the job queue from which a job was completed
 *
 * OUT: N/A
 */
void jobq_report_complete(job_queue *);


/*
 * Waits until all outstanding jobs have been completed
 *
 * IN:
 *      [job_queue *] - the job queue to wait on
 *
 * OUT: N/A
 */
void jobq_wait_complete(job_queue *);


// -----===[ Job Functions ]===-----

/*
 * Creates a new render job (for an actual coordinate area)
 *
 * IN:
 *      [unsigned int] - the starting x coordinate
 *      [unsigned int] - the ending x coordinate (exclusive)
 *      [unsigned int] - the starting y coordinate
 *      [unsigned int] - the ending y coordinate (exclusive)
 *
 * OUT: [render_job * | NULL] - the newly created render job
 *                              NULL on error
 */
render_job *job_init(unsigned int, unsigned int, unsigned int, unsigned int);


/*
 * Creates a new render job for quitting
 *
 * IN: N/A
 *
 * OUT: [render_job * | NULL] - the newly created render job
 *                              NULL on error
 */
render_job *job_quit_init(void);


/*
 * Deletes a render job
 *
 * IN:
 *      [render_job *] - the render job to delete
 *
 * OUT: N/A
 */
void job_delete(render_job *);

#endif
