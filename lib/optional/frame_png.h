/*
 * Provides frame_io (see frame_io.h) functionality for png files
 */

#ifndef FRAME_IO_PNG_H
#define FRAME_IO_PNG_H

#include <stdint.h>
#include <stdlib.h>
#include <png.h>
#include "core/frame_io.h"
#include "core/framebuffer.h"

// -----===[ Functions ]===-----

/*
 * A frame_dump function that saves a given framebuffer as a png file
 *
 * Specifically, saves the framebuffer as an 8 bit truecolour (with alpha) png
 *
 * IN:
 *		[char *] - the path to save at
 *		[framebuf *] - the framebuffer to save
 *
 * OUT: N/A
 */
int frame_png_dump(char *, framebuf *);


/*
 * Loads a given png file into a framebuffer
 *
 * IN:
 *		[char *] - the path to load from
 *
 * OUT: [framebuf * | NULL] - the framebuffer
 *							  NULL on error
 */
framebuf *frame_png_load(char *);

#endif
