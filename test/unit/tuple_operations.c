#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../../lib/tuple.h"


static void tuple_check_add(void **state)
{
	(void) state;

	tup3 a, b, res;

	a = tuple3(3.0f, -2.0f, 5.0f, 1.0f);
	b = tuple3(-2.0f, 3.0f, 1.0f, 0.0f);

	res = add_t3(&a, &b);

	assert_float_equal(res.x, 1.0f, TUP_EPSILON);
	assert_float_equal(res.y, 1.0f, TUP_EPSILON);
	assert_float_equal(res.z, 6.0f, TUP_EPSILON);
	assert_float_equal(res.w, 1.0f, TUP_EPSILON);
}

static void tuple_check_sub_pp(void **state)
{
	(void) state;

	tup3 a, b, res;

	a = point3(3.0f, 2.0f, 1.0f);
	b = point3(5.0f, 6.0f, 7.0f);

	res = sub_t3(&a, &b);

	assert_float_equal(res.x, -2.0f, TUP_EPSILON);
	assert_float_equal(res.y, -4.0f, TUP_EPSILON);
	assert_float_equal(res.z, -6.0f, TUP_EPSILON);
	assert_float_equal(res.w, 0.0f, TUP_EPSILON);
}

static void tuple_check_sub_pv(void **state)
{
	(void) state;

	tup3 a, b, res;

	a = point3(3.0f, 2.0f, 1.0f);
	b = vec3(5.0f, 6.0f, 7.0f);

	res = sub_t3(&a, &b);

	assert_float_equal(res.x, -2.0f, TUP_EPSILON);
	assert_float_equal(res.y, -4.0f, TUP_EPSILON);
	assert_float_equal(res.z, -6.0f, TUP_EPSILON);
	assert_float_equal(res.w, 1.0f, TUP_EPSILON);
}

static void tuple_check_sub_vv(void **state)
{
	(void) state;

	tup3 a, b, res;

	a = vec3(3.0f, 2.0f, 1.0f);
	b = vec3(5.0f, 6.0f, 7.0f);

	res = sub_t3(&a, &b);

	assert_float_equal(res.x, -2.0f, TUP_EPSILON);
	assert_float_equal(res.y, -4.0f, TUP_EPSILON);
	assert_float_equal(res.z, -6.0f, TUP_EPSILON);
	assert_float_equal(res.w, 0.0f, TUP_EPSILON);
}

static void tuple_check_sub_vzero(void **state)
{
	(void) state;

	tup3 a, b, res;

	a = vec3(0.0f, 0.0f, 0.0f);
	b = vec3(1.0f, -2.0f, 3.0f);

	res = sub_t3(&a, &b);

	assert_float_equal(res.x, -1.0f, TUP_EPSILON);
	assert_float_equal(res.y, 2.0f, TUP_EPSILON);
	assert_float_equal(res.z, -3.0f, TUP_EPSILON);
	assert_float_equal(res.w, 0.0f, TUP_EPSILON);
}

static void tuple_check_neg(void **state)
{
	(void) state;

	tup3 a, res;

	a = tuple3(1.0f, -2.0f, 3.0f, -4.0f);

	res = neg_t3(&a);

	assert_float_equal(res.x, -1.0f, TUP_EPSILON);
	assert_float_equal(res.y, 2.0f, TUP_EPSILON);
	assert_float_equal(res.z, -3.0f, TUP_EPSILON);
	assert_float_equal(res.w, 4.0f, TUP_EPSILON);
}

static void tuple_check_mul_s(void **state)
{
	(void) state;

	tup3 a, res;
	float scalar;

	a = tuple3(1.0f, -2.0f, 3.0f, -4.0f);
	scalar = 3.5f;

	res = mul_t3(&a, scalar);

	assert_float_equal(res.x, 3.5f, TUP_EPSILON);
	assert_float_equal(res.y, -7.0f, TUP_EPSILON);
	assert_float_equal(res.z, 10.5f, TUP_EPSILON);
	assert_float_equal(res.w, -14.0f, TUP_EPSILON);
}

static void tuple_check_mul_f(void **state)
{
	(void) state;

	tup3 a, res;
	float scalar;

	a = tuple3(1.0f, -2.0f, 3.0f, -4.0f);
	scalar = 0.5f;

	res = mul_t3(&a, scalar);

	assert_float_equal(res.x, 0.5f, TUP_EPSILON);
	assert_float_equal(res.y, -1.0f, TUP_EPSILON);
	assert_float_equal(res.z, 1.5f, TUP_EPSILON);
	assert_float_equal(res.w, -2.0f, TUP_EPSILON);
}

static void tuple_check_div(void **state)
{
	(void) state;

	tup3 a, res;
	float scalar;

	a = tuple3(1.0f, -2.0f, 3.0f, -4.0f);
	scalar = 2.0f;

	res = div_t3(&a, scalar);

	assert_float_equal(res.x, 0.5f, TUP_EPSILON);
	assert_float_equal(res.y, -1.0f, TUP_EPSILON);
	assert_float_equal(res.z, 1.5f, TUP_EPSILON);
	assert_float_equal(res.w, -2.0f, TUP_EPSILON);
}

static void tuple_check_div_zero(void **state)
{
	(void) state;

	tup3 a, res;

	a = point3(3.0f, 2.0f, 1.0f);

	res = div_t3(&a, 0.0f);

	assert_float_equal(res.x, 0.0f, TUP_EPSILON);
	assert_float_equal(res.y, 0.0f, TUP_EPSILON);
	assert_float_equal(res.z, 0.0f, TUP_EPSILON);
	assert_float_equal(res.w, 0.0f, TUP_EPSILON);
}

static void tuple_check_mag(void **state)
{
	(void) state;

	tup3 a;
	float res;

	res = mag_t3(&vec3_right);
	assert_float_equal(res, 1.0f, TUP_EPSILON);

	res = mag_t3(&vec3_down);
	assert_float_equal(res, 1.0f, TUP_EPSILON);

	res = mag_t3(&vec3_forward);
	assert_float_equal(res, 1.0f, TUP_EPSILON);


	a = vec3(1.0f, 2.0f, 3.0f);

	res = mag_t3(&a);
	assert_float_equal(res, sqrtf(14.0f), TUP_EPSILON);

	a = vec3(-1.0f, -2.0f, -3.0f);

	res = mag_t3(&a);
	assert_float_equal(res, sqrtf(14.0f), TUP_EPSILON);
}

static void tuple_check_norm(void **state)
{
	(void) state;

	tup3 a, res;
	float mag_res;

	a = vec3(4.0f, 0.0f, 0.0f);

	res = norm_t3(&a);

	assert_float_equal(res.x, 1.0f, TUP_EPSILON);
	assert_float_equal(res.y, 0.0f, TUP_EPSILON);
	assert_float_equal(res.z, 0.0f, TUP_EPSILON);
	assert_float_equal(res.w, 0.0f, TUP_EPSILON);

	a = vec3(1.0f, 2.0f, 3.0f);

	res = norm_t3(&a);

	assert_float_equal(res.x, 1.0f / sqrtf(14.0f), TUP_EPSILON);
	assert_float_equal(res.y, 2.0f / sqrtf(14.0f), TUP_EPSILON);
	assert_float_equal(res.z, 3.0f / sqrtf(14.0f), TUP_EPSILON);
	assert_float_equal(res.w, 0.0f, TUP_EPSILON);

	mag_res = mag_t3(&res);

	assert_float_equal(mag_res, 1.0f, TUP_EPSILON);
}

static void tuple_check_dot(void **state)
{
	(void) state;

	tup3 a, b;
	float res;

	a = vec3(1.0f, 2.0f, 3.0f);
	b = vec3(2.0f, 3.0f, 4.0f);

	res = dot_t3(&a, &b);

	assert_float_equal(res, 20.0f, TUP_EPSILON);
}

static void tuple_check_cross(void **state)
{
	(void) state;

	tup3 a, b, res;

	a = vec3(1.0f, 2.0f, 3.0f);
	b = vec3(2.0f, 3.0f, 4.0f);

	res = cross_t3(&a, &b);

	assert_float_equal(res.x, -1.0f, TUP_EPSILON);
	assert_float_equal(res.y, 2.0f, TUP_EPSILON);
	assert_float_equal(res.z, -1.0f, TUP_EPSILON);
	assert_float_equal(res.w, 0.0f, TUP_EPSILON);

	res = cross_t3(&b, &a);

	assert_float_equal(res.x, 1.0f, TUP_EPSILON);
	assert_float_equal(res.y, -2.0f, TUP_EPSILON);
	assert_float_equal(res.z, 1.0f, TUP_EPSILON);
	assert_float_equal(res.w, 0.0f, TUP_EPSILON);
}


int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(tuple_check_add),
		cmocka_unit_test(tuple_check_sub_pp),
		cmocka_unit_test(tuple_check_sub_pv),
		cmocka_unit_test(tuple_check_sub_vv),
		cmocka_unit_test(tuple_check_sub_vzero),
		cmocka_unit_test(tuple_check_neg),
		cmocka_unit_test(tuple_check_mul_s),
		cmocka_unit_test(tuple_check_mul_f),
		cmocka_unit_test(tuple_check_div),
		cmocka_unit_test(tuple_check_div_zero),
		cmocka_unit_test(tuple_check_mag),
		cmocka_unit_test(tuple_check_norm),
		cmocka_unit_test(tuple_check_dot),
		cmocka_unit_test(tuple_check_cross),
	};
	
	return cmocka_run_group_tests(tests, NULL, NULL);
}
