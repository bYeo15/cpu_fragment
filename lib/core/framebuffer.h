/*
 * A framebuffer is a n x m grid of pixels (colour tuples) that represents a given screen
 *
 * A framebuffer cannot be resized after creation
 *
 * Framebuffers are organised with the top left coordinate being (0, 0), such that;
 *
 * (0, 0)
 *      #---...-#           ^
 *      |       |          - y
 *      |       |
 *      |       |
 *      .       .
 *      .       .
 *      .       .          + y
 *      |       |           V
 *      #---...-#
 *              (n, m)
 *
 *    < -x     +x >
 */

#ifndef FRAMEBUF_H
#define FRAMEBUF_H

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "tuple.h"

// -----===[ Structures ]===-----

/*
 * The actual framebuf
 *
 * dimx [unsigned int] - the width of the framebuf
 * dimy [unsigned int] - the height of the framebuf
 * buf [tup3 *] - the actual framebuf buffer
 */
typedef struct framebuf {
    unsigned int dimx;
    unsigned int dimy;
    tup3 *buf;
} framebuf;


// -----===[ Functions ]===-----

// < Framebuffer Memory >

/*
 * Creates a new framebuf of the given dimensions, with all pixels initialised to opaque black
 * (0.0, 0.0, 0.0, 1.0)
 *
 * IN:
 *      [unsigned int] - the x dimension for the framebuf
 *      [unsigned int] - the y dimension for the framebuf
 *
 * OUT: [framebuf * | NULL] - the newly created framebuf
 *                            NULL on memory error
 */
framebuf *framebuf_init(unsigned int, unsigned int);


/*
 * Deletes a framebuf and frees the associated memory
 *
 * IN:
 *      [framebuf *] - the framebuf to delete
 *
 * OUT: N/A
 */
void framebuf_delete(framebuf *);


// < Framebuffer Operations >

/*
 * Writes a pixel into a framebuf
 *
 * IN:
 *      [framebuf *] - the framebuf to write to
 *      [unsigned int] - the x coordinate (0-indexed) to write to
 *      [unsigned int] - the y coordinate (0-indexed) to write to
 *      [tup3 *] - the colour tuple to write in
 *
 * OUT: [int] - result code
 *              0: success
 *              -1: indices out of bounds
 */
int framebuf_write(framebuf *, unsigned int, unsigned int, tup3 *);


/*
 * Reads a pixel from a framebuf
 *
 * IN:
 *      [framebuf *] - the framebuf to read from
 *      [unsigned int] - the x coordinate (0-indexed) to read from
 *      [unsigned int] - the y coordinate (0-indexed) to read from
 *      [tup3 * | NULL] - the tup3 to read into (if NULL, just performs an index check)
 *
 * OUT: [int] - result code
 *              0: success
 *              -1: indices out of bounds
 */
int framebuf_read(framebuf *, unsigned int, unsigned int, tup3 *);


/*
 * Copies from one framebuffer into another - they must be of the same size
 *
 * IN:
 *      [framebuf *] - the framebuffer to copy into
 *      [framebuf *] - the framebuffer to copy from
 *
 * OUT: [int] - 0 on success, positive on invalid sizing, negative on source == dest
 */
int framebuf_copy(framebuf *, framebuf *);


#endif
