#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../../lib/framebuffer.h"


static void framebuf_test_init(void **state)
{
	(void) state;

	framebuf *fb;

	fb = framebuf_init(10, 10);


	assert_non_null(fb);
	assert_int_equal(fb->dimx, 10);
	assert_int_equal(fb->dimy, 10);

	tup3 *pix;
	pix = fb->buf;

	assert_non_null(pix);
	
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			assert_float_equal(pix->x, 0.0f, TUP_EPSILON);
			assert_float_equal(pix->y, 0.0f, TUP_EPSILON);
			assert_float_equal(pix->z, 0.0f, TUP_EPSILON);

			pix++;
		}
	}

	framebuf_delete(fb);
}


int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(framebuf_test_init),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
