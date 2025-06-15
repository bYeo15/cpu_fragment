#include "../lib/tuple.h"

// -----===[ Globals ]===-----

tup3 vec3_zero = { 0.0f, 0.0f, 0.0f, 0.0f };
tup3 vec3_left = { -1.0f, 0.0f, 0.0f, 0.0f };
tup3 vec3_right = { 1.0f, 0.0f, 0.0f, 0.0f };
tup3 vec3_up = { 0.0f, -1.0f, 0.0f, 0.0f };
tup3 vec3_down = { 0.0f, 1.0f, 0.0f, 0.0f };
tup3 vec3_forward = { 0.0f, 0.0f, 1.0f, 0.0f };
tup3 vec3_backward = { 0.0f, 0.0f, -1.0f, 0.0f };


tup3 point3_zero = { 0.0f, 0.0f, 0.0f, 1.0f };


// -----===[ Functions ]===-----

// < Tuple Creation >


inline tup3 tuple3(float x, float y, float z, float w)
{
	tup3 t3;

	t3.x = x;
	t3.y = y;
	t3.z = z;
	t3.w = w;

	return t3;
}


inline tup3 point3(float x, float y, float z)
{
	tup3 p3;

	p3.x = x;
	p3.y = y;
	p3.z = z;
	p3.w = 1.0f;
	
	return p3;
}


inline tup3 vec3(float x, float y, float z)
{
	tup3 v3;

	v3.x = x;
	v3.y = y;
	v3.z = z;
	v3.w = 0.0f;

	return v3;
}


inline tup3 col_xyz(float x, float y, float z)
{
	tup3 col;

	col.x = x;
	col.y = y;
	col.z = z;
	col.w = 1.0f;

	return col;
}


// < Tuple Operations >

int eq_t3(tup3 *a, tup3 *b)
{
	int eq;

	eq = 1;

	eq &= fabs(a->x - b->x) < TUP_EPSILON;
	eq &= fabs(a->y - b->y) < TUP_EPSILON;
	eq &= fabs(a->z - b->z) < TUP_EPSILON;
	eq &= fabs(a->w - b->w) < TUP_EPSILON;

	return eq;
}


tup3 add_t3(tup3 *a, tup3 *b)
{
	tup3 res;

	res.x = a->x + b->x;
	res.y = a->y + b->y;
	res.z = a->z + b->z;
	res.w = a->w + b->w;

	return res;
}


tup3 sub_t3(tup3 *a, tup3 *b)
{
	tup3 res;

	res.x = a->x - b->x;
	res.y = a->y - b->y;
	res.z = a->z - b->z;
	res.w = a->w - b->w;

	return res;
}


tup3 neg_t3(tup3 *a)
{
	tup3 res;

	res.x = -(a->x);
	res.y = -(a->y);
	res.z = -(a->z);
	res.w = -(a->w);

	return res;
}


tup3 mul_t3(tup3 *a, float s)
{
	tup3 res;

	res.x = a->x * s;
	res.y = a->y * s;
	res.z = a->z * s;
	res.w = a->w * s;

	return res;
}


tup3 hadamard_t3(tup3 *a, tup3 *b)
{
	tup3 res;

	res.x = a->x * b->x;
	res.y = a->y * b->x;
	res.z = a->z * b->x;
	res.w = a->w * b->x;

	return res;
}


tup3 div_t3(tup3 *a, float s)
{
	tup3 res;

	// Catch divide by zero
	if (fabs(s) < TUP_EPSILON)
	{
		res.x = 0.0f;
		res.y = 0.0f;
		res.z = 0.0f;
		res.w = 0.0f;

		return res;
	}

	res.x = a->x / s;
	res.y = a->y / s;
	res.z = a->z / s;
	res.w = a->w / s;

	return res;
}


float mag_t3(tup3 *a)
{
	float mag;

	mag = a->x * a->x;
	mag += a->y * a->y;
	mag += a->z * a->z;

	mag = sqrtf(mag);

	return mag;
}


tup3 norm_t3(tup3 *a)
{
	float mag;
	tup3 res;

	mag = mag_t3(a);

	if (fabs(mag) < TUP_EPSILON)
	{
		res.x = 0.0f;
		res.y = 0.0f;
		res.z = 0.0f;
		res.w = 0.0f;

		return res;
	}

	res.x = a->x / mag;
	res.y = a->y / mag;
	res.z = a->z / mag;
	res.w = a->w;

	return res;
}


float dot_t3(tup3 *a, tup3 *b)
{
	return a->x * b->x +
		   a->y * b->y +
		   a->z * b->z +
		   a->w * b->w;
}


tup3 cross_t3(tup3 *a, tup3 *b)
{
	tup3 res;

	res.x = a->y * b->z - a->z * b->y;
	res.y = a->z * b->x - a->x * b->z;
	res.z = a->x * b->y - a->y * b->x;
	res.w = 0.0f;

	return res;
}
