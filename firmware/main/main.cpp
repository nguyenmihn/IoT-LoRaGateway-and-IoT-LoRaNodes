#include "freertos/projdefs.h"
#include "main.h"
#include "adc_reader.h"
#include "lora.h"
#include <LoRa.h>

#include <Arduino.h>
#include "MLX90614.h"
#include "MaxRead6675.h"
#include "globals.h"

TaskHandle_t taskAdcHandle;
TaskHandle_t taskLoraHandle;
TaskHandle_t taskMLXHandle;
TaskHandle_t taskMax6675Handle;

void taskAdc(void *pvParameters) {
  while(1){
    readAdcValue();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void taskLora(void* pvParameters) {
  while (1) {
    sendLoRaPacket();
    vTaskDelay(pdMS_TO_TICKS(500)); // gửi mỗi 2 giây
  }
}

void taskMLX(void *pvParameters){
  while(1){
    ReadMLX96014();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void taskMax6675(void* pvParameters) {
  while (1) {
    ReadMax6675();
    vTaskDelay(pdMS_TO_TICKS(500)); // đọc mỗi 3 giây
  }
}
void SetupSystem(){
  spiMutex = xSemaphoreCreateMutex();
  initMax6675();
  initADC();
  initLoRa();
  initMLX90614();
  delay(100);
  xTaskCreatePinnedToCore(taskAdc, "ADC Task", 2048, NULL, 1, &taskAdcHandle, 1);
  xTaskCreatePinnedToCore(taskMax6675, "Max6675 Task", 4096, NULL, 1, &taskMax6675Handle, 1);
  xTaskCreatePinnedToCore(taskLora, "LoRa Task", 4096, NULL, 1, &taskLoraHandle, 0);
  xTaskCreatePinnedToCore(taskMLX, "MLX96014 Task", 4096, NULL, 1, &taskMLXHandle, 0);
  
}