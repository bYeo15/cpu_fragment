#include "core/fragment.h"
#include "core/frame_io.h"
#include "core/tuple.h"
#include "optional/frame_png.h"

#include <math.h>

int resx = 192;
int resy = 108;

float frag_rand(tup3 *st)
{
    tup3 v = vec3(12.9898, 78.233, 0.0);
    float iptr;
    return modff(sinf(dot_t3(st, &v)), &iptr) * 43758.543123;
}

tup3 fragment(tup3 *frag_coord)
{
    float fr = frag_rand(frag_coord);
    return col_xyz(fr, fr, fr);
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
