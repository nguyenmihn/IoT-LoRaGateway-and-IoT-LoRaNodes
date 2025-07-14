#include <Arduino.h>

// Định nghĩa rõ tên từng thiết bị
#define SPI_NONE  -1
#define SPI_LORA   0
#define SPI_ETH    1
#define SPI_MAX    2

#define CS_LORA    21
#define CS_ETH     15
#define CS_MAX     12

void selectSPIDevice(int device) {
  // Tắt tất cả thiết bị trước
  digitalWrite(CS_LORA, HIGH);
  digitalWrite(CS_ETH, HIGH);
  digitalWrite(CS_MAX, HIGH);

  // Bật đúng thiết bị được chọn
  switch (device) {
    case SPI_LORA: digitalWrite(CS_LORA, LOW); break;
    case SPI_ETH:  digitalWrite(CS_ETH, LOW);  break;
    case SPI_MAX:  digitalWrite(CS_MAX, LOW);  break;
    default: break; // SPI_NONE hoặc sai mã → không chọn gì
  }
}
