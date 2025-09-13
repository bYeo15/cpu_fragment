float gamma_factor = 1.7;

tup3 gamma_correct(tup3 c, float gamma)
{
    float recip_gamma = 1.0 / gamma;
    tup3 res = { powf(c.x, recip_gamma), powf(c.y, recip_gamma), powf(c.z, recip_gamma), 0.0 };

    return res;
}

tup3 fragment(tup3 *frag_coord)
{
    tup3 norm_coord = { frag_coord->x / FRAME_DIM.x, frag_coord->y / FRAME_DIM.y, 0.0, 0.0 };
    return gamma_correct(norm_coord, gamma_factor);
}
