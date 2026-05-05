#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "SPIFFS.h"

// --- Налаштування пінів ---
#define ONE_WIRE_BUS 4
#define LDR_PIN 34

// --- Ініціалізація сенсорів ---
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Adafruit_BMP280 bmp;

// --- Налаштування таймера ---
unsigned long previousMillis = 0;
const long interval = 5000;

// Назва файлу
const char* filename = "/data.csv"; 

void logData(unsigned long timestamp) {
  sensors.requestTemperatures();
  float tempDS = sensors.getTempCByIndex(0);
  float tempBMP = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;
  int lightLevel = analogRead(LDR_PIN);

  String dataString = String(timestamp) + "," + String(tempDS, 2) + "," + 
                      String(tempBMP, 2) + "," + String(pressure, 2) + "," + 
                      String(lightLevel) + "\r\n";

  // Відкриття файлу в режимі "a" 
  File file = SPIFFS.open(filename, "a");
  if (file) {
    file.print(dataString);
    file.close();
    Serial.print("Записано: " + dataString);
  } else {
    Serial.println("Failed to open file");
  }
}

void printFileContent() {
  Serial.println("\n--- Читання файлу ---");
  
  // Відкриття файлу в режимі "r" 
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }

  Serial.print("Розмір файлу: ");
  Serial.println(file.size());
  
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
  Serial.println("Test complete");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Ініціалізація сенсорів
  sensors.begin();
  if (!bmp.begin(0x76)) {
    Serial.println("Помилка: BMP280 не знайдено!");
  }

  // Ініціалізація та монтування SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Error mounting SPIFFS");
    return;
  }
  Serial.println("SPIFFS змонтовано успішно.");

  // Створення файлу з режимом "w" 
  File file = SPIFFS.open(filename, "w");
  if (file) {
    file.print("Timestamp(ms),Temp_DS(C),Temp_BMP(C),Press(hPa),Light\r\n");
    file.close();
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Збір даних по таймеру
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    logData(currentMillis);
  }

  // Перевірка команд із Serial для читання файлу
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "read") {
      printFileContent();
    }
  }
}
