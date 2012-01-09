#if !defined( KALMAN_H )
#define KALMAN_H

#include <libopencm3/cm3/common.h>

struct KALMAN {
    double angle;
    double dt;
    double J[2][2];
    double P[2][2];
    double x_hat[2];
    double P_medicion;
    double P_gyro_bias;
    double P_teta;
};

extern struct KALMAN k[2];

void kalman_setup();
void KalmanUpdate(u8 n);//struct KALMAN *k, double teta, double gyro);

#endif
