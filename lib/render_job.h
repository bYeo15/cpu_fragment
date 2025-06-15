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
 */
typedef struct job_queue {
	struct render_job *head;
	struct render_job *tail;
	pthread_mutex_t access_lock;
	pthread_cond_t is_empty;
	pthread_cond_t is_nonempty;
} job_queue;


// -----===[ Job Queue Functions ]===-----

/*
 * Creates a new, empty job queue
 *
 * IN: N/A
 *
 * OUT: [job_queue * | NULL] - the newly created job queue
 *							   NULL on error
 */
job_queue *jobq_init(void);


/*
 * Deletes a job_queue and any existing jobs
 *
 * Does not respect synchronisation, should only be used at cleanup
 *
 * IN:
 *		[job_queue *] - the job queue to delete
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
 * IN:
 *		[job_queue *] - the job queue to enqueue to
 *		[render_job *] - the job to enqueue
 *
 * OUT: N/A
 */
void jobq_enqueue(job_queue *, render_job *);


/*
 * Dequeues a job
 * 
 * Waits until a job is available
 *
 * May signal the "is-empty" condition
 *
 * IN:
 *		[job_queue *] - the job queue to dequeue from
 *
 * OUT: [render_job *] - the dequeued job (must be freed)
 */
render_job *jobq_dequeue(job_queue *);


/*
 * Waits until a queue has been emptied
 *
 * IN:
 *		[job_queue *] - the job queue to wait on
 *
 * OUT: N/A
 */
void jobq_wait_empty(job_queue *);


// -----===[ Job Functions ]===-----

/*
 * Creates a new render job (for an actual coordinate area)
 *
 * IN:
 *		[unsigned int] - the starting x coordinate
 *		[unsigned int] - the ending x coordinate (inclusive)
 *		[unsigned int] - the starting y coordinate
 *		[unsigned int] - the ending y coordinate (inclusive)
 *
 * OUT: [render_job * | NULL] - the newly created render job
 *								NULL on error
 */
render_job *job_init(unsigned int, unsigned int, unsigned int, unsigned int);


/*
 * Creates a new render job for quitting
 *
 * IN: N/A
 *
 * OUT: [render_job * | NULL] - the newly created render job
 *								NULL on error
 */
render_job *job_quit_init(void);


/*
 * Deletes a render job
 *
 * IN:
 *		[render_job *] - the render job to delete
 *
 * OUT: N/A
 */
void job_delete(render_job *);

#endif
