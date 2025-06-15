#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../../lib/tuple.h"


static void tuple_check_point(void **state)
{
	(void) state;

	tup3 p = point3(1.0f, 0.0f, -1.0f);

	assert_float_equal(p.w, 1.0f, TUP_EPSILON);
}


static void tuple_check_vec(void **state)
{
	(void) state;

	tup3 v = vec3(1.0f, 0.0f, -1.0f);

	assert_float_equal(v.w, 0.0f, TUP_EPSILON);
}


static void tuple_check_coord(void **state)
{
	(void) state;

	tup3 p = point3(1.5f, 0.5f, -0.5f);

	assert_float_equal(p.x, 1.5f, TUP_EPSILON);
	assert_float_equal(p.y, 0.5f, TUP_EPSILON);
	assert_float_equal(p.z, -0.5f, TUP_EPSILON);
}

static void tuple_check_component(void **state)
{
	(void) state;

	tup3 v= vec3(1.5f, 0.5f, -0.5f);

	assert_float_equal(v.x, 1.5f, TUP_EPSILON);
	assert_float_equal(v.y, 0.5f, TUP_EPSILON);
	assert_float_equal(v.z, -0.5f, TUP_EPSILON);
}


int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(tuple_check_point),
		cmocka_unit_test(tuple_check_vec),
		cmocka_unit_test(tuple_check_coord),
		cmocka_unit_test(tuple_check_component),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
