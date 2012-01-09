/* -*- indent-tabs-mode:T; c-basic-offset:8; tab-width:8; -*- vi: set ts=8:
 * $Id: tilt.c,v 1.1 2003/07/09 18:23:29 john Exp $
 *
 * 1 dimensional tilt sensor using a dual axis accelerometer
 * and single axis angular rate gyro.  The two sensors are fused
 * via a two state Kalman filter, with one state being the angle
 * and the other state being the gyro bias.
 *
 * Gyro bias is automatically tracked by the filter.  This seems
 * like magic.
 *
 * Please note that there are lots of comments in the functions and
 * in blocks before the functions.  Kalman filtering is an already complex
 * subject, made even more so by extensive hand optimizations to the C code
 * that implements the filter.  I've tried to make an effort of explaining
 * the optimizations, but feel free to send mail to the mailing list,
 * autopilot-devel@lists.sf.net, with questions about this code.
 *
 * 
 * (c) 2003 Trammell Hudson <hudson@rotomotion.com>
 *
 *************
 *
 *  This file is part of the autopilot onboard code package.
 *  
 *  Autopilot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  Autopilot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with Autopilot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <math.h>
#include "kalman.h"
#include "ADXL345.h"
#include "ITG3200.h"
#include <stdio.h>

/*
struct KALMAN {
    double angle;
    double dt;
//    double J[2][2];
    double P[2][2];
    double x_hat[2];
    double P_medicion;
    double P_gyro_bias;
    double P_teta;
};
*/

struct KALMAN k[2];
/* = {
   {// kalman[0]
	0,		// angle
	0.01,		// dt
	{{0,0},{0,0}},	// J
	{{1,0},{0,1}},	// P
	{0,0},		// x_hat
	0.001,		// P_gyro_bias
	0.003,		// P_teta
  },
  };
  { // kalman[1]
	0,		// angle
	0.01,		// dt
	{{0,0},{0,0}},	// J
	{{0,0},{0,0}},	// P
	{0,0},		// x_hat
	0.001,		// P_gyro_bias
	0.003,		// P_teta
  }
};*/

void kalman_setup() {
    u8 n=0;
    k[n].angle = 0;
    k[n].dt = 0.002;
    k[n].J[0][0] = 0.0;
    k[n].J[0][1] = 0.0;
    k[n].J[1][0] = 0.0;
    k[n].J[1][1] = 0.0;
    k[n].P[0][0] = 1.0;
    k[n].P[0][1] = 0.0;
    k[n].P[1][0] = 0.0;
    k[n].P[1][1] = 1.0;
    k[n].x_hat[0] = 0.0;
    k[n].x_hat[1] = 0.0;
    k[n].P_medicion = 0.3;
    k[n].P_teta = 0.001;
    k[n].P_gyro_bias = 0.003;
    k[n].angle = 0;
    n++;
    k[n].dt = 0.002;
    k[n].J[0][0] = 0.0;
    k[n].J[0][1] = 0.0;
    k[n].J[1][0] = 0.0;
    k[n].J[1][1] = 0.0;
    k[n].P[0][0] = 1.0;
    k[n].P[0][1] = 0.0;
    k[n].P[1][0] = 0.0;
    k[n].P[1][1] = 1.0;
    k[n].x_hat[0] = 0.0;
    k[n].x_hat[1] = 0.0;
    k[n].P_medicion = 0.3;
    k[n].P_teta = 0.001;
    k[n].P_gyro_bias = 0.003;
}

void KalmanUpdate(u8 n) { //struct KALMAN *k, double teta, double gyro) {
        double q, Pdot[2][2];
        double angle_err, C_0, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;                                               
	//double J_[2][2], P_[2][2], x_hat_[2];
	double teta;
	double gyro;
//	struct KALMAN *k;

	if(n==1) {
		teta = atan2((double)-acelerometer[0],(double)acelerometer[2]);
		gyro = (double)gyroscope[1]*M_PI/14.375/180;
	} else {
		teta = atan2((double)acelerometer[1],(double)acelerometer[2]);
		gyro = (double)gyroscope[0]*M_PI/14.375/180;
	}

//	k = &kalman[0];



                        
        /* Unbias our gyro */
        q = gyro - k[n].x_hat[1];

        /*
         * Compute the derivative of the covariance matrix
         *
         *      Pdot = A*P + P*A' + Q
         *
         * We've hand computed the expansion of A = [ 0 -1, 0 0 ] multiplied
         * by P and P multiplied by A' = [ 0 0, -1, 0 ].  This is then added
         * to the diagonal elements of Q, which are Q_angle and Q_gyro.
         */
        Pdot[0][0] = k[n].P_teta - k[n].P[0][1] - k[n].P[1][0];  /* 0,0 */
        Pdot[0][1] = - k[n].P[1][1];            /* 0,1 */
        Pdot[1][0] = - k[n].P[1][1];            /* 1,0 */
        Pdot[1][1] = k[n].P_gyro_bias;                       /* 1,1 */

        /* Store our unbias gyro estimate */
        //k[n].rate = q;

        /*
         * Update our angle estimate
         * angle += angle_dot * dt
         *       += (gyro - gyro_bias) * dt
         *       += q * dt
         */
	k[n].x_hat[0] += q * k[n].dt;

        /* Update the covariance matrix */
        k[n].P[0][0] += Pdot[0][0] * k[n].dt;
        k[n].P[0][1] += Pdot[0][1] * k[n].dt;
        k[n].P[1][0] += Pdot[1][0] * k[n].dt;
        k[n].P[1][1] += Pdot[1][1] * k[n].dt;





        /* Compute our measured angle and the error in our estimate */
        //angle_m = atan2( -az_m, ax_m );
//      angle_m = atan( -ax_m/ az_m );
        angle_err = teta - k[n].angle;

        /*
         * C_0 shows how the state measurement directly relates to
         * the state estimate.
         *
         * The C_1 shows that the state measurement does not relate
         * to the gyro bias estimate.  We don't actually use this, so
         * we comment it out.
         */
        C_0 = 1;
        /* C_1 = 0; */

        /*
         * PCt<2,1> = P<2,2> * C'<2,1>, which we use twice.  This makes
         * it worthwhile to precompute and store the two values.
         * Note that C[0,1] = C_1 is zero, so we do not compute that
         * term.
         */
        PCt_0 = C_0 * k[n].P[0][0]; /* + C_1 * P[0][1] = 0 */
        PCt_1 = C_0 * k[n].P[1][0]; /* + C_1 * P[1][1] = 0 */
                
        /*
         * Compute the error estimate.  From the Kalman filter paper:
         * 
         *      E = C P C' + R
         * 
         * Dimensionally,
         *
         *      E<1,1> = C<1,2> P<2,2> C'<2,1> + R<1,1>
         *
         * Again, note that C_1 is zero, so we do not compute the term.
         */
        E = k[n].P_medicion
                + C_0 * PCt_0
        /*      + C_1 * PCt_1 = 0 */
        ;

        /*
         * Compute the Kalman filter gains.  From the Kalman paper:
         *
         *      K = P C' inv(E)
         *
         * Dimensionally:
         *
         *      K<2,1> = P<2,2> C'<2,1> inv(E)<1,1>
         *
         * Luckilly, E is <1,1>, so the inverse of E is just 1/E.
         */
if(isnan(E) || E == 0) { long long int i; printf("nan!\r\n"); for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }}

        K_0 = PCt_0 / E;
        K_1 = PCt_1 / E;
                
        /*
         * Update covariance matrix.  Again, from the Kalman filter paper:
         *
         *      P = P - K C P
         *
         * Dimensionally:
         *
         *      P<2,2> -= K<2,1> C<1,2> P<2,2>
         *
         * We first compute t<1,2> = C P.  Note that:
         *
         *      t[0,0] = C[0,0] * P[0,0] + C[0,1] * P[1,0]
         *
         * But, since C_1 is zero, we have:
         *
         *      t[0,0] = C[0,0] * P[0,0] = PCt[0,0]
         *
         * This saves us a floating point multiply.
         */
        t_0 = PCt_0; /* C_0 * P[0][0] + C_1 * P[1][0] */
        t_1 = C_0 * k[n].P[0][1]; /* + C_1 * P[1][1]  = 0 */

        k[n].P[0][0] -= K_0 * t_0;
        k[n].P[0][1] -= K_0 * t_1;
        k[n].P[1][0] -= K_1 * t_0;
        k[n].P[1][1] -= K_1 * t_1;
        
        /*
         * Update our state estimate.  Again, from the Kalman paper:
         *
         *      X += K * err
         *
         * And, dimensionally,
         *
         *      X<2> = X<2> + K<2,1> * err<1,1>
         *
         * err is a measurement of the difference in the measured state
         * and the estimate state.  In our case, it is just the difference
         * between the two accelerometer measured angle and our estimated
         * angle.
         */
        k[n].x_hat[0]  += K_0 * angle_err;
        k[n].x_hat[1]  += K_1 * angle_err;


	k[n].angle = k[n].x_hat[0];



//	printf("t: %3.0f g: %5.3f", teta, gyro);
//	printf(" => %f\r\n", k[n].angle);

	return;

}

