const int photoresistorPin = 34;

hw_timer_t *timer1 = NULL;
hw_timer_t *timer2 = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile bool triggerTimer1 = false;
volatile bool triggerTimer2 = false;

// ISR для Таймера 1
void IRAM_ATTR onTimer1() {
  portENTER_CRITICAL_ISR(&timerMux);
  triggerTimer1 = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

// ISR для Таймера 2
void IRAM_ATTR onTimer2() {
  portENTER_CRITICAL_ISR(&timerMux);
  triggerTimer2 = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  // Таймер 1: кожні 2 сек (2 000 000 мкс)
  timer1 = timerBegin(1000000);
  timerAttachInterrupt(timer1, &onTimer1);
  timerAlarm(timer1, 2000000, true, 0);

  // Таймер 2: кожні 5 сек (5 000 000 мкс)
  timer2 = timerBegin(1000000);
  timerAttachInterrupt(timer2, &onTimer2);
  timerAlarm(timer2, 5000000, true, 0);
}

void loop() {
  if (triggerTimer1) {
    portENTER_CRITICAL(&timerMux);
    triggerTimer1 = false;
    portEXIT_CRITICAL(&timerMux);

    Serial.printf("Т1 (2с): ADC %d\n", analogRead(photoresistorPin));
  }

  if (triggerTimer2) {
    portENTER_CRITICAL(&timerMux);
    triggerTimer2 = false;
    portEXIT_CRITICAL(&timerMux);

    Serial.printf("Т2 (5с): ADC %d\n", analogRead(photoresistorPin));
  }
}
