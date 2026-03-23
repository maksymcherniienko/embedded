#include <SPI.h>
#include <Adafruit_BMP280.h>

// Визначаємо піни CS (Chip Select)
#define BMP_CS1 5
#define BMP_CS2 17

// Створюємо два об'єкти для кожного датчика через SPI
Adafruit_BMP280 bmp1(BMP_CS1);
Adafruit_BMP280 bmp2(BMP_CS2);

void setup() {
  Serial.begin(115200);
  Serial.println("Тестування двох BMP280 через SPI...");

  // Ініціалізація 1-го датчика
  if (!bmp1.begin()) {
    Serial.println("Датчик #1 не знайдено! Перевірте пін CS 5");
    while (1);
  }

  // Ініціалізація 2-го датчика
  if (!bmp2.begin()) {
    Serial.println("Датчик #2 не знайдено! Перевірте пін CS 17");
    while (1);
  }

  Serial.println("Обидва датчики готові!");
}

void loop() {
  // Датчик 1
  Serial.print("Датчик 1: T=");
  Serial.print(bmp1.readTemperature());
  Serial.print("C, P=");
  Serial.print(bmp1.readPressure() * 0.00750062); // в мм рт. ст.
  Serial.println(" mmHg");

  // Датчик 2
  Serial.print("Датчик 2: T=");
  Serial.print(bmp2.readTemperature());
  Serial.print("C, P=");
  Serial.print(bmp2.readPressure() * 0.00750062);
  Serial.println(" mmHg");

  Serial.println("------------------------------------");
  delay(2000);
}
