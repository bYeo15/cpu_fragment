#include "core/fragment.h"
#include "core/frame_io.h"
#include "core/tuple.h"
#include "optional/frame_png.h"

#include <stdlib.h>


void frag_init(int argc, char **argv)
{
    if (argc < 4 || argc > 5)
    {
        puts("[ USAGE ] : <output path> <res_x> <res_y> [<n_frames>]");
        puts("<output path> : the path to use for frame output, minus an extension");
        puts("<res_x> : the x resolution to render at (in pixels)");
        puts("<res_y> : the y resolution to render at (in pixels)");
        puts("<n_frames> : optional (defaults to 1), the number of frames to render");
        exit(1);
    }

    n_threads = 8;
    n_jobs = 24;

    // Parse args
    char *err = NULL;

    int res_x = strtol(argv[2], &err, 10);
    if (*err != '\0' || res_x <= 0)
    {
        printf("[ ERROR ] : '%s' was not a valid x resolution\n", argv[2]);
        exit(2);
    }

    int res_y = strtol(argv[3], &err, 10);
    if (*err != '\0' || res_y <= 0)
    {
        printf("[ ERROR ] : '%s' was not a valid y resolution\n", argv[3]);
        exit(3);
    }

    if (argc < 5)
    {
        n_frames = 1;
    }
    else
    {
        n_frames = strtol(argv[4], &err, 10);
        if (*err != '\0' || n_frames <= 0)
        {
            printf("[ ERROR ] : '%s' was not a valid frame count\n", argv[4]);
            exit(4);
        }
    }

    // Set up render frame buffer
    create_render_frame(res_x, res_y);

    // Set up output
    set_frame_output(argv[1], "png", frame_png_dump);
}


void frag_cleanup()
{
    return;
}
