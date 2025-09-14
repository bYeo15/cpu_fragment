/*
 * Greyscale comparison of two colour tuples
 * zero -> a >= b
 * non-zero -> a < b
 */
int cmp_t3(tup3 *a, tup3 *b)
{
    float a_avg = (a->x + a->y + a->z) / 3.0;
    float b_avg = (b->x + b->y + b->z) / 3.0;

    return (a_avg - b_avg >= 0.0);
}

/* Original odd-even sort
tup3 fragment(tup3 *frag_coord)
{
    tup3 sample_self, sample_neighbour;

    framebuf_read(BACKBUF, (int) frag_coord->x, (int) frag_coord->y, &sample_self);

    // Even frame
    int frame_even = 1 - (FRAME_COUNT % 2);

    // Even y
    int y_even = 1 - (((int) frag_coord->y) % 2);

    // Try to read neighbour
    if (! framebuf_read(BACKBUF, (int) frag_coord->x,
                     ((int) frag_coord->y) + (frame_even == y_even) - (frame_even != y_even),
                     &sample_neighbour))
    {
        // self >= neighbor
        if (frame_even != y_even)
        {
            if (cmp_t3(&sample_self, &sample_neighbour))
            {
                return sample_neighbour;
            }
        }
        // self < neighbour
        else if (frame_even == y_even)
        {
            return sample_neighbour;
        }
    }

    // No neighbour, retain current colour
    return sample_self;
}
*/

#define WINDOW_SIZE (16)

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


/* Nine pixel rolling window sort */
tup3 fragment(tup3 *frag_coord)
{
    tup3 samples[WINDOW_SIZE];
    int skipped_samples = 0;
    int n_samples = 0;

    int window_offset = FRAME_COUNT % WINDOW_SIZE;
    int self_index = (int) frag_coord->y;
    int start_y = self_index - ((self_index + window_offset) % WINDOW_SIZE);

    skipped_samples = start_y > 0 ? 0 : -(start_y);

    // Load in pixel window
    for (; n_samples + skipped_samples < WINDOW_SIZE && ! framebuf_read(BACKBUF, (int) frag_coord->x, start_y + n_samples + skipped_samples, samples + n_samples); n_samples++);

    // Sort pixel window
    swap_sort(samples, n_samples);

    return samples[(self_index % WINDOW_SIZE)];
}
