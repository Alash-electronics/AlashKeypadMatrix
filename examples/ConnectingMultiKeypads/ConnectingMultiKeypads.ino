/*Этот код управляет тремя различными клавиатурами, каждая из которых имеет свои символы и подключена к определенным пинам. 
 * Первая клавиатура предназначена для ввода цифр, 
 * вторая - для ввода букв, 
 * а третья - для ввода специальных символов. 
 * 
 * В функции setup() каждая клавиатура инициализируется, а последовательное соединение настраивается для вывода информации. 
 * В функции loop() код проверяет, какая клавиша была нажата на каждой из клавиатур, и выводит соответствующий символ на последовательный порт.
*/
#include <AlashKeypadMatrix.h>

// Определяем количество строк и столбцов для цифровой клавиатуры
const byte NUM_ROWS = 2; // используем 2x2 клавиатуру для примера
const byte NUM_COLS = 2;

// Определяем символы для цифровой клавиатуры
char numericKeys[NUM_ROWS][NUM_COLS] = {
  {'1','2'},
  {'3','4'}
};

// Пины для подключения строк и столбцов цифровой клавиатуры
byte numericRowPins[NUM_ROWS] = {5, 4}; // подключение к строкам матричной клавиатуры
byte numericColPins[NUM_COLS] = {7, 6}; // подключение к столбцам матричной клавиатуры

// Создаем объект для цифровой клавиатуры
AlashKeypadMatrix numericKeypad(makeKeymap(numericKeys), numericRowPins, numericColPins, NUM_ROWS, NUM_COLS);

// Определяем количество строк и столбцов для буквенной клавиатуры
const byte ALPHA_ROWS = 2;
const byte ALPHA_COLS = 2;

// Определяем символы для буквенной клавиатуры
char alphaKeys[ALPHA_ROWS][ALPHA_COLS] = {
  {'a','b'},
  {'c','d'}
};

// Пины для подключения строк и столбцов буквенной клавиатуры
byte alphaRowPins[ALPHA_ROWS] = {3, 2}; // подключение к строкам матричной клавиатуры
byte alphaColPins[ALPHA_COLS] = {7, 6}; // подключение к столбцам матричной клавиатуры

// Создаем объект для буквенной клавиатуры
AlashKeypadMatrix alphaKeypad(makeKeymap(alphaKeys), alphaRowPins, alphaColPins, ALPHA_ROWS, ALPHA_COLS);

// Определяем количество строк и столбцов для специальной клавиатуры
const byte SPECIAL_ROWS = 4;
const byte SPECIAL_COLS = 1;

// Определяем символы для специальной клавиатуры
char specialKeys[SPECIAL_ROWS][SPECIAL_COLS] = {
  {'M'},
  {'A'},
  {'R'},
  {'K'}
};

// Пины для подключения строк и столбцов специальной клавиатуры
byte specialRowPins[SPECIAL_ROWS] = {5, 4, 3, 2}; // подключение к строкам матричной клавиатуры
byte specialColPins[SPECIAL_COLS] = {8}; // подключение к столбцам матричной клавиатуры

// Создаем объект для специальной клавиатуры
AlashKeypadMatrix specialKeypad(makeKeymap(specialKeys), specialRowPins, specialColPins, SPECIAL_ROWS, SPECIAL_COLS);

void setup() {
  // Инициализация всех клавиатур
  specialKeypad.begin(makeKeymap(specialKeys));
  alphaKeypad.begin(makeKeymap(alphaKeys));
  numericKeypad.begin(makeKeymap(numericKeys));

  // Инициализация последовательного соединения для вывода информации
  Serial.begin(9600);
  Serial.println("start");
}

// Переменные для хранения нажатых клавиш
char numericKey, alphaKey, specialKey;

void loop() {
  // Считывание нажатой клавиши с цифровой клавиатуры
  numericKey = numericKeypad.getKey();
  
  // Считывание нажатой клавиши с буквенной клавиатуры
  alphaKey = alphaKeypad.getKey();
  
  // Считывание нажатой клавиши с специальной клавиатуры
  specialKey = specialKeypad.getKey();

  // Проверка и вывод нажатой клавиши цифровой клавиатуры
  if (numericKey) {
    Serial.println(numericKey);
  }

  // Проверка и вывод нажатой клавиши буквенной клавиатуры
  if (alphaKey) {
    Serial.println(alphaKey);
  }

  // Проверка и вывод нажатой клавиши специальной клавиатуры
  if (specialKey) {
    Serial.println(specialKey);
  }
}
