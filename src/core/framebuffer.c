#include "core/framebuffer.h"

// < Framebuffer Memory >

framebuf *framebuf_init(unsigned int dimx, unsigned int dimy)
{
    framebuf *new_fb;
    tup3 *new_buf;

    // Try to create the framebuf itself
    if ((new_fb = malloc(sizeof(framebuf))) == NULL)
    {
        return NULL;
    }

    // Try to create the new pixel buffer for the framebuf
    if ((new_buf = malloc(sizeof(tup3) * dimx * dimy)) == NULL)
    {
        free(new_fb);
        return NULL;
    }

    new_fb->dimx = dimx;
    new_fb->dimy = dimy;
    new_fb->buf = new_buf;


    // Initialise the buffer to all opaque black
    tup3 black = col_xyz(0.0f, 0.0f, 0.0f);

    for (unsigned int y = 0; y < dimy; y++)
    {
        for (unsigned int x = 0; x < dimx; x++)
        {
            new_buf[x + (y * dimx)] = black;
        }
    }

    return new_fb;
}


void framebuf_delete(framebuf *fb)
{
    free(fb->buf);
    free(fb);
}


// < Framebuffer Operations >

int framebuf_write(framebuf *fb, unsigned int x, unsigned int y, tup3 *colour)
{
    // Ensure write is within framebuf bounds
    if (x < fb->dimx)
    {
        if (y < fb->dimy)
        {
            fb->buf[x + (y * fb->dimx)] = *colour;
            return 0;
        }
    }

    return -1;
}


int framebuf_read(framebuf *fb, unsigned int x, unsigned int y, tup3 *dest)
{
    // Ensure read is within framebuf bounds
    if (x < fb->dimx)
    {
        if (y < fb->dimy)
        {
            if (dest != NULL)
            {
                *dest = fb->buf[x + (y * fb->dimx)];
            }

            return 0;
        }
    }

    return -1;
}


int framebuf_copy(framebuf *dest, framebuf *src)
{
    if (dest == src)
    {
        return -1;
    }

    if (dest->dimx != src->dimx || dest->dimy != src->dimy)
    {
        return 1;
    }

    for (unsigned int y = 0; y < src->dimy; y++)
    {
        for (unsigned int x = 0; x < src->dimx; x++)
        {
            dest->buf[x + y * src->dimx] = src->buf[x + y * src->dimx];
        }
    }

    return 0;
}
