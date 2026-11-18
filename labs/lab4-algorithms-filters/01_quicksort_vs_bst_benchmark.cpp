#include "soc/rtc.h"

// Шаблонна структура вузла дерева (підійде для int, float, double, char)
template <typename T>
struct Node {
    T data;         // Саме значення (число або символ)
    Node* left;     // Покажчик на лівого нащадка (менші значення)
    Node* right;    // Покажчик на правого нащадка (більші значення)

    // Конструктор: коли створюємо новий вузол, лівий і правий покажчики порожні (nullptr)
    Node(T val) {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

template <typename T>
Node<T>* insert(Node<T>* root, T value) {
    // Якщо дійшли до порожнього місця — створюємо новий вузол
    if (root == nullptr) {
        return new Node<T>(value);
    }

    // Якщо значення менше за поточний вузол — йдемо ліворуч
    if (value < root->data) {
        root->left = insert(root->left, value);
    } 
    // Якщо більше або дорівнює — йдемо праворуч
    else {
        root->right = insert(root->right, value);
    }

    return root; // Повертаємо незмінений корінь
}

template <typename T>
// Функція для обміну елементів
void swap(T* a, T* b) {
    T t = *a;
    *a = *b;
    *b = t;
}

// Функція розділення: ставить опорний елемент (pivot) на своє місце
template <typename T>
int partition(T arr[], int low, int high) {
    T pivot = arr[high]; 
    int i = (low - 1);    

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) { 
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return (i+1);
}

template <typename T>
void quickSort(T arr[], int low, int high) {
  if (low < high) {
  int pi = partition(arr, low, high);
  quickSort(arr, low, pi - 1);
  quickSort(arr, pi + 1, high);
  }
}

void setup() {
  Serial.begin(115200);
  // Встановлюємо частоту (доступні: 240, 160, 80, 40)
  setCpuFrequencyMhz(240);
  delay(10);
  Serial.println("Старт...");

  const int sizeofIntArr = 1000;
  const int sizeofDoubleArr = 100;
  const int sizeofCharArr = 50;
  const int sizeofFloatArr = 500;
  static int intArray[sizeofIntArr];
  static double doubleArray[sizeofDoubleArr];
  static char charArray[sizeofCharArr];
  static float floatArray[sizeofFloatArr];
  unsigned long startTime, endTime, timeTaken;
  uint32_t memBefore, memAfter, memUsed; // лічильники пам'яті
  // масиви для звіту
  unsigned long resultsTime[8]; // 4 для сортування + 4 для дерев
  uint32_t resultsMem[8];       // Пам'ять для кожного етапу

  Serial.begin(115200);
  // 1. Заповнюємо int, double, char, float масиви
  for (int i = 0; i < sizeofIntArr; i++) intArray[i] = esp_random() % 1000; 
  for (int i = 0; i < sizeofDoubleArr; i++) doubleArray[i] = (double)(esp_random() % 100);   
  for (int i = 0; i < sizeofCharArr; i++) charArray[i] = 'a' + (esp_random() % 26);
  for (int i = 0; i < sizeofFloatArr; i++) floatArray[i] = (esp_random() % 1000) / 100.0f;

  // 2. Сортуємо
  memBefore = ESP.getFreeHeap(); // Пам'ять до сортування
  startTime = micros(); // Старт секундоміра
  quickSort(intArray, 0, sizeofIntArr - 1);
  endTime = micros();   // Фініш
  timeTaken = endTime - startTime;
  Serial.print("Час сортування int масиву: ");
  Serial.print(timeTaken);
  Serial.println(" мкс");
  memAfter = ESP.getFreeHeap(); // Пам'ять після сортування
  memUsed = memBefore - memAfter; // Використано пам'яті
  Serial.print("Пам'ять, використана для сортування int: ");
  Serial.print(memUsed);
  Serial.println(" байт");
  resultsTime[0] = timeTaken; // Зберігаємо час сортування int
  resultsMem[0] = memUsed; // Зберігаємо пам'ять для сортування int

  memBefore = ESP.getFreeHeap(); // Пам'ять до сортування
  startTime = micros(); // Старт секундоміра
  quickSort(doubleArray, 0, sizeofDoubleArr - 1);
  endTime = micros();   // Фініш
  timeTaken = endTime - startTime;
  Serial.print("Час сортування double масиву: ");
  Serial.print(timeTaken);
  Serial.println(" мкс");
  memAfter = ESP.getFreeHeap(); // Пам'ять після сортування
  memUsed = memBefore - memAfter; // Використано пам'яті
  Serial.print("Пам'ять, використана для сортування double: ");
  Serial.print(memUsed);
  Serial.println(" байт");
  resultsTime[1] = timeTaken; // Зберігаємо час сортування double
  resultsMem[1] = memUsed; // Зберігаємо пам'ять для сортування double

  memBefore = ESP.getFreeHeap(); // Пам'ять до сортування
  startTime = micros(); // Старт секундоміра
  quickSort(charArray, 0, sizeofCharArr - 1);
  endTime = micros();   // Фініш
  timeTaken = endTime - startTime;
  Serial.print("Час сортування char масиву: ");
  Serial.print(timeTaken);
  Serial.println(" мкс");
  memAfter = ESP.getFreeHeap(); // Пам'ять після сортування
  memUsed = memBefore - memAfter; // Використано пам'яті
  Serial.print("Пам'ять, використана для сортування char: ");
  Serial.print(memUsed);
  Serial.println(" байт");
  resultsTime[2] = timeTaken; // Зберігаємо час сортування char
  resultsMem[2] = memUsed; // Зберігаємо пам'ять для сортування char

  memBefore = ESP.getFreeHeap(); // Пам'ять до сортування
  startTime = micros(); // Старт секундоміра
  quickSort(floatArray, 0, sizeofFloatArr - 1);
  endTime = micros();   // Фініш
  timeTaken = endTime - startTime;
  Serial.print("Час сортування float масиву: ");
  Serial.print(timeTaken);
  Serial.println(" мкс");
  memAfter = ESP.getFreeHeap(); // Пам'ять після сортування
  memUsed = memBefore - memAfter; // Використано пам'яті
  Serial.print("Пам'ять, використана для сортування float: ");
  Serial.print(memUsed);
  Serial.println(" байт");
  resultsTime[3] = timeTaken; // Зберігаємо час сортування float
  resultsMem[3] = memUsed; // Зберігаємо пам'ять для сортування float

  // 3. Виводимо результат
  Serial.print("Відсортований масив Int: ");
    for (int i = 0; i < sizeofIntArr; i++) {
    Serial.print(intArray[i]);
    Serial.print(i == sizeofIntArr - 1 ? "" : ", ");
  }
  Serial.println();
    Serial.print("Відсортований масив Double: ");
    for (int i = 0; i < sizeofDoubleArr; i++) {
    Serial.print(doubleArray[i]);
    Serial.print(i == sizeofDoubleArr - 1 ? "" : ", ");
  }
  Serial.println();
    Serial.print("Відсортований масив Char: ");
    for (int i = 0; i < sizeofCharArr; i++) {
    Serial.print(charArray[i]);
    Serial.print(i == sizeofCharArr - 1 ? "" : ", ");
  }
  Serial.println();
    Serial.print("Відсортований масив Float: ");
    for (int i = 0; i < sizeofFloatArr; i++) {
    Serial.print(floatArray[i]);
    Serial.print(i == sizeofFloatArr - 1 ? "" : ", ");
  }
  Serial.println();

Serial.println("\nПерегенерую масиви випадковими числами для дерев...");
  
  for (int i = 0; i < sizeofIntArr; i++) intArray[i] = esp_random() % 1000;
  for (int i = 0; i < sizeofDoubleArr; i++) doubleArray[i] = (double)(esp_random() % 10000) / 100.0;
  for (int i = 0; i < sizeofCharArr; i++) charArray[i] = 'a' + (esp_random() % 26);
  for (int i = 0; i < sizeofFloatArr; i++) floatArray[i] = (float)(esp_random() % 1000) / 10.0f;
  
  // бінарні дерева для кожного типу даних
  Node<int>* rootInt = nullptr;

  // Закидаємо весь масив у дерево

  memBefore = ESP.getFreeHeap(); // Пам'ять до побудови дерева
  startTime = micros(); // Старт секундоміра
    for(int i = 0; i < sizeofIntArr; i++) {
    rootInt = insert(rootInt, intArray[i]); 
  }
  Serial.println("Дерево для масиву int на " + String(sizeofIntArr) + " елементів побудовано!");
   endTime = micros();   // Фініш
   timeTaken = endTime - startTime;
  Serial.print("Час побудови дерева: ");
  Serial.print(timeTaken);
  Serial.println(" мкс");
  memAfter = ESP.getFreeHeap(); // Пам'ять після побудови дерева
  memUsed = memBefore - memAfter; // Використано пам'яті
  Serial.print("Пам'ять, використана для дерева int: ");
  Serial.print(memUsed);
  Serial.println(" байт");
  resultsMem[4] = memUsed; // Зберігаємо пам'ять для дерева int
  resultsTime[4] = timeTaken; // Зберігаємо час для дерева int

  memBefore = ESP.getFreeHeap(); // Пам'ять до побудови дерева
  startTime = micros(); // Старт секундоміра
  Node<double>* rootDouble = nullptr;
  for(int i = 0; i < sizeofDoubleArr; i++) {
    rootDouble = insert(rootDouble, doubleArray[i]); 
  }
  Serial.println("Дерево для масиву double на " + String(sizeofDoubleArr) + " елементів побудовано!");
  endTime = micros();   // Фініш
  timeTaken = endTime - startTime;
  Serial.print("Час побудови дерева: ");
  Serial.print(timeTaken);
  Serial.println(" мкс");
  memAfter = ESP.getFreeHeap(); // Пам'ять після побудови дерева
  memUsed = memBefore - memAfter; // Використано пам'яті
  Serial.print("Пам'ять, використана для дерева double: ");
  Serial.print(memUsed);
  Serial.println(" байт");
  resultsMem[5] = memUsed; // Зберігаємо пам'ять для дерева double
  resultsTime[5] = timeTaken; // Зберігаємо час для дерева double

  memBefore = ESP.getFreeHeap(); // Пам'ять до побудови дерева
  startTime = micros(); // Старт секундоміра
  Node<float>* rootFloat = nullptr;
  for(int i = 0; i < sizeofFloatArr; i++) {
    rootFloat = insert(rootFloat, floatArray[i]);
    if (i %50 == 0) yield(); //щоб не блокувати систему
  }
  Serial.println("Дерево для масиву float на " + String(sizeofFloatArr) + " елементів побудовано!");
  endTime = micros();   // Фініш
  timeTaken = endTime - startTime;
  Serial.print("Час побудови дерева: ");
  Serial.print(timeTaken);
  Serial.println(" мкс");
  memAfter = ESP.getFreeHeap(); // Пам'ять після побудови дерева
  memUsed = memBefore - memAfter; // Використано пам'яті
  Serial.print("Пам'ять, використана для дерева float: ");
  Serial.print(memUsed);
  Serial.println(" байт");
  resultsMem[6] = memUsed; // Зберігаємо пам'ять для дерева float
  resultsTime[6] = timeTaken; // Зберігаємо час для дерева

  memBefore = ESP.getFreeHeap(); // Пам'ять до побудови дерева
  startTime = micros(); // Старт секундоміра
  Node<char>* rootChar = nullptr;
  for(int i = 0; i < sizeofCharArr; i++) {
    rootChar = insert(rootChar, charArray[i]);
    if (i %50 == 0) yield(); //щоб не блокувати систему
  }
  Serial.println("Дерево для масиву char на " + String(sizeofCharArr) + " елементів побудовано!");
  endTime = micros();   // Фініш
  timeTaken = endTime - startTime;
  Serial.print("Час побудови дерева: ");
  Serial.print(timeTaken);
  Serial.println(" мкс");
  memAfter = ESP.getFreeHeap(); // Пам'ять після побудови дерева
  memUsed = memBefore - memAfter; // Використано пам'яті
  Serial.print("Пам'ять, використана для дерева char: ");
  Serial.print(memUsed);
  Serial.println(" байт");
  resultsMem[7] = memUsed; // Зберігаємо пам'ять для дерева char
  resultsTime[7] = timeTaken; // Зберігаємо час для дерева char

  Serial.println("\n--- ФІНАЛЬНИЙ ЗВІТ ---");
  Serial.println("Тип, Розмір, Алгоритм, Час(мкс), Пам'ять(байт)");

  String names[] = {"Int", "Double", "Char", "Float"};
  int sizes[] = {sizeofIntArr, sizeofDoubleArr, sizeofCharArr, sizeofFloatArr};

  Serial.println("Частота:" + String(getCpuFrequencyMhz()) + " MHz");
  for (int i = 0; i < 4; i++) {
      // Вивід для QuickSort (індекси 0-3)
      Serial.printf("%s,%d,QuickSort,%lu,%u\n", names[i].c_str(), sizes[i], resultsTime[i], resultsMem[i]);
  }
  for (int i = 0; i < 4; i++) {
      // Вивід для Trees (індекси 4-7)
      Serial.printf("%s,%d,TreeBuild,%lu,%u\n", names[i].c_str(), sizes[i], resultsTime[i+4], resultsMem[i+4]);
  }
}

void loop() {}