// https://stackoverflow.com/a/16659263
float fclamp(float f, float min, float max)
{
    const float t = f < min ? min : f;
    return t > max ? max : t;
}

#define TO_PROP(f) (fclamp((f), 0.0, 1.0))

// Reverse epsilon (factor to multiply proportional values by before subtracting to int)
#define REV_EPSILON (10000.0)

/*
 * Greyscale comparison of two colour tuples
 */
int cmp_t3(const void *a, const void *b)
{
    tup3 *ta = (tup3 *) a;
    tup3 *tb = (tup3 *) b;
    float a_avg = (TO_PROP(ta->x) + TO_PROP(ta->y) + TO_PROP(ta->z));
    float b_avg = (TO_PROP(tb->x) + TO_PROP(tb->y) + TO_PROP(tb->z));

    return (int) ((a_avg - b_avg) * REV_EPSILON);
}


#define MAX_WINDOW_SIZE (128)
#define WINDOW_GROWTH (3)

/* Widening window sort */
tup3 fragment(tup3 *frag_coord)
{
    tup3 samples[MAX_WINDOW_SIZE];
    int n_samples = 0;
    int window_size = 3 * (FRAME_COUNT + 1);

    window_size = window_size > MAX_WINDOW_SIZE ? MAX_WINDOW_SIZE : window_size;

    int self_index = (int) frag_coord->y;
    int start_y = (self_index / window_size) * window_size;

    // Load in pixel window
    for (; n_samples < window_size && ! framebuf_read(BACKBUF, (int) frag_coord->x, start_y + n_samples, samples + n_samples); n_samples++);

    // Sort pixel window
    qsort(samples, n_samples, sizeof(tup3), cmp_t3);

    return samples[(self_index % window_size)];
}
