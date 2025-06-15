/*
 * A tuple is a fixed-size collection of floating point values.
 *
 * A tuple can represent either a point or a vector. This is indicated by an additional
 * component (0 for vector, 1 for point).
 * Colours use the fourth component for a (currently unused) alpha channel
 */

#ifndef TUPLE_H
#define TUPLE_H

#include <math.h>

// -----===[ Definitions ]===-----

/*
 * Comparison factor for floating-point comparisons between tuples
 */
#define TUP_EPSILON 0.00001


// -----===[ Structures ]===-----

/*
 * A 3-dimensional tuple
 */
typedef struct tup3 {
	float x;
	float y;
	float z;
	float w;
} tup3;


// -----===[ Globals ]===-----

extern tup3 vec3_zero;
extern tup3 vec3_left;
extern tup3 vec3_right;
extern tup3 vec3_up;
extern tup3 vec3_down;
extern tup3 vec3_forward;
extern tup3 vec3_backward;

extern tup3 point3_zero; 


// -----===[ Functions ]===-----

// < Tuple Creation >

/*
 * Creates a new arbitrary tuple with the given components
 *
 * IN:
 *		[float] - the x component
 *		[float] - the y component
 *		[float] - the z component
 *		[float] - the w component
 *
 * OUT: [tup3] - a tuple with those components
 */
tup3 tuple3(float, float, float, float);


/*
 * Creates a new point tuple with the given coordinates
 *
 * IN:
 *		[float] - the x coordinate
 *		[float] - the y coordinate
 *		[float] - the z coordinate
 *
 * OUT: [tup3] - a tuple with those coordinates
 */
tup3 point3(float, float, float);


/*
 * Creates a new vector tuple with the given components
 *
 * IN:
 *		[float] - the x component 
 *		[float] - the y component
 *		[float] - the z component
 *
 * OUT: [tup3] - a tuple with those components
 */
tup3 vec3(float, float, float);


/*
 * Creates a new XYZ colour tuple 
 *
 * IN:
 *		[float [0, 1]] - the x colour channel (red)
 *		[float [0, 1]] - the y colour channel (green)
 *		[float [0, 1]] - the z colour channel (blue)
 *		
 * OUT: [tup3] - a tuple representing that colour
 */
tup3 col_xyz(float, float, float);


// < Tuple Operations >


/*
 * Checks tuple equality
 *
 * IN:
 *		[tup3 *] - the first tuple to compare
 *		[tup3 *] - the second tuple to compare
 *
 * OUT: [int] - 0: they differ in at least one component
 *				1: they have the same components
 */
int eq_t3(tup3 *, tup3 *);


/*
 * Adds two tuples component-wise
 *
 * IN:
 *		[tup3 *] - the first tuple to add
 *		[tup3 *] - the second tuple to add
 *
 * OUT: [tup3] - the result of the addition
 */
tup3 add_t3(tup3 *, tup3 *);


/*
 * Subtracts one tuple from another component-wise
 *
 * IN:
 *		[tup3 *] - the tuple to subtract FROM
 *		[tup3 *] - the tuple to subtract
 *
 * OUT: [tup3] - the result of the subtraction
 */
tup3 sub_t3(tup3 *, tup3 *);


/*
 * Negates a given tuple, including the w-component
 *
 * IN:
 *		[tup3 *] - the tuple to negate
 *
 * OUT: [tup3] - the negated tuple
 */
tup3 neg_t3(tup3 *);


/*
 * Performs scalar multiplication of a tuple
 *
 * IN:
 *		[tup3 *] - the tuple to scale
 *		[float] - the scaling factor
 *
 * OUT: [tup3] - the scaled tuple
 */
tup3 mul_t3(tup3 *, float);


/*
 * Performs component-wise multiplication of tuples
 *
 * IN:
 *		[tup3 *] - the first tuple
 *		[tup3 *] - the second tuple
 *
 * OUT: [tup3] - the result
 */
tup3 hadamard_t3(tup3 *, tup3 *);


/*
 * Performs scalar division of a tuple
 *
 * IN:
 *		[tup3 *] - the tuple to scale
 *		[float] - the scaling factor
 *
 * OUT: [tup3] - the scaled tuple
 */
tup3 div_t3(tup3 *, float);


/*
 * Determines the magnitude of a tuple
 *
 * IN:
 *		[tup3 *] - the tuple to calculate the magnitude for
 *
 * OUT: [float] - the magnitude of the tuple
 */
float mag_t3(tup3 *);


/*
 * Normalises a tuple
 *
 * IN:
 *		[tup3 *] - the tuple to normalise
 *
 * OUT: [tup3] - the normalised tuple
 */
tup3 norm_t3(tup3 *);


/*
 * Calculates the dot product of two tuples
 *
 * IN:
 *		[tup3 *] - the first tuple to dot
 *		[tup3 *] - the second tuple to dot
 *
 * OUT: [float] - the dot product of the two tuples
 */
float dot_t3(tup3 *, tup3 *);


/*
 * Calculates the cross product of two tuples
 *
 * IN:
 *		[tup3 *] - the first tuple to cross
 *		[tup3 *] - the second tuple to cross 
 *
 * OUT: [tup3] - the cross product of the two tuples
 */
tup3 cross_t3(tup3 *, tup3 *);

#endif
