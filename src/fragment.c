#include "../lib/fragment.h"


// -----===[ Globals ]===-----

char *output_path = NULL;

framebuf *render_frame = NULL;


// -----===[ Global Uniforms ]===-----

framebuf *BACKBUF = NULL;

unsigned long FRAME_COUNT = 0;

unsigned long CLOCK_MS = 0;
stuct timespec prev_t;

float CONST_RAND = 0.0;


// -----===[ Exposed Functions ]===-----

void create_render_frame(unsigned int dimx, unsigned int dimy)
{
	if (render_frame == NULL)
	{
		render_frame = framebuf_init(dimx, dimy);

		if (render_frame == NULL)
		{
			fprintf(stderr, "[ ERROR ] : Not enough memory to create render framebuffer of size %d x %d\n",
					dimx, dimy);

			exit(1);
		}

		BACKBUF = framebuf_init(dimx, dimy);

		if (BACKBUF == NULL)
		{
			framebuf_delete(render_frame);

			fprintf(stderr, "[ ERROR ] : Not enough memory to create render framebuffer of size %d x %d\n",
					dimx, dimy);

			exit(1);
		}
	}
}


// -----===[ Internal Functions ]===-----

int fragment_main()
{
	// Update CLOCK_NS uniform
	unsigned long long diff_s = prev_t.tv_sec;
	unsigned long long diff_ns = prev_t.tv_nsec;
	
	clock_gettime(CLOCK_MONOTONIC, &prev_t);

	diff_s = prev_t.tv_sec - diff_s;
	diff_ns = prev_t.tv_nsec - diff_ns;

	CLOCK_NS += diff_ns + (diff_s * 1e9);

	// Enqueue all render jobs TODO

	// Wait for render jobs to be complete
	// TODO - pthread_barrier?
	
	// Save current frame TODO

	// Copy current frame into BACKBUF TODO
	
	// Update FRAME_COUNT uniform
	FRAME_COUNT++;

	return 1;
}



int main(int argc, char **argv)
{
	// TODO - Handle arguments
	
	// Compute CONST_RAND uniform
	srand(time(NULL));
	CONST_RAND = (float) rand() / (float) RAND_MAX;

	// Initialise CLOCK_NS
	clock_gettime(CLOCK_MONOTONIC, &prev_t);
	CLOCK_NS = 0;

	// TODO - Only provide with unused arguments
	frag_init(0, NULL);

	// Create queue of render jobs (calculate division based on number of threads)
	
	// Dispatch all threads
	
	// Enter main loop
	while (fragment_main());
	
	// Enqueue "quit" jobs once the rendering is done TODO

	// Clean up user resources
	frag_cleanup();
}
