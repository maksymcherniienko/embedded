#include <OneWire.h>
#include <DallasTemperature.h>

// Налаштування 1-Wire та датчика
OneWire oneWire(4); // Пін, до якого підключено датчик
DallasTemperature sensors(&oneWire);

// Константи
const int NUM_READINGS = 10;
float raw_temperatures[NUM_READINGS];
float filtered_temperatures[NUM_READINGS];

// Початкові значення для фільтра Кальмана
float last_estimate = 0; // Попередня оцінка температури
float Err_disp = 1.0;      // Початкова дисперсія помилки
float Q = 0.022;         // Шум процесу
float R = 0.617;         // Шум вимірювання

void setup() {
  Serial.begin(9600);
  sensors.begin();
  
  Serial.println("Починаємо збір даних...");
  
  // Вичитка значень з інтервалом
  for(int i = 0; i < NUM_READINGS; i++) {
    sensors.requestTemperatures(); // Запит на читання температури
    raw_temperatures[i] = sensors.getTempCByIndex(0);
    
    Serial.print("Вимір ["); Serial.print(i + 1); Serial.print("]: ");
    Serial.print(raw_temperatures[i]); Serial.println(" °C");
    
    delay(1000); // Інтервал
  }
  
  Serial.println("\nЗбір даних завершено. Запускаємо фільтр Кальмана...\n");
  
  // Фільтрація зібраного масиву
  // Ініціалізуємо фільтр першим виміряним значенням
  last_estimate = raw_temperatures[0];
  
  Serial.println("Raw Temp (°C)\tFiltered Temp (°C)\tDifference");
  Serial.println("--------------------------------------------------");

  for(int i = 0; i < NUM_READINGS; i++) {
    float measured_temp = raw_temperatures[i]; // перше виміряне значення
    
    // 1. Прогноз
    float predicted_temp = last_estimate;     
    float P_temp = Err_disp + Q;
    
    // 2. Обчислення коефіцієнта Кальмана
    float K = P_temp * (1.0 / (P_temp + R));
    
    // 3. Фінальна температура
    float estimated_temp = predicted_temp + K * (measured_temp - predicted_temp);
    // 4. Оновлення дисперсії помилки
    float P = (1.0 - K) * P_temp;
    
    // Зберігаємо відфільтроване значення
    filtered_temperatures[i] = estimated_temp;
    
    // Вивід результатів
    Serial.print(measured_temp, 3);
    Serial.print("\t\t");
    Serial.print(estimated_temp, 3);
    Serial.print("\t\t\t");
    Serial.println(abs(measured_temp - estimated_temp), 3);
    
    // Оновлення змінних для наступної ітерації
    Err_disp = P;
    last_estimate = estimated_temp;
  }
  Serial.println("\nОбробка завершена.");
}
void loop() {}