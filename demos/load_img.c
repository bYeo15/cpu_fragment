tup3 fragment(tup3 *frag_coord)
{
    tup3 sample;

    framebuf_read(BACKBUF, (int) frag_coord->x, (int) frag_coord->y, &sample);
    return sample;
}
