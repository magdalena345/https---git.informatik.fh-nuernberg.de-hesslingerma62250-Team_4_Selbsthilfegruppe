#ifndef OHMRACER_H
#define OHMRACER_H

#include "derivative.h" 
#include "TFC/TFC.h"
#include "Camera.h"
#include "Motor.h"
#include "I2C.h"
#include "Accelerometer.h"

#define OHMRACER_DEBUG
#define GETTICKER(x) ((x >= NUM_TFC_TICKERS) ? (TFC_Ticker[NUM_TFC_TICKERS - 1]) : (TFC_Ticker[x+4]))
#define SETTICKER(x,y) ((x >= NUM_TFC_TICKERS) ? (TFC_Ticker[NUM_TFC_TICKERS - 1] = y) : (TFC_Ticker[x+4] = y))
#define WAITING(x) TFC_Ticker[3] = 0; do{}while(TFC_Ticker[3] <= x)

#define MINEXPOSURETIME 1000
#define MAXEXPOSURETIME 50000

#endif
