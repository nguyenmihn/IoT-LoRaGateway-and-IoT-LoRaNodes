#include <Arduino.h> 
#include "adc_reader.h"
#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include "globals.h"

Adafruit_ADS1115 ads;

void initADC() {
  Wire.begin(17, 14);
  if(!ads.begin()) {
    Serial.print("không tìm thấy ADS1115");
    // while(1);
  }
  ads.setGain(GAIN_TWO);

  Serial.println("ADS1115 đã sẵn sàng");
}

void readAdcValue() {
  adc1 = ads.readADC_SingleEnded(0);
  adc2 = ads.readADC_SingleEnded(1);
  Serial.printf("Cảm biến lửa: %d, Cảm biến khí ga: %d\n", adc1, adc2);
}