#include "core/fragment.h"
#include "core/frame_io.h"
#include "optional/frame_png.h"


int resx = 1920;
int resy = 1080;


tup3 fragment(tup3 *frag_coord)
{
    tup3 norm_coord = { frag_coord->x / resx, frag_coord->y / resx, 0.0, 0.0 };
    return norm_coord;
}

void frag_init(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    create_render_frame(resx, resy);
    set_frame_output("./frag", "png", frame_png_dump);
}

void frag_cleanup()
{
    return;
}
