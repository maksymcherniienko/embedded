#include <OneWire.h>
#include <DallasTemperature.h>

const int ONE_WIRE_BUS = 4; // GPIO 4 для лінії даних
const int NUM_SAMPLES = 100;
const int INTERVAL_MS = 200; // Інтервал 
const int WINDOW_SIZE = 5;    // Розмір вікна для медіанного фільтра (бажано непарне число)

// Налаштування 1-Wire та сенсора
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Масиви для сирих та відфільтрованих даних
float rawTemperatures[NUM_SAMPLES];
float filteredTemperatures[NUM_SAMPLES];

// Функція реалізації 1D медіанного фільтру на основі наданого псевдокоду
void applyMedianFilter() {
  float window[WINDOW_SIZE];
  int edge = WINDOW_SIZE / 2; // для вікна 5, edge = 2
  
  // Копіюємо краї масиву "як є", оскільки вікно там не поміщається повністю
  for (int i = 0; i < NUM_SAMPLES; i++) {
    filteredTemperatures[i] = rawTemperatures[i];
  }

  // Проходимось по масиву від edge до довжини мінус edge
  // Це еквівалент вашого: for x from edgex to image width - edgex do
  for (int x = edge; x < NUM_SAMPLES - edge; x++) {
    
    // 1. Заповнюємо вікно (еквівалент внутрішнього циклу fx)
    for (int fx = 0; fx < WINDOW_SIZE; fx++) {
      window[fx] = rawTemperatures[x + fx - edge];
    }
    
    // 2. Сортуємо вікно (використаємо простий алгоритм сортування бульбашкою)
    // Це еквівалент вашого: sort entries in window[]
    for (int i = 0; i < WINDOW_SIZE - 1; i++) {
      for (int j = 0; j < WINDOW_SIZE - i - 1; j++) {
        if (window[j] > window[j + 1]) {
          float temp = window[j];
          window[j] = window[j + 1];
          window[j + 1] = temp;
        }
      }
    }
    
    // 3. Беремо середнє (медіанне) значення і записуємо у вихідний масив
    // Це еквівалент: outputPixelValue[x] = window[window width / 2]
    filteredTemperatures[x] = window[WINDOW_SIZE / 2];
  }
}

void setup() {
  Serial.begin(115200);
  sensors.begin();
  
  Serial.println("Початок вичитки 100 значень температури...");
  
  // Крок 1: Вичитка 100 значень
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sensors.requestTemperatures(); 
    rawTemperatures[i] = sensors.getTempCByIndex(0);
    
    Serial.print("Зразок [");
    Serial.print(i);
    Serial.print("]: ");
    Serial.print(rawTemperatures[i]);
    Serial.println(" °C");
    
    delay(INTERVAL_MS);
  }
  
  Serial.println("Вичитка завершена. Застосування медіанного фільтру...");
  
  // Крок 2: Застосування одновимірного медіанного фільтру
  applyMedianFilter();
  
  // Крок 3: Вивід результатів для порівняння
  Serial.println("Результати (Сирі дані -> Відфільтровані дані):");
  for (int i = 0; i < NUM_SAMPLES; i++) {
    Serial.print(i);
    Serial.print("\t");
    Serial.print(rawTemperatures[i]);
    Serial.print("\t->\t");
    Serial.println(filteredTemperatures[i]);
  }
}
void loop() {}