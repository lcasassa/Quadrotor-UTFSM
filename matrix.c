#include "matrix.h"

// R = rotation 3x3 matrix, angle = global angles 3x1 vector
void matrix_rotarion(float **R, float *angle) {

	float C[3], S[3];
	int i;
	for(i=0; i<3; i++) {
		C[i] = cos(angle[i]);
		S[i] = sin(angle[i]);
	}

	// matrix[row][col]
	R[0][0] =                  C[1]*C[2];
	R[0][1] = S[0]*S[1]*C[2] - C[0]*S[2];
	R[0][2] = C[0]*S[1]*C[2] + S[0]*S[2];
	R[1][0] =                  C[1]*S[2];
	R[1][1] = S[0]*S[1]*S[2] + C[0]*C[2];
	R[1][2] = C[0]*S[1]*S[2] - S[0]*C[2];
	R[2][0] =                - S[1]     ;
	R[2][1] =                  S[0]*C[1];
	R[2][2] =                  C[0]*C[1];

}

// R = rotation 3x3 matrix, v = 3x1 vector to rotate, v_R = result: 3x1 vector rotated
void matrix_rotate_vector(float **R, float *v, float *v_R) {
	v_R[0] = R[0][0]*v[0] + R[0][1]*v[1] + R[0][2]*v[2];
	v_R[1] = R[1][0]*v[0] + R[1][1]*v[1] + R[1][2]*v[2];
	v_R[2] = R[2][0]*v[0] + R[2][1]*v[1] + R[2][2]*v[2];
}


