#ifndef LORA_H
#define LORA_H

#include <Arduino.h>
#include <SPI.h>                // 🔧 Cần include để biết SPIClass

extern SPIClass spi_custom;     // 💥 Quan trọng: Cho phép truy cập SPI driver từ ngoài

void initLoRa();
void sendLoRaPacket();
void suspendLoRaSPI();          // Gọi từ file MaxRead6675.cpp
void resumeLoRaSPI();
void taskLora(void* pvParameters);  // Nếu có task LoRa riêng

#endif
