/*
 * Greyscale comparison of two colour tuples
 * zero -> a >= b
 * non-zero -> a < b
 */
int cmp_t3(tup3 *a, tup3 *b)
{
    float a_avg = (a->x + a->y + a->z) / 3.0;
    float b_avg = (b->x + b->y + b->z) / 3.0;

    if (a_avg >= b_avg)
    {
        return 0;
    }
    return 1;
}


tup3 fragment(tup3 *frag_coord)
{
    tup3 sample_a, sample_b;

    // For even frames, compare (even_y, even_y + 1)
    if (! (FRAME_COUNT % 2))
    {
        // even_y -> min(even_y, even_y + 1)
        if (fmodf(frag_coord->y, 2.0))
        {
            framebuf_read(BACKBUF, (int) frag_coord->x, (int) frag_coord->y, &sample_a);
            if (! framebuf_read(BACKBUF, (int) frag_coord->x, ((int) frag_coord->y) + 1, &sample_b))
            {
                if (cmp_t3(&sample_a, &sample_b))
                {
                    return sample_b;
                }
            }

            return sample_a;
        }
        // odd_y -> min(odd_y - 1, odd_y)
        else
        {
            framebuf_read(BACKBUF, (int) frag_coord->x, (int) frag_coord->y, &sample_a);
            if (! framebuf_read(BACKBUF, (int) frag_coord->x, ((int) frag_coord->y) - 1, &sample_b))
            {
                if (! cmp_t3(&sample_a, &sample_b))
                {
                    return sample_b;
                }
            }

            return sample_a;
        }
    }
    // For odd frames, compare (even_y - 1, even_y)
    else
    {
        // even_y -> max(even_y - 1, even_y)
        if (fmodf(frag_coord->y, 2.0))
        {
            framebuf_read(BACKBUF, (int) frag_coord->x, (int) frag_coord->y, &sample_a);
            if (! framebuf_read(BACKBUF, (int) frag_coord->x, ((int) frag_coord->y) - 1, &sample_b))
            {
                if (! cmp_t3(&sample_a, &sample_b))
                {
                    return sample_b;
                }
            }

            return sample_a;
        }
        // odd_y -> min(odd_y, odd_y + 1)
        else
        {
            framebuf_read(BACKBUF, (int) frag_coord->x, (int) frag_coord->y, &sample_a);
            if (! framebuf_read(BACKBUF, (int) frag_coord->x, ((int) frag_coord->y) + 1, &sample_b))
            {
                if (cmp_t3(&sample_a, &sample_b))
                {
                    return sample_b;
                }
            }

            return sample_a;
        }
    }
}
