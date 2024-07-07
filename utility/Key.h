// Написан для Alash Education

#ifndef KEY_H
#define KEY_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"    // для digitalRead, digitalWrite и т. д.
#else
#include "WProgram.h"
#endif

#define OPEN LOW        // Состояние "открыто" для ключа
#define CLOSED HIGH     // Состояние "закрыто" для ключа

typedef unsigned int uint;    // Тип для беззнакового целого числа
typedef enum { IDLE, PRESSED, HOLD, RELEASED } KeyState;    // Перечисление состояний клавиши

const char NO_KEY = '\0';    // Символ, представляющий отсутствие нажатой клавиши

class Key {
public:
    // Члены класса
    char kchar;             // Символ клавиши
    int kcode;              // Код клавиши (не используется в этом примере)
    KeyState kstate;        // Текущее состояние клавиши
    boolean stateChanged;   // Флаг изменения состояния клавиши

    // Методы класса
    Key();                                  // Конструктор по умолчанию
    Key(char userKeyChar);                  // Конструктор с инициализацией символа клавиши
    void key_update(char userKeyChar, KeyState userState, boolean userStatus);  // Обновление данных о клавише

private:

};

#endif