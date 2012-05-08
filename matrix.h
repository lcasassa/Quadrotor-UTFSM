#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>

void matrix_rotarion(float **R, float *angle);
void matrix_rotate_vector(float **R, float *v, float *v_R);

/* Example:
 *
 * float R[3][3];
 * float angles[3] = {M_PI/2, 0, M_PI} // (90, 0, 180) degrees
 * float vector[3] = {1,0,0};
 * float vector_rotated[3];
 *
 * matrix_rotation(R, angles);
 * matrix_rotate_vector(R, vector, vector_rotated);
 *
 * printf("(%f, %f, %f)", vector_rotated[0], vector_rotated[1], vector_rotated[2]);
 * 
 */

#endif
