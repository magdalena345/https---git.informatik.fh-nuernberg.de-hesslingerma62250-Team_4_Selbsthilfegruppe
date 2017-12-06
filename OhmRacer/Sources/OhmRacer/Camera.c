#include "OhmRacer/OhmRacer.h"

void setupCamera()
{
	leftBlackIndex = -2;
	lightThreshold = 1500; //vorher 1500
	minLightValue = 1500; //vorher 1500
	maxLightValue = 0;
	currentShotCount = 0;
}

void calibrateCamera()
{
	while(!TFC_PUSH_BUTTON_1_PRESSED)
	{
		TFC_Task();
		
		if(TFC_Ticker[0]>=20)
		{
			TFC_Ticker[0] = 0; //reset the Ticker
		}
		
		if(GETTICKER(0) > 50 && LineScanImageReady == 1)
		{
			SETTICKER(0,0);
			calculateLightTresholdDynamic();
			LineScanImageReady = 0;
			calculateSteering();
		}		
	}
	configCar();
}

void calculateLightTresholdStatic()
{
	int16_t i;
	
	for(i = 0; i < 128; i++)
	{
		picturesShot[currentShotCount][i] = LineScanImage0[i];
	}
	
	TFC_SetBatteryLED_Level(currentShotCount + 1);
	currentShotCount++;
	
	if(currentShotCount == COUNTPICTURE)
	{
		int16_t minLightValue;
		int16_t y;
		int16_t tmpMinLightValue = 0;

		for(i = 0; i < COUNTPICTURE; i++)
		{
			minLightValue = 5000;		
			
			for(y = STARTARRAY; y < ENDARRAY; y++)
			{
				if(picturesShot[i][y] < minLightValue)
					minLightValue = picturesShot[i][y];
			}
			tmpMinLightValue += minLightValue;
		}
		
		tmpMinLightValue = tmpMinLightValue / COUNTPICTURE;
		
		lightThreshold = tmpMinLightValue * BLACKRANGEPERCENT;
	}
}
void calculateLightTresholdDynamic()
{
	char rightTmpBlackIndex;
	char leftTmpBlackIndex;
	
	int16_t i;
	
	rightPrevBlackIndex = rightBlackIndex;
	leftPrevBlackIndex = leftBlackIndex;
	rightBlackIndex = -1;
	leftBlackIndex = -1;
	blackCounter = 0;
	minLightValue = 5000;
	maxLightValue = 0;
	
	for(i = STARTARRAY; i < ENDARRAY; i++)
	{	
		if(dynamicLightCalculation == TRUE)
		{		
			if(LineScanImage0[i] < minLightValue)
				minLightValue = LineScanImage0[i];
			if(LineScanImage0[i] > maxLightValue)
				maxLightValue = LineScanImage0[i];
		}
		
		if(LineScanImage0[i] >= lightThreshold)
		{
			TERMINAL_PRINTF("_");
		}
		else 
		{
			TERMINAL_PRINTF("I");
		}
		
		// Black
		if(LineScanImage0[i] >= lightThreshold || i == 111)
		{
			if(blackCounter > 1)
			{
				rightTmpBlackIndex = i - 1;
				leftTmpBlackIndex = rightTmpBlackIndex - blackCounter;
					
				rightBlackIndex = rightTmpBlackIndex;
				leftBlackIndex = leftTmpBlackIndex;
			}
			blackCounter = 0;
		}
		else
		{
			++blackCounter;
		}
	}
	
	for(i = STARTARRAY; i < ENDARRAY; i++)
	{
			TERMINAL_PRINTF("Start:");
			TERMINAL_PRINTF(LineScanImage0[i] + " ");
			TERMINAL_PRINTF("Ende");
	}
	
	TERMINAL_PRINTF("\n\r");

	if(dynamicLightCalculation == TRUE)
	{
		float diff = maxLightValue - minLightValue;
			diff *= 0.30f;
			lightThreshold = minLightValue + diff;
			if(maxLightValue - lightThreshold < 1000 ){
				lightThreshold = minLightValue - 10;
			}
	}
	
	if(leftBlackIndex == -2)
	{
		leftBlackIndex = rightBlackIndex;
		return;
	}
	
	if(rightBlackIndex == -1)
	{
		steerAction = 0;
	}
	else
	{
		if(rightBlackIndex >= STARTARRAY && rightBlackIndex < MINPARTSTEERACTIONTHREE)
		{
			// black line left - steer right
			steerAction = 1;
		}
		else if(leftBlackIndex > MAXPARTSTEERACTIONTHREE && rightBlackIndex < ENDARRAY)
		{
			// black line right - steer left
			steerAction = 2;
		}
		else if((rightBlackIndex >= MINPARTSTEERACTIONTHREE && rightBlackIndex <= MAXPARTSTEERACTIONTHREE) 
				|| (leftBlackIndex >= MINPARTSTEERACTIONTHREE && leftBlackIndex <= MAXPARTSTEERACTIONTHREE))
		{
			// black line centre
			steerAction = 3;
		}
		else
		{
			steerAction = -1;
		}
	}
}
