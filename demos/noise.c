float frag_rand(tup3 *st)
{
    tup3 v = vec3(12.9898, 78.233, 0.0);
    v = mul_t3(&v, FRAME_COUNT + 1);
    float iptr;
    return modff(sinf(dot_t3(st, &v)), &iptr) * 43758.543123;
}

tup3 fragment(tup3 *frag_coord)
{
    float fr = frag_rand(frag_coord);
    return col_xyz(fr, fr, fr);
}
