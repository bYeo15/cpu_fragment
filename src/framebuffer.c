#include "../lib/framebuffer.h"

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
				*dest = fb->buf[x + (y *fb->dimx)];
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

	for (int y = 0; y < src->dimy; y++)
	{
		for (int x = 0; x < src->dimx; x++)
		{
			dest->buf[x + y * src->dimx] = src->buf[x + y * src->dimx];
		}
	}

	return 0;
}


int framebuf_to_ppm(framebuf *fb, char *path)
{
	FILE *dump_f;
	int curr_line_len;

	if ((dump_f = fopen(path, "w")) == NULL)
	{
		return -1;
	}
	
	// Write PPM header
	if (fwrite("P3\n", sizeof(char), 3, dump_f) != 3)
	{
		fclose(dump_f);
		return -2;
	}


	// Write width, height
	if (fprintf(dump_f, "%ud\n%ud\n", fb->dimx, fb->dimy) < 0)
	{
		fclose(dump_f);
		return -2;
	}

	
	// Write each pixel's data (with a max line length)
	curr_line_len = 0;
	tup3 *pixel = fb->buf;

	for (unsigned int y = 0; y < fb->dimy; y++)
	{
		for (unsigned int x = 0; x < fb->dimx; x++)
		{
			int n_bytes;
			int r, g, b;

			// Write either a space or a newline, as required
			if (curr_line_len > 58)
			{
				if (fwrite("\n", sizeof(char), 1, dump_f) != 1)
				{
					fclose(dump_f);
					return -2;
				}

				curr_line_len = 0;
			}
			else if (curr_line_len > 0)
			{
				if (fwrite(" ", sizeof(char), 1, dump_f) != 1)
				{
					fclose(dump_f);
					return -2;
				}

				curr_line_len += 1;
			}

			// Convert the current pixel's (x, y, z) colour to integer (r, g, b)
			r = (int)roundf(pixel->x * 255);
			g = (int)roundf(pixel->y * 255);
			b = (int)roundf(pixel->z * 255);

			// Clamp between 0 and 255
			// ~0xFF is all bits above 255
			// (check for any matches before handling individuals,
			// more efficient if it is assumed they will generally all
			// be in range)
			if (((r | g | b) & ~0xFF) != 0)
			{
				if ((r & ~0xFF) != 0)
				{
					r = ((~r) >> 31) & 0xFF;
				}
				if ((g & ~0xFF) != 0)
				{
					g = ((~g) >> 31) & 0xFF;
				}
				if ((b & ~0xFF) != 0)
				{
					b = ((~b) >> 31) & 0xFF;
				}
			}

			// Write each channel
			if ((n_bytes = fprintf(dump_f, "%03d %03d %03d", r, g, b)) < 0)
			{
				fclose(dump_f);
				return -2;
			}
			
			curr_line_len += n_bytes;

			// Move on to the next pixel
			pixel++;
		}

		// Add a newline as required
		if (curr_line_len > 0)
		{
			if (fwrite("\n", sizeof(char), 1, dump_f) != 1)
			{
				fclose(dump_f);
				return -2;
			}

			curr_line_len = 0;
		}
	}

	fclose(dump_f);

	return 0;
}


