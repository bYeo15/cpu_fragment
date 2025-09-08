#include "optional/frame_png.h"

// -----===[ Functions ]===-----

typedef struct rgb_pix {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_pix;


static inline uint8_t prop_to_255(float prop)
{
    return (uint8_t) (prop * 255);
}


static inline rgb_pix t3_to_rgb(tup3 *t)
{
    rgb_pix rgb = { prop_to_255(t->x), prop_to_255(t->y), prop_to_255(t->z) };

    return rgb;
}

int frame_png_dump(char *path, framebuf *fb)
{
    // Try to open the provided path
    FILE *png_f = fopen(path, "w");

    if (png_f == NULL)
    {
        return 1;
    }

    png_structp png_ptr;
    png_infop info_ptr;
    png_byte **png_rows;
    int status = 0;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL)
		{
		    status = 2;
		    goto cleanup_pngf;
		}

		info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        status = 3;
        goto cleanup_pngptr;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        status = 4;
        goto cleanup_pngptr;
    }

    // Set PNG attributes (8 for single bit colour)
    png_set_IHDR(png_ptr, info_ptr, fb->dimx, fb->dimy,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Create PNG rows
    png_rows = png_malloc(png_ptr, fb->dimy * sizeof(png_byte *));
    tup3 curr_t3;
    for (unsigned int y = 0; y < fb->dimy; y++)
    {
        // * 3 is for rgb components (no alpha)
        png_byte *png_row = png_malloc(png_ptr, sizeof(uint8_t) * fb->dimx * 3);
        unsigned int row_x = 0;
        png_rows[y] = png_row;

        for (unsigned int x = 0; x < fb->dimx; x++)
        {
            framebuf_read(fb, x, y, &curr_t3);
            rgb_pix p = t3_to_rgb(&curr_t3);
            png_row[row_x++] = p.r;
            png_row[row_x++] = p.g;
            png_row[row_x++] = p.b;
        }
    }

    // Write to the PNG
    png_init_io(png_ptr, png_f);
    png_set_rows(png_ptr, info_ptr, png_rows);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // Free rows
    for (unsigned int y = 0; y < fb->dimy; y++)
    {
        png_free(png_ptr, png_rows[y]);
    }
    png_free(png_ptr, png_rows);

cleanup_pngptr:
    png_destroy_write_struct(&png_ptr, &info_ptr);
cleanup_pngf:
    fclose(png_f);
    return status;
}

framebuf *frame_png_load(char *path)
{
    // TODO
    (void) path;
    return NULL;
}
