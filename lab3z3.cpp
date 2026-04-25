#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

const int btnPin = 18;
const int led1Pin = 16; // Діод для таймера (BMP280)
const int led2Pin = 17; // Діод для кнопки

Adafruit_BMP280 bmp; // I2C об'єкт

hw_timer_t *timer = NULL;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// Прапорці та змінні для переривань
volatile bool readBmpFlag = false;
volatile uint32_t pressTime = 0;
volatile uint32_t pressDuration = 0;
volatile bool btnReleasedFlag = false;

// ISR Таймера: встановлює прапорець для зчитування BMP280
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&mux);
  readBmpFlag = true;
  portEXIT_CRITICAL_ISR(&mux);
}

// ISR Кнопки: рахує час натискання (режим CHANGE)
void IRAM_ATTR onButton() {
  uint32_t t = millis();
  bool isPressed = !digitalRead(btnPin); // LOW = натиснуто (PULLUP)
  
  portENTER_CRITICAL_ISR(&mux);
  if (isPressed) {
    pressTime = t; // Фіксуємо старт
  } else if (pressTime > 0) {
    pressDuration = t - pressTime; // Рахуємо різницю при відпусканні
    btnReleasedFlag = true;
    pressTime = 0;
  }
  portEXIT_CRITICAL_ISR(&mux);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  
  // Ініціалізація BMP280 (I2C адреса зазвичай 0x76 або 0x77)
  if (!bmp.begin(0x76)) {
    Serial.println("Помилка: BMP280 не знайдено!");
  }
  
  // Таймер на 3 секунди (3 000 000 мкс)
  timer = timerBegin(1000000); 
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 3000000, true, 0); 
  
  // Переривання спрацьовує на ЗМІНУ стану (натискання + відпускання)
  attachInterrupt(btnPin, onButton, CHANGE);
}

void loop() {
  // --- Завдання 1: Таймер + BMP280 ---
  if (readBmpFlag) {
    portENTER_CRITICAL(&mux);
    readBmpFlag = false;
    portEXIT_CRITICAL(&mux);

    float temp = bmp.readTemperature();
    float pres = bmp.readPressure() / 100.0F; // в гектопаскалях
    Serial.printf("[Таймер] Темп: %.2f °C, Тиск: %.2f hPa\n", temp, pres);

    // Мигання діодом після вичитки
    digitalWrite(led1Pin, HIGH);
    delay(100); 
    digitalWrite(led1Pin, LOW);
  }

  // --- Завдання 2: Кнопка + Діод ---
  if (btnReleasedFlag) {
    portENTER_CRITICAL(&mux);
    uint32_t duration = pressDuration;
    btnReleasedFlag = false;
    portEXIT_CRITICAL(&mux);

    Serial.printf("[Кнопка] Затиснута на: %u мс\n", duration);

    // Світимо другим діодом точно такий самий час
    digitalWrite(led2Pin, HIGH);
    delay(duration);
    digitalWrite(led2Pin, LOW);
  }
}
