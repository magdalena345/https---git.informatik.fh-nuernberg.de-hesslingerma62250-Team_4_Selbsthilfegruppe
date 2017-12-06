#include "OhmRacer/OhmRacer.h"

void initMotor()
{
	calibrateServo();	
	TFC_InitMotorPWM();
	straightCounter = 0;
	leftCounter = 0;
	rightCounter = 0;
} 
void calibrateServo()
{
	TFC_SetServo(0,1);
	WAITING(550);
	TFC_SetServo(0,-1);
	WAITING(550);
	TFC_SetServo(0,0.15); //ursprünglich: TFC_SetServo(0,0)
}
void startMotor()
{
	TFC_SetMotorPWM(0, 0);
	TFC_HBRIDGE_ENABLE;
}
void stopMotor()
{
	TFC_SetMotorPWM(0, 0);
	TFC_HBRIDGE_DISABLE;
}
void setSpeed()
{
	//B left motor, A right motor
	if(currentSpeedA == wishSpeedA)
	{
		currentSpeedA = wishSpeedA;
	}
	if(currentSpeedA > wishSpeedA)
	{
		currentSpeedA = wishSpeedA;
	}
	if(currentSpeedA < wishSpeedA)
	{
		currentSpeedA += ADD_BRAKE_COUNTER;
	}
	if(currentSpeedB == wishSpeedB)
	{
		currentSpeedB = wishSpeedB;
	}
	if(currentSpeedB > wishSpeedB)
	{
		currentSpeedB = wishSpeedB;
	}
	if(currentSpeedB < wishSpeedB)
	{
		currentSpeedB += ADD_BRAKE_COUNTER;
	}
	TFC_SetMotorPWM(0, 0);
}
void calculateSteering()
 {
	 switch (steerAction)
	 {
		 case 0:
			 straightCounter++;
			 if(straightCounter < 2)
			 {
				 angle = prevAngle;
				 wishSpeedA = 0.4;
				 wishSpeedB = 0.4;
			 }
			 else
			 {
				 angle = 0.0;
				 leftCounter = rightCounter = 0;
				 wishSpeedA = 0.4;
				 wishSpeedB = 0.4;
			 }
			 break;
		 // black line left - steer right
		 case 1:
			 if(rightCounter > 8)
			 {
				 angle = prevAngle;
			 }
			 else
			 {
				 leftCounter++;
				 angle = 0.5;
			 }
			 rightCounter = 0;
			 straightCounter = 0;
			 wishSpeedA = 0.4;
			 wishSpeedB = 0.3;
			 break;
		 // black line right - steer left
		 case 2:
			 if(leftCounter > 8)
			 {
				 angle = prevAngle;
			 }
			 else
			 {
				 rightCounter++;
				 angle = -0.5; 
			 }
			 leftCounter = 0;
			 straightCounter = 0;
			 wishSpeedA = 0.3;
			 wishSpeedB = 0.4;
			 break;
		 // black line centre
		 case 3:
			 straightCounter = 0;
			 if((64-rightBlackIndex)<(leftBlackIndex-64))
			 {
				 // 0.4 -> bremst zu stark, 0.3 -> etwas besser, 
				 // 0.2 -> bremsen fällt fast gar nicht mehr auf
				 TFC_SetMotorPWM(0.2, 0.2);
				 angle = -0.8;
				 wishSpeedA = 0.4;
				 wishSpeedB = -0.2;
				 leftCounter++;
				 rightCounter = 0;
			 }
			 else
			 {
				 TFC_SetMotorPWM(0.2, 0.2);
				 angle = 0.8;
				 wishSpeedA = -0.2;
				 wishSpeedB = 0.4;
				 rightCounter++;
				 leftCounter = 0;
			 }
			 break; 			 
	 }
	 steering(angle + 0.15);
	 prevAngle = angle + 0.15;
	 angle = 0.15;
	 
	 TERMINAL_PRINTF("SA %i CI %i PI %i C %i CL %i CR %i \n\r", 
			 steerAction, rightBlackIndex, leftBlackIndex, straightCounter, leftCounter, rightCounter);
 }
