/*
 * The framework for a fragment shader
 *
 * A fragment shader shall implement;
 *  - `fragment`, a function that renders a single pixel based on its coordinates
 *  - `frag_init`, a function that loads any user-provided resources, and handles
 *                 arguments provided to the program
 *                 must also invoke `create_render_frame`
 *                 expected to set `n_threads`, `n_jobs`, etc. if non-default values are
 *                 desired
 *  - `frag_cleanup`, a function that shall clean up any resources created by frag_init
 */

#ifndef FRAGMENT_H
#define FRAGMENT_H

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include "framebuffer.h"
#include "frame_io.h"
#include "render_job.h"

// -----===[ Globals ]===-----

// The number of threads to dispatch - defaults to four
extern unsigned int n_threads;

/*
 * The number of jobs to use per-frame - defaults to eight
 *
 * Ideally, will always be >= n_threads, and a multiple of n_threads
 *
 * Frames are split into horizontal bands for jobs, meaning that (at most)
 * one job can be dispatched for each row (pixel in the y-axis) of the render frame
 * This value provides an upper bound - very short render frames may not receive a full
 * number of jobs
 */
extern unsigned int n_jobs;


// The number of frames to render - defaults to one
extern unsigned int n_frames;


// -----===[ Global Uniforms ]===-----

/*
 * The previously rendered frame
 *
 * Initialised to all opaque black
 */
extern framebuf *BACKBUF;

/*
 * The current frame number, starting at zero, and incrementing after each frame
 */
extern unsigned long FRAME_COUNT;

/*
 * The clock's time (ns) at the start of a given frame's rendering
 * (starts at zero)
 */
extern unsigned long long CLOCK_NS;


/*
 * A constant random value between 0.0 and 1.0, loaded at initialisation
 * (safe to use in `frag_init`)
 */
extern float CONST_RAND;


// -----===[ External Functions ]===-----

/*
 * Provides the functionality for the fragment shader, as run on a single pixel.
 *
 * IN:
 *      [tup3 *] - the uv coordinates of the current pixel
 *                z and w components are undefined and should not be used
 *
 * OUT: [tup3] - the resulting pixel colour
 *               w component is alpha channel, but may not be used by all output formats
 */
extern tup3 fragment(tup3 *);


/*
 * Provides the functionality for loading any user-provided resources, and handling
 * arguments supplied to the program (that were not already consumed)
 *
 * Must invoke `create_render_frame` at some point in its execution
 *
 * IN:
 *      [int] - the number of arguments
 *      [char **] - the arguments
 *
 * OUT: N/A
 */
extern void frag_init(int, char **);


/*
 * Provides the functionality for cleaning up any user-provided resources
 *
 * IN: N/A
 *
 * OUT: N/A
 */
extern void frag_cleanup(void);


// -----===[ Functions ]===-----

/*
 * Initialises the frame for rendering, with the given dimensions
 *
 * IN:
 *      [int] - the x dimension
 *      [int] - the y dimension
 *
 * OUT: N/A
 */
void create_render_frame(unsigned int, unsigned int);


/*
 * Generates a random value in [0, LONG_MAX], based on a seed
 *
 * Afterwards, alters the seed, so that it can be used again to
 * get a new value
 *
 * Can be used inside `fragment` to ensure that all pixels get the same random
 * values per-frame (ie. seed with FRAME_COUNT)
 *
 * To get random values per-pixel, seed with a combination of FRAME_COUNT and the (x, y) coordinates
 *
 * IN:
 *      [long *] - the seed, will be modified to store the result
 *
 * OUT: [long] - the random value
 */
long frag_random(long *);

#endif
