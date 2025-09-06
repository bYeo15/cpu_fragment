#include "../../lib/optional/frame_png.h"

// -----===[ Functions ]===-----

void frame_png_dump(char *path, framebuf *fb)
{
	// Try to open the provided path
	FILE *png_f = fopen(path, "w");

	if (png_f == NULL)
	{
		fprintf(stderr, "[ ERROR ] : Cannot open file '%s' for saving framebuffer\n", path);

		exit(1);
	}

}

framebuf *frame_png_load(char *path)
{
	return NULL;
}
