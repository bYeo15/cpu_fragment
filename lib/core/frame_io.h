/*
 * Provides an interface for configuring how framebuffers are output, and loading framebuffers
 * from files into memory
 */

#ifndef FRAME_IO_H
#define FRAME_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "framebuffer.h"


// -----===[ Structures and Typedefs ]===-----

/*
 * A function that can write a framebuffer to a file
 *
 * IN:
 *      [char *] - the full path to save at
 *      [framebuf *] - the framebuffer to save
 *
 * OUT: N/A
 */
typedef void(*frame_dump)(char *, framebuf *);


/*
 * A struct that specifies how frames are to be saved
 *
 * Frame N shall be saved at <output_path>_<N>[.<output_ext>]
 *
 * output_path [char *] - the path to save each frame at (including a name)
 * output_ext [char * | NULL] - the extension to use for each file (optional)
 * dump_method [frame_dump] - a function by which a framebuffer can be saved as an image
 *                            of the desired format
 */
typedef struct frame_output {
    char *output_path;
    char *output_ext;
    frame_dump dump_method;
} frame_output;


// -----===[ Functions ]===-----

/*
 * Sets the frame_output configuration
 *
 * Will validate the path (for permission)
 *
 * IN:
 *      [char *] - the output path (no extension)
 *      [char *| NULL] - the output extension (NULL for no extension)
 *      [frame_dump] - the function for saving each framebuffer as an image
 *
 * OUT: N/A
 */
void set_frame_output(char *, char *, frame_dump);


/*
 * Frees the frame_output config
 *
 * IN: N/A
 *
 * OUT: N/A
 */
void free_frame_output(void);


/*
 * Saves a frame using the current configuration
 *
 * IN:
 *      [framebuf *] - the frame to save
 *      [unsigned long] - the frame number
 *
 * OUT: N/A
 */
void save_frame(framebuf *, unsigned long);

#endif
