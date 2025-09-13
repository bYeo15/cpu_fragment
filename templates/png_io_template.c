#include "core/fragment.h"
#include "core/frame_io.h"
#include "core/tuple.h"
#include "optional/frame_png.h"

#include <stdlib.h>

framebuf *sampler = NULL;

void frag_init(int argc, char **argv)
{
    if (argc < 3 || argc > 4)
    {
        puts("[ USAGE ] : <input image path> <output path> [<n_frames>]");
        puts("<input image path> : the path to the png file to use as input");
        puts("<output path> : the path to use for frame output, minus an extension");
        puts("<n_frames> : optional (defaults to 1), the number of frames to render");
        exit(1);
    }

    sampler = frame_png_load(argv[1]);

    if (sampler == NULL)
    {
        printf("[ ERROR ] : Failed to load image '%s'\n", argv[1]);
    }

    n_threads = 8;
    n_jobs = 24;

    if (argc < 4)
    {
        n_frames = 1;
    }
    else
    {
        char *err = NULL;
        n_frames = strtol(argv[3], &err, 10);
        if (*err != '\0' || n_frames < 1)
        {
            printf("[ ERROR ] : '%s' was not a valid frame count\n", argv[3]);
            framebuf_delete(sampler);
            exit(2);
        }
    }

    // Set up render frame buffer
    create_render_frame(sampler->dimx, sampler->dimy);

    // Copy sampler into the backbuffer
    framebuf_copy(BACKBUF, sampler);

    // Remove the sampler
    framebuf_delete(sampler);

    // Set up output
    set_frame_output(argv[2], "png", frame_png_dump);
}


void frag_cleanup()
{
    return;
}
