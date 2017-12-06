#ifndef MOTOR_H_

#define MOTOR_H_
#define steering(x) (TFC_SetServo(0, x))
#define ADD_BRAKE_COUNTER 0.1

int straightCounter;
int leftCounter;
int rightCounter;
float angle;
float prevAngle;
volatile float wishSpeedA;
volatile float wishSpeedB;
volatile float currentSpeedA;
volatile float currentSpeedB;

void initMotor();
void calibrateServo();
void startMotor();
void stopMotor();
void setSpeed();
void calculateSteering();

#endif /* MOTOR_H_ */
