/* Этот код управляет матричной клавиатурой, поддерживающей буквенные и цифровые режимы ввода. 
 * Он обрабатывает нажатия клавиш, переключение режимов ввода и выполнение определенных действий в зависимости от нажатых клавиш. 
 * Массивы `keys` и `keyStringArray` определяют карту клавиш и соответствующие им строки символов. 
 * Объект `AlashKeypadMatrix` инициализирует матричную клавиатуру. 
 * В функции `setup()` инициализируется последовательное соединение, пин 13 настраивается как выход, и добавляется обработчик событий клавиатуры. 
 * Функция `loop()` считывает нажатую клавишу и выводит текущий текст и режим. `processKeyChar(int keyVal)` обрабатывает многократные нажатия для переключения символов, удаляет последний символ и добавляет новый. 
 * `isAlphanumericKey(int keyVal)` проверяет, является ли клавиша буквенно-цифровой. `handleLongPress(int keyValue)` обрабатывает длительное нажатие на клавишу `#`, переключая режимы ввода. 
 * `keypadEvent(KeypadEvent key)` определяет состояние клавиатуры (нажатие, удержание, отпускание) и выполняет соответствующие действия. 
 * При отпускании клавиши проверяет длительность нажатия и выполняет соответствующее действие. Добавляет символы в `outputText` и выполняет команды (`cancel`, `on`, `off`). 
 * Длительное нажатие клавиши `#` переключает между режимами ввода, многократные нажатия позволяют циклически переключаться между символами на одной клавише, а команды `on` и `off` управляют светодиодом.
*/
//  1     2      3      cancel
//  4     5      6      ok
//  7     8      9      on
//  *     0   #(mode)   off

#include <AlashKeypadMatrix.h>

const byte ROWS = 4; // четыре строки
const byte COLS = 4; // четыре столбца
static byte keypadState; // состояние клавиатуры

unsigned long lastPressTime; // время последнего нажатия
int pressInterval = 1500; // интервал для многократного нажатия
int charIndex = 1; // индекс текущего символа для каждой клавиши
int previousKey; // предыдущая нажатая клавиша
String outputText = ""; // выводимый текст
String currentAction = ""; // текущая выполняемая команда
bool isLongPress = false; // флаг длительного нажатия

// матрица клавиш
char keys[ROWS][COLS] = {
  { 0, 1, 2, 3 },
  { 4, 5, 6, 7 },
  { 8, 9, 10, 11 },
  { 12, 13, 14, 15 }
};

// массив строк для каждой клавиши
String keyStringArray[16] = {
  "1", "2abcABC ", "3defDEF ", "cancel", // строки для клавиш 1, 2, 3, отмена
  "4ghiGHI ", "5jklJKL ", "6mnoMNO ", "ok", // строки для клавиш 4, 5, 6, ок
  "7pqrsPQRS", "8tuvTUV ", "9wxyzWXYZ", "on", // строки для клавиш 7, 8, 9, включение
  "*", "0", "#", "off" // строки для клавиш *, 0, #, выключение
};

// индексы клавиш, которые содержат буквенно-цифровые символы
int alphanumericKeys[12] = {0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14};

// массив действий для длинного нажатия
char* longPressActions[16] = {
  "", "", "", "", // действия для клавиш 1, 2, 3, отмена
  "", "", "", "", // действия для клавиш 4, 5, 6, ок
  "", "", "", "", // действия для клавиш 7, 8, 9, включение
  "", "", "mode", "" // действия для клавиш *, 0, #, выключение
};

// режимы клавиш: нет, маленькие буквы, большие буквы
char* keyModes[3] = {"no", "small", "cap"};
int currentMode = 0; // текущий режим

// пины для строк и столбцов матричной клавиатуры
byte rowPins[ROWS] = {2, 3, 4, 5}; // подключение к строкам матрицы AlashKeypadMatrix
byte colPins[COLS] = {8, 9, 10, 11}; // подключение к столбцам матрицы AlashKeypadMatrix

// инициализация матричной клавиатуры
AlashKeypadMatrix keypad = AlashKeypadMatrix(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600); // инициализация последовательного соединения
  pinMode(13, OUTPUT); // настройка пина 13 как выход
  keypad.addEventListener(keypadEvent); // добавление обработчика событий клавиатуры
  keypad.setHoldTime(1500); // установка времени удержания клавиши для длительного нажатия
  lastPressTime = millis(); // установка времени последнего нажатия
}

void loop() {
  char key = keypad.getKey(); // получение нажатой клавиши
  char *cstr = new char[outputText.length() + 1]; // создание массива символов для вывода текста
  cstr = outputText.c_str(); // преобразование строки в массив символов
  Serial.print(outputText); // вывод текущего текста
  Serial.print("\t\t\t\t\tCurrent Mode: ");
  Serial.println(keyModes[currentMode]); // вывод текущего режима
}

void processKeyChar(int keyVal) {
  int modeOffset = currentMode == 2 ? 3 : 0; // смещение для больших букв
  int maxIndex = ((keyStringArray[keyVal].length() - 1) / 2) + 1; // максимальный индекс для символов на клавише
  Serial.println(maxIndex); // вывод максимального индекса для отладки

  // проверка времени между нажатиями и одинаковости клавиши
  if ((millis() - lastPressTime) < pressInterval && previousKey == keyVal) {
    charIndex++; // увеличение индекса символа
    if (charIndex == maxIndex) {
      charIndex = 1; // сброс индекса, если достигнут конец строки
    }
    outputText.remove(outputText.length() - 1); // удаление последнего символа
    outputText = outputText + keyStringArray[keyVal].substring(charIndex + modeOffset, charIndex + modeOffset + 1); // добавление нового символа
  } else {
    charIndex = 1; // сброс индекса символа
    outputText = outputText + keyStringArray[keyVal].substring(charIndex + modeOffset, charIndex + modeOffset + 1); // добавление нового символа
  }
  previousKey = keyVal; // установка предыдущей клавиши
  lastPressTime = millis(); // обновление времени последнего нажатия
}

bool isAlphanumericKey(int keyVal) {
  // проверка, является ли клавиша буквенно-цифровой
  for (int i = 0; i < 12; i++) {
    if (alphanumericKeys[i] == keyVal) {
      return true; // клавиша является буквенно-цифровой
    }
  }
  return false; // клавиша не является буквенно-цифровой
}

void handleLongPress(int keyValue) {
  // обработка длительного нажатия
  if (longPressActions[keyValue] == "mode") {
    currentMode++; // переключение режима
    if (currentMode == 3) {
      currentMode = 0; // сброс режима, если достигнут конец списка
    }
    Serial.print("Mode changed to: ");
    Serial.println(keyModes[currentMode]); // вывод нового режима
  }
}

void keypadEvent(KeypadEvent key) {
  keypadState = keypad.getState(); // получение состояния клавиатуры
  int keyVal = key; // получение значения клавиши
  Serial.println(keyVal); // вывод значения клавиши

  switch (keypadState) {
    case PRESSED:
      // обработка нажатия клавиши
      break;

    case HOLD:
      // обработка удержания клавиши
      isLongPress = true; // установка флага длительного нажатия
      break;

    case RELEASED:
      // обработка отпускания клавиши
      if (isLongPress) {
        handleLongPress(keyVal); // вызов функции обработки длительного нажатия
        isLongPress = false; // сброс флага длительного нажатия
      } else {
        Serial.println(keyStringArray[keyVal]); // вывод строки для клавиши
        if (isAlphanumericKey(keyVal)) {
          if (currentMode == 0) {
            outputText = outputText + keyStringArray[keyVal].substring(0, 1); // добавление символа в обычном режиме
          } else {
            processKeyChar(keyVal); // обработка символа в режиме букв
          }
        } else {
          currentAction = keyStringArray[keyVal]; // установка текущей команды
          if (currentAction == "cancel") {
            outputText.remove(outputText.length() - 1); // удаление последнего символа
          }
          if (currentAction == "on") {
            digitalWrite(13, HIGH); // включение пина 13
          }
          if (currentAction == "off") {
            digitalWrite(13, LOW); // выключение пина 13
          }
        }
      }
      break;
  }
}