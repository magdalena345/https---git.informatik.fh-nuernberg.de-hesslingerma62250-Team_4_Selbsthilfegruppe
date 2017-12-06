#include "OhmRacer/OhmRacer.h"

int isInConfig;
int isDriving;
int buttonAPressed;
typedef int bool;
enum { false, true };

void init()
{
	TFC_Init();
	initMotor();
	setupCamera();
						
}
void drive()
{
	isDriving = TRUE;
	calibrateServo();
	WAITING(550);
	startMotor();

	while(!TFC_PUSH_BUTTON_1_PRESSED)
	{
		TFC_Task();
		if(LineScanImageReady == 1)
		{
			SETTICKER(0,0);
			calculateLightTresholdDynamic();
			LineScanImageReady = 0;
			calculateSteering();
			
			if(rightBlackIndex > 0 && leftBlackIndex > 0 && leftBlackIndex > STARTARRAY && rightBlackIndex < ENDARRAY-2)
			{
				
				stopMotor();
			}
		} 
		
		
		if(GETTICKER(1) > 25)
		{
			SETTICKER(1, 0);
			setSpeed();
		}
	}
	TERMINAL_PRINTF("STOP Hammerzeit");	
	stopMotor();
	steering(0.0);
	configCar();			
}

void lightValueOutput()
{
	while(!TFC_PUSH_BUTTON_1_PRESSED)
	{
		TFC_Task();
		
		// Abblendzeit der Kamera einstellen (bei hoher Beleuchtung)
		TFC_SetLineScanExposureTime(MINEXPOSURETIME + (((TFC_ReadPot(1)+ 1) / 2) * (MAXEXPOSURETIME - MINEXPOSURETIME)));
		
		if(LineScanImageReady)
		{
			LineScanImageReady = 0;
			uint16_t i, minLightValue, maxLightValue;
			minLightValue = 5000;
			maxLightValue = 0;
			
			for(i = STARTARRAY; i < ENDARRAY; i++)
			{
				if(LineScanImage0[i] < minLightValue)
					minLightValue = LineScanImage0[i];
				if(LineScanImage0[i] > maxLightValue)
					maxLightValue = LineScanImage0[i];
			}
			TERMINAL_PRINTF("min: %i, max: %i \n\r", minLightValue, maxLightValue);			
		}
	}
	configCar();
}
void configCar()
{
	isInConfig = TRUE;

	while(isInConfig == TRUE)
	{
		TFC_Task();
		
		switch(TFC_GetDIP_Switch() & 0x07)
		{
			case 1:
				TERMINAL_PRINTF("CASE 1");
				dynamicLightCalculation = FALSE;
				
				if(TFC_PUSH_BUTTON_0_PRESSED)
				{
					isInConfig = FALSE;
					drive();
				}
				break;
				
			case 2:
				TERMINAL_PRINTF("CASE 2");
				dynamicLightCalculation = TRUE;
				
				if(TFC_PUSH_BUTTON_0_PRESSED)
				{
					isInConfig = FALSE;
					calibrateCamera();
				}
				break;
				
			case 3:
				TERMINAL_PRINTF("CASE 3");
				dynamicLightCalculation = FALSE;
	
				if(TFC_PUSH_BUTTON_0_PRESSED && LineScanImageReady == 1 && buttonAPressed == FALSE)
				{
					buttonAPressed = TRUE;
					calculateLightTresholdStatic();
				}
				else if(TFC_PUSH_BUTTON_1_PRESSED && LineScanImageReady == 1 && buttonAPressed == TRUE)
				{
					buttonAPressed = FALSE;
					calculateLightTresholdStatic();
				}
				break;
			
			case 5:
				TERMINAL_PRINTF("CASE 5");
				dynamicLightCalculation = FALSE;
							
				if(TFC_PUSH_BUTTON_0_PRESSED)
				{
					isInConfig = FALSE;
					calibrateCamera();
				}
				break;
				
			case 6:
				TERMINAL_PRINTF("CASE 6");
				dynamicLightCalculation = TRUE;
				
				if(TFC_PUSH_BUTTON_0_PRESSED)
				{
					isInConfig = FALSE;
					lightValueOutput();
				}
				break;
				
			case 7:
				TERMINAL_PRINTF("CASE 7");
				dynamicLightCalculation = FALSE;
				currentShotCount = 0;				
				break;
				
				
			default:
				TERMINAL_PRINTF("CASE 0");
				dynamicLightCalculation = TRUE;
				
				if(TFC_PUSH_BUTTON_0_PRESSED)
				{
					isInConfig = FALSE;
					
					drive();
				}
				break;
		}
	}
}
void main()
  {
	init();
	configCar();
 }
