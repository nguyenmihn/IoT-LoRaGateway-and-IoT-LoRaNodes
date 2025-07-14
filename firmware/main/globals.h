#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// Semaphore để bảo vệ SPI dùng chung (LoRa, MAX6675, v.v.)
extern SemaphoreHandle_t spiMutex;
extern volatile float Ambient;
extern volatile float Object;
extern volatile int16_t adc1;
extern volatile int16_t adc2;
extern volatile float tempC;
extern volatile float tempF;

#ifdef __cplusplus
}
#endif

#endif // GLOBALS_H
