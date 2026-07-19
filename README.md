# AlashKeypadMatrix

> 🌐 **English** | [Русский](README_ru.md)

Arduino library that simplifies working with matrix keypads — reading key presses and driving events for keypads of any size (4x4, 3x4, etc.) with minimal setup.

## Key features

- Supports matrix keypads of any size (4x4, 3x4, and others)
- Simple setup via `makeKeymap`
- Handles press, hold, and release states
- Event-driven API via `addEventListener`
- Straightforward wiring to Arduino row/column pins

## Core API

- `makeKeymap(keys)` — builds a keymap from a character array
- `begin(keymap)` — initializes the keypad with a given keymap
- `getKey()` — returns the character of the currently pressed key
- `addEventListener(listener)` — registers a callback for key events
- `getState()` — returns the current key state (pressed, held, released)
- `setHoldTime(time)` — sets how long a key must be held to register as "hold"

## Example

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
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {8, 9, 10, 11};

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

Check the bundled examples for more usage patterns, including multi-keypad setups and custom event handling.

## Other Arduino libraries from Alash-electronics

[github.com/Alash-electronics](https://github.com/Alash-electronics)
