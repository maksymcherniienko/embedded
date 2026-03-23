#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Конфігурація пінів
const int LDR_PIN = 4;        // Фоторезистор (Аналоговий)
const int ONE_WIRE_BUS = 5;    // DS18B20 (1-Wire) - змінено на 5 для зручності

// Об'єкти для датчиків
Adafruit_BMP280 bmp;                          // I2C: SDA=21, SCL=22
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dsSensors(&oneWire);

void setup() {
  Serial.begin(115200);
  Serial.println("--- Система моніторингу запущена ---");

  // 1. Ініціалізація BMP280 (I2C)
  if (!bmp.begin(0x76)) { // Спробуйте 0x77, якщо не знаходить
    Serial.println("Помилка: BMP280 не знайдено!");
  }

  // 2. Ініціалізація DS18B20 (1-Wire)
  dsSensors.begin();
  
  // 3. Налаштування піна фоторезистора
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  Serial.println("\n--- Нове зчитування ---");

  // --- Блок DS18B20 (1-Wire) ---
  dsSensors.requestTemperatures();
  float tempDS = dsSensors.getTempCByIndex(0);
  Serial.print("DS18B20 Температура: ");
  Serial.print(tempDS);
  Serial.println(" *C");

  // --- Блок BMP280 (I2C) ---
  float tempBMP = bmp.readTemperature();
  float pressure = bmp.readPressure() * 0.00750062; // Перевід у мм рт. ст.
  Serial.print("BMP280 Температура: ");
  Serial.print(tempBMP);
  Serial.print(" *C | Тиск: ");
  Serial.print(pressure);
  Serial.println(" мм рт. ст.");

  // --- Блок Фоторезистора (Analog) ---
  int lightRaw = analogRead(LDR_PIN);
  Serial.print("Освітленість (Raw): ");
  Serial.println(lightRaw);

  Serial.println("-------------------------");
  delay(2000);
}
