#include <OneWire.h>
#include <DallasTemperature.h>

const int ONE_WIRE_BUS = 4;

// Налаштовуємо oneWire для роботи з обраним піном
OneWire oneWire(ONE_WIRE_BUS);

// Передаємо посилання на oneWire бібліотеці DallasTemperature
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin(); // Ініціалізація шини

  // Перевіряємо, скільки датчиків знайшлося на шині
  int deviceCount = sensors.getDeviceCount();
  Serial.print("Знайдено датчиків: ");
  Serial.println(deviceCount);

}

void loop() {
  // Команда всім датчикам на шині виміряти температуру
  sensors.requestTemperatures();

  // Виводимо температуру кожного з 3-х датчиків за індексом
  for (int i = 0; i < 3; i++) {
    float tempC = sensors.getTempCByIndex(i);
    Serial.print("Датчик ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(tempC);
    Serial.println(" °C");
  }
  Serial.println("-------------------------");

  delay(1000);

}
