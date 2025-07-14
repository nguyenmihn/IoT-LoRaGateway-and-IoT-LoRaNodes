#ifndef SPI_DEVICE_MANAGER_H
#define SPI_DEVICE_MANAGER_H

#pragma once
#include <Arduino.h>

// Định nghĩa ID thiết bị SPI
#define SPI_NONE  -1
#define SPI_LORA   0
#define SPI_ETH    1
#define SPI_MAX    2

// Định nghĩa các chân CS tương ứng
#define CS_LORA    21
#define CS_ETH     15
#define CS_MAX     12

// Hàm điều khiển thiết bị SPI
void selectSPIDevice(int device);

#endif
