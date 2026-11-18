#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "SPIFFS.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define DS18B20_PIN 32
#define PHOTORESISTOR_PIN 34

OneWire oneWireBus(DS18B20_PIN);
DallasTemperature myTempSensor(&oneWireBus);
Adafruit_BMP280 myBarometer;

unsigned long lastReadTime = 0;
const long readingInterval = 5000;

// Унікальні UUID для сервісу та характеристик BLE
#define MY_SERVICE_UUID "a1ea81f0-0e1b-d4a1-8840-63f88c8da1ea"
#define CHAR_TEMP_UUID  "a1ea81f0-0e1b-d4a1-8840-63f88c8da1eb"
#define CHAR_PRESS_UUID "a1ea81f0-0e1b-d4a1-8840-63f88c8da1ec"
#define CHAR_ALT_UUID   "a1ea81f0-0e1b-d4a1-8840-63f88c8da1ed"
#define CHAR_LUX_UUID   "a1ea81f0-0e1b-d4a1-8840-63f88c8da1ee"

BLECharacteristic* bleTemp;
BLECharacteristic* blePress;
BLECharacteristic* bleAlt;
BLECharacteristic* bleLux;

// Запис даних у файл CSV на внутрішню пам'ять ESP32
void logDataToSPIFFS(float t, float p, float a, int l) {
    File logFile = SPIFFS.open("/data.csv", FILE_APPEND);
    if (!logFile) return;

    String csvRow = String(millis()) + "," + String(t, 2) + "," + String(p, 2) + "," + String(a, 2) + "," + String(l) + "\n";
    logFile.print(csvRow);
    logFile.close();
}

// Пошук та парсинг останнього рядка у файлі
bool fetchLatestRecord(float& t, float& p, float& a, int& l) {
    if (!SPIFFS.exists("/data.csv")) return false;

    File logFile = SPIFFS.open("/data.csv", FILE_READ);
    if (!logFile) return false;

    String finalLine;
    while (logFile.available()) {
        String currentLine = logFile.readStringUntil('\n');
        if (currentLine.length() > 0) finalLine = currentLine;
    }
    logFile.close();

    if (finalLine.length() == 0) return false;

    // Розподіл рядка на значення за комами
    int pos1 = finalLine.indexOf(',');
    int pos2 = finalLine.indexOf(',', pos1 + 1);
    int pos3 = finalLine.indexOf(',', pos2 + 1);
    int pos4 = finalLine.indexOf(',', pos3 + 1);

    t = finalLine.substring(pos1 + 1, pos2).toFloat();
    p = finalLine.substring(pos2 + 1, pos3).toFloat();
    a = finalLine.substring(pos3 + 1, pos4).toFloat();
    l = finalLine.substring(pos4 + 1).toInt();

    return true;
}

void setup() {
    Serial.begin(115200);

    if (!SPIFFS.begin(true)) return;

    myTempSensor.begin();
    if (!myBarometer.begin(0x76)) { // I2C адреса BMP280
        Serial.println("BMP280 error");
        while (1);
    }

    // Створення заголовка файлу, якщо він новий
    if (!SPIFFS.exists("/data.csv")) {
        File logFile = SPIFFS.open("/data.csv", FILE_WRITE);
        logFile.println("time_ms,temp_C,press_Pa,alt_m,lux");
        logFile.close();
    }

    // Налаштування BLE сервера та характеристик
    BLEDevice::init("BLE_Station");
    BLEServer* myServer = BLEDevice::createServer();
    BLEService* myService = myServer->createService(MY_SERVICE_UUID);

    // Додаємо Notify дескриптор (2902) для кожної характеристики
    bleTemp = myService->createCharacteristic(CHAR_TEMP_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    bleTemp->addDescriptor(new BLE2902());

    blePress = myService->createCharacteristic(CHAR_PRESS_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    blePress->addDescriptor(new BLE2902());

    bleAlt = myService->createCharacteristic(CHAR_ALT_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    bleAlt->addDescriptor(new BLE2902());

    bleLux = myService->createCharacteristic(CHAR_LUX_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    bleLux->addDescriptor(new BLE2902());

    myService->start();
    BLEAdvertising* myAd = BLEDevice::getAdvertising();
    myAd->addServiceUUID(MY_SERVICE_UUID);
    myAd->start();
}

void loop() {
    unsigned long currentTime = millis();

    // Робота за таймером без блокування (BlinkWithoutDelay style)
    if (currentTime - lastReadTime >= readingInterval) {
        lastReadTime = currentTime;

        // Збір даних з сенсорів
        myTempSensor.requestTemperatures();
        float currentTemp = myTempSensor.getTempCByIndex(0);
        float currentPress = myBarometer.readPressure();
        float currentAlt = myBarometer.readAltitude(1013.25);
        int currentLux = analogRead(PHOTORESISTOR_PIN);

        Serial.printf("T: %.2f C | P: %.2f Pa | Alt: %.2f m | Light: %d\n", currentTemp, currentPress, currentAlt, currentLux);

        logDataToSPIFFS(currentTemp, currentPress, currentAlt, currentLux);

        // Оновлення значень BLE, вичитуючи їх безпосередньо з файлу
        float savedTemp, savedPress, savedAlt;
        int savedLux;

        if (fetchLatestRecord(savedTemp, savedPress, savedAlt, savedLux)) {
            bleTemp->setValue(String(savedTemp, 2).c_str());
            bleTemp->notify();

            blePress->setValue(String(savedPress, 2).c_str());
            blePress->notify();

            bleAlt->setValue(String(savedAlt, 2).c_str());
            bleAlt->notify();

            bleLux->setValue(String(savedLux).c_str());
            bleLux->notify();
        }
    }
}
