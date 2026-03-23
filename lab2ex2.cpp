#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp1; 
Adafruit_BMP280 bmp2;

void setup() {
  Serial.begin(115200);
  Serial.println("Пошук двох датчиків BMP280...");

  // Ініціалізація першого датчика (адреса за замовчуванням 0x76)
  if (!bmp1.begin(0x76)) {
    Serial.println("Датчик #1 (0x76) не знайдено!");
  }

  // Ініціалізація другого датчика (адреса 0x77)
  if (!bmp2.begin(0x77)) {
    Serial.println("Датчик #2 (0x77) не знайдено!");
  }
}

void loop() {
  // Зчитування з першого датчика
  Serial.print("Датчик 1: T=");
  Serial.print(bmp1.readTemperature());
  Serial.print(" C, P=");
  Serial.print(bmp1.readPressure() * 0.00750062); // в мм рт. ст.
  Serial.println(" mmHg");

  // Зчитування з другого датчика
  Serial.print("Датчик 2: T=");
  Serial.print(bmp2.readTemperature());
  Serial.print(" C, P=");
  Serial.print(bmp2.readPressure() * 0.00750062);
  Serial.println(" mmHg");

  Serial.println("------------------------------------");
  delay(2000);
}
