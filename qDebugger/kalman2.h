#if !defined( KALMAN2_H )
#define KALMAN2_H

struct KALMAN2 {
    float x[3];
    float P[3][3];
    float J[3][2];
    float Ptheta;
    float Pomega;
    float Pbias;
    float PsensorTheta;
    float PsensorOmega;
    float h;
};

extern struct KALMAN2 kalman2[2];

float Kalman2(struct KALMAN2 *kalman, float motor, float theta, float omega);
void Kalman2_init(struct KALMAN2 *kalman);


#endif
