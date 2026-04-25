// Оголошуємо структуру кнопки
struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

// Ініціалізація кнопок
Button button1 = {18, 0, false};
Button button2 = {19, 0, false};

// Піни
const int photoresistorPin = 34;
const int led1Pin = 21;
const int led2Pin = 22;

// Налаштування ШІМ
const int pwmFreq = 5000;
const int pwmResolution = 8;

// Обробник 1-го переривання
void IRAM_ATTR isr1() {
  static uint32_t lastInterruptTime = 0;
  uint32_t interruptTime = millis();
  if (interruptTime - lastInterruptTime > 200) {
    button1.pressed = true;
    lastInterruptTime = interruptTime;
  }
}

// Обробник 2-го переривання
void IRAM_ATTR isr2() {
  static uint32_t lastInterruptTime = 0;
  uint32_t interruptTime = millis();
  if (interruptTime - lastInterruptTime > 200) {
    button2.numberKeyPresses += 1;
    button2.pressed = true;
    lastInterruptTime = interruptTime;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(button1.PIN, INPUT_PULLUP);
  pinMode(button2.PIN, INPUT_PULLUP);
  pinMode(led1Pin, OUTPUT);
  digitalWrite(led1Pin, LOW);

  // ШІМ
  ledcAttach(led2Pin, pwmFreq, pwmResolution);
  ledcWrite(led2Pin, 0);
  attachInterrupt(button1.PIN, isr1, RISING);
  attachInterrupt(button2.PIN, isr2, FALLING);
  Serial.println("Систему запущено. Очікування натискань...");
}

void loop() {
  // Фоторезистор та мигання
  if (button1.pressed) {
    button1.pressed = false;
    int lightValue = analogRead(photoresistorPin);
    Serial.printf("Кнопку 1 відпущено. Значення фоторезистора: %d\n", lightValue);
    digitalWrite(led1Pin, HIGH);
    delay(300);
    digitalWrite(led1Pin, LOW);
  }

  // Лічильник, Увімк/Вимк, Яскравість
  if (button2.pressed) {
    button2.pressed = false;
    uint32_t presses = button2.numberKeyPresses;
    Serial.printf("Кнопка 2 натиснута %u разів\n", presses);
    bool isLedOn = (presses % 2 != 0);
    
    if (isLedOn) {
      int brightness = (presses * 15) % 256;
      if (brightness < 20) brightness = 20;
      
      // Використовуємо led2Pin замість pwmChannel
      ledcWrite(led2Pin, brightness);
      Serial.printf("  -> Діод 2 УВІМКНЕНО. Яскравість ШІМ: %d\n", brightness);
    } else {
      // Використовуємо led2Pin замість pwmChannel
      ledcWrite(led2Pin, 0);
      Serial.println("  -> Діод 2 ВИМКНЕНО");
    }
  }
}
