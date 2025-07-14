#include "esp32-hal.h"
#include "globals.h"
#include "lora.h"
#include "SPIDeviceManager.h"
#include <SPI.h>

extern SPIClass spi_custom;

#define CS_MAX6675 12

void initMax6675() {
  pinMode(CS_MAX6675, OUTPUT);
  digitalWrite(CS_MAX6675, HIGH);
  Serial.println("MAX6675 bắt đầu");
  delay(1000);
}

float readMAX6675_SPI(SPIClass& spi, uint8_t csPin) {
  uint8_t msb, lsb;
  uint16_t v;

  digitalWrite(csPin, LOW);
  delayMicroseconds(10);

  msb = spi.transfer(0x00);
  lsb = spi.transfer(0x00);

  digitalWrite(csPin, HIGH);

  v = ((uint16_t)msb << 8) | lsb;

  if (v & 0x4) return NAN; // No thermocouple connected

  v >>= 3;
  return v * 0.25;
}

void ReadMax6675() {
  if (xSemaphoreTake(spiMutex, portMAX_DELAY) == pdTRUE) {
    selectSPIDevice(2); // SPI_MAX

    tempC = readMAX6675_SPI(spi_custom, CS_MAX6675);
    tempF = isnan(tempC) ? NAN : tempC * 9.0 / 5.0 + 32;

    if (isnan(tempC)) {
      Serial.println("⚠️ MAX6675 lỗi hoặc chưa gắn đầu dò!");
    } else {
      Serial.printf("🌡️ Nhiệt độ: %.2f°C | %.2f°F\n", tempC, tempF);
    }

    selectSPIDevice(0); // SPI_NONE
    xSemaphoreGive(spiMutex);
  }
}
