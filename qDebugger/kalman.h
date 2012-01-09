#if !defined( KALMAN_H )
#define KALMAN_H

struct KALMAN {
    float angle;
    float dt;
    float P[2][2];
    float q_bias;
    float rate;
    float R_angle;
    float Q_angle;
    float Q_gyro;
};

extern struct KALMAN kalman[2];

void KalmanStateUpdate(struct KALMAN *kalman, float q_m); // Pitch gyro measurement
float KalmanUpdate(struct KALMAN *kalman, float angle_m);// atan(X acceleration / Z acceleration) );

#endif
