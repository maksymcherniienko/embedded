const int ldrPin = 4;    // Пін фоторезистора (вхід)
const int dacPin = 25;   // Пін ЦАП (вихід на світлодіод)

void setup() {
  Serial.begin(115200);
}

void loop() {
  // 1. Зчитуємо освітленість (0-4095)
  int lightLevel = analogRead(ldrPin);

  // Інвертуємо діапазони
  int dacValue = map(lightLevel, 0, 4095, 255, 0);
  dacValue = constrain(dacValue, 0, 255);

  dacWrite(dacPin, dacValue);

  Serial.print("Світло: ");
  Serial.print(lightLevel);
  Serial.print(" => ЦАП: ");
  Serial.println(dacValue);

  delay(100);
}
