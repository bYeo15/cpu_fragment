#include "core/fragment.h"
#include "core/frame_io.h"
#include "core/tuple.h"
#include "optional/frame_png.h"

#include <math.h>

int resx = 1920;
int resy = 1080;

float gamma_factor = 1.7;

tup3 gamma_correct(tup3 c, float gamma)
{
    float recip_gamma = 1.0 / gamma;
    tup3 res = { powf(c.x, recip_gamma), powf(c.y, recip_gamma), powf(c.z, recip_gamma), 0.0 };

    return res;
}

tup3 fragment(tup3 *frag_coord)
{
    tup3 norm_coord = { frag_coord->x / resx, frag_coord->y / resy, 0.0, 0.0 };
    return gamma_correct(norm_coord, gamma_factor);
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
