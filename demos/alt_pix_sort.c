// https://stackoverflow.com/a/16659263
double fclamp(float f, float min, float max)
{
    const float t = f < min ? min : f;
    return t > max ? max : t;
}

#define TO_PROP(f) (fclamp((f), 0.0, 1.0))

/*
 * Greyscale comparison of two colour tuples
 * zero -> a >= b
 * non-zero -> a < b
 */
int cmp_t3(tup3 *a, tup3 *b)
{
    float a_avg = (TO_PROP(a->x) + TO_PROP(a->y) + TO_PROP(a->z)) / 3.0;
    float b_avg = (TO_PROP(b->x) + TO_PROP(b->y) + TO_PROP(b->z)) / 3.0;

    return (a_avg - b_avg >= 0.0);
}

#define WINDOW_SIZE (4)
#define INTERLEAVE_GAP (4)

// Sort (1-n elements)
void swap_sort(tup3 *arr, int size)
{
    if (size < 2 || size > WINDOW_SIZE)
    {
        return;
    }

    // Naive O(n^2) min sort
    for (int i = 0; i < size; i++)
    {
        // Find smallest
        int smallest_i = i;
        for (int j = i + 1; j < size; j++)
        {
            if (! cmp_t3(arr + smallest_i, arr + j))
            {
                smallest_i = j;
            }
        }

        // Swap
        tup3 tmp = arr[i];
        arr[i] = arr[smallest_i];
        arr[smallest_i] = tmp;
    }
}


/*
 * Pixel sort that alternates between fixed local and interleaved windows
 */
tup3 fragment(tup3 *frag_coord)
{
    tup3 samples[WINDOW_SIZE];
    int n_samples = 0;

    int self_index = (int) frag_coord->y;

    // Even frame - local window
    if (! (FRAME_COUNT % 2))
    {
        int start_y = (self_index / WINDOW_SIZE) * WINDOW_SIZE;
        // Load in local pixel window
        for (; n_samples < WINDOW_SIZE && ! framebuf_read(BACKBUF, (int) frag_coord->x, start_y + n_samples, samples + n_samples); n_samples++);
    }
    else
    {
        int start_y = (self_index / WINDOW_SIZE) * WINDOW_SIZE + (self_index % INTERLEAVE_GAP);
        // Load in interleaved pixel window
        for (; n_samples < WINDOW_SIZE && ! framebuf_read(BACKBUF, (int) frag_coord->x, start_y + n_samples * INTERLEAVE_GAP, samples + n_samples); n_samples++);
    }

    // Sort pixel window
    swap_sort(samples, n_samples);

    return samples[(self_index % WINDOW_SIZE)];
}
