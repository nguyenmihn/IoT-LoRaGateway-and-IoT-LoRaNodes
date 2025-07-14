#include "globals.h"

SemaphoreHandle_t spiMutex;

volatile float Ambient = 0.0;
volatile float Object = 0.0;
volatile int16_t adc1 = 0;
volatile int16_t adc2 = 0;
volatile float tempC = 0.0;
volatile float tempF = 0.0;
