#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_CS1 5
#define BMP_CS2 17

Adafruit_BMP280 bmp1(BMP_CS1);
Adafruit_BMP280 bmp2(BMP_CS2);

void setup() {
  Serial.begin(115200);
  Serial.println("Тест двох BMP280 через SPI");

  // Ініціалізація першого
  if (!bmp1.begin()) {
    Serial.println("Датчик 1 не знайдено на піні CS 5");
  }

  // Ініціалізація другого
  if (!bmp2.begin()) {
    Serial.println("Датчик 2 не знайдено на піні CS 17");
  }
}

void loop() {
  // Читаємо дані з першого
  Serial.print("BMP1: ");
  Serial.print(bmp1.readTemperature());
  Serial.print(" *C, ");
  Serial.print(bmp1.readPressure() * 0.00750062);
  Serial.println(" mmHg");

  // Читаємо дані з другого
  Serial.print("BMP2: ");
  Serial.print(bmp2.readTemperature());
  Serial.print(" *C, ");
  Serial.print(bmp2.readPressure() * 0.00750062);
  Serial.println(" mmHg");

  Serial.println("-------------------------");
  delay(2000);
}
