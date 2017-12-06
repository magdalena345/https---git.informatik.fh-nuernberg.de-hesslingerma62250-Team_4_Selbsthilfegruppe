#ifndef CAMERA

#define CAMERA

#define COUNTPICTURE 4
#define BLACKRANGEPERCENT 1.10
#define MINPARTSTEERACTIONTHREE 38
#define MAXPARTSTEERACTIONTHREE 90
#define STARTARRAY 16
#define ENDARRAY 112

int16_t rightBlackIndex;
int16_t rightPrevBlackIndex;
int16_t leftBlackIndex;
int16_t leftPrevBlackIndex;
uint16_t minLightValue;
uint16_t maxLightValue;
volatile uint16_t lightThreshold;
uint16_t blackCounter;

volatile short steerAction;

volatile char dynamicLightCalculation;
uint16_t currentShotCount;
uint16_t picturesShot[COUNTPICTURE][128];

void setupCamera();
void calibrateCamera();
void calculateLightTresholdStatic();
void calculateLightTresholdDynamic();

#endif
