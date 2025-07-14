#include <Arduino.h>
#include <LoRa.h>
#include "lora.h"
#include <SPI.h>
#include "adc_reader.h"
#include "globals.h"
#include "SPIDeviceManager.h"
#include "MLX90614.h"
#include "MaxRead6675.h"
#include "WiFi.h"

#define PIN_MISO 18
#define PIN_MOSI 19
#define PIN_SCK  4
#define PIN_SS   21
#define PIN_RST  22
#define PIN_DIO0 23
#define LO_RA_EN_PIN 2

SPIClass spi_custom(HSPI);
String mac;

void initLoRa() {
  pinMode(LO_RA_EN_PIN, OUTPUT);
  pinMode(PIN_SS, OUTPUT);
  digitalWrite(LO_RA_EN_PIN, HIGH);
  digitalWrite(PIN_SS, HIGH);  // CS mặc định ở HIGH

  spi_custom.begin(PIN_SCK, PIN_MISO, PIN_MOSI, -1); // CS controlled manually

  LoRa.setSPI(spi_custom);
  LoRa.setPins(PIN_SS, PIN_RST, PIN_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("❌ LoRa khởi tạo thất bại!");
    while (1);
  }

  Serial.println("✅ LoRa đã sẵn sàng!");
  mac = WiFi.macAddress();  // Lấy địa chỉ MAC ESP32, ví dụ "24:6F:28:AA:BB:CC"
  Serial.println("Địa chỉ MAC:");
  Serial.print(mac);
}

void sendLoRaPacket() {
  if (xSemaphoreTake(spiMutex, portMAX_DELAY) == pdTRUE) {
    selectSPIDevice(SPI_LORA);  // rõ ràng hơn so với số 0

    LoRa.beginPacket();
    mac.replace(":", "");  // Bỏ dấu ":" để dễ xử lý
    LoRa.print("ID");  // prefix giúp bên nhận biết là ID
    LoRa.print(mac);
    LoRa.println(":");  // tạo định dạng ID:xxx
    LoRa.println("Bắt đầu gửi dữ liệu");
    LoRa.print("Cảm biến lửa = ");
    LoRa.println(adc1);
    LoRa.print("Cảm biến ga = ");
    LoRa.println(adc2);
    LoRa.print("Ambient_MLX90614 = ");
    LoRa.println(Ambient);
    LoRa.print("Object_MLX90614 = ");
    LoRa.println(Object);
    LoRa.print("Object_MAX6675 = ");
    LoRa.println(tempC);
    LoRa.println("Kết thúc gửi dữ liệu");
    LoRa.endPacket();

    selectSPIDevice(SPI_NONE);  // bỏ chọn sau khi gửi xong
    xSemaphoreGive(spiMutex);
  }

  Serial.println("📤 Đã gửi gói LoRa");
}

// Chỉ dùng nếu bạn cần "reset" SPI
// void suspendLoRaSPI() {
//   spi_custom.end();
//   delay(5);
// }

// void resumeLoRaSPI() {
//   spi_custom.begin(PIN_SCK, PIN_MISO, PIN_MOSI, -1);
//   LoRa.setPins(PIN_SS, PIN_RST, PIN_DIO0);
//   LoRa.begin(433E6);  // Cân nhắc kỹ: nên hạn chế gọi lại nếu đang hoạt động ổn
// }
