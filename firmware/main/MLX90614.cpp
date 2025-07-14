#include <Arduino.h>
#include <Adafruit_MLX90614.h>
#include "globals.h"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void initMLX90614() {
  Serial.println("Khởi tạo cảm biến MLX96014");
  if (!mlx.begin()) {
    Serial.println("MLX96014 Không khởi tạo thành công");
    while (1);
  };

  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");
}

void ReadMLX96014() {
  Ambient = mlx.readAmbientTempC();
  Object = mlx.readObjectTempC();

  Serial.print("Ambient = "); Serial.print(Ambient);
  Serial.print("*C\tObject = "); Serial.print(Object); Serial.println("*C");
  
  // Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF());
  // Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");

  Serial.println();
}