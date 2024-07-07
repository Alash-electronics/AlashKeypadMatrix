# Библиотека AlashKeypadMatrix
Библиотека AlashKeypadMatrix предназначена для упрощения работы с матричными клавиатурами, позволяя легко считывать нажатия клавиш и управлять ими. С помощью этой библиотеки можно легко интегрировать матричные клавиатуры в свои проекты на платформе Arduino.

## Основные возможности:
Поддержка матричных клавиатур различного размера (например, 4x4, 3x4 и т.д.).
Простая настройка и инициализация клавиатуры с помощью функции makeKeymap.
Обработка нажатий, удержаний и отпусканий клавиш.
Поддержка событий для клавиш с использованием функции addEventListener.
Легкое подключение к пинам Arduino для строк и столбцов матрицы клавиатуры.
## Основные функции:
### makeKeymap(keys): Создает карту клавиш на основе заданного массива символов.
### begin(keymap): Инициализирует клавиатуру с заданной картой клавиш.
### getKey(): Возвращает символ нажатой клавиши.
### addEventListener(listener): Добавляет обработчик событий для клавиатуры.
### getState(): Возвращает текущее состояние клавиши (нажата, удерживается, отпущена).
### setHoldTime(time): Устанавливает время удержания для клавиши.
## Пример использования:

```cpp
#include <AlashKeypadMatrix.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

AlashKeypadMatrix keypad = AlashKeypadMatrix(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent);
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
  }
}

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      Serial.println("Key Pressed");
      break;
    case RELEASED:
      Serial.println("Key Released");
      break;
    case HOLD:
      Serial.println("Key Held Down");
      break;
  }
}

```

### Рекомендуется ознакомиться со всеми примерами библиотеки, чтобы лучше понять её возможности и способы применения. Это поможет вам эффективно использовать библиотеку в ваших проектах.

### Желаю вам удачи в ваших проектах! Пусть ваши разработки будут успешными и приносят удовольствие и новые знания. Удачи в кодинге и реализации всех ваших идей!
