// Написан для Alash Education

#include <Key.h>

// Конструктор по умолчанию
Key::Key() {
	kchar = NO_KEY;         // Инициализация символа клавиши как NO_KEY
	kstate = IDLE;          // Инициализация состояния как IDLE
	stateChanged = false;   // Инициализация флага изменения состояния как ложь
}

// Конструктор
Key::Key(char userKeyChar) {
	kchar = userKeyChar;    // Установка символа клавиши из аргумента
	kcode = -1;             // Инициализация кода клавиши как -1
	kstate = IDLE;          // Инициализация состояния как IDLE
	stateChanged = false;   // Инициализация флага изменения состояния как ложь
}

// Обновление данных клавиши
void Key::key_update(char userKeyChar, KeyState userState, boolean userStatus) {
	kchar = userKeyChar;    // Установка символа клавиши
	kstate = userState;     // Установка состояния клавиши
	stateChanged = userStatus;  // Установка флага изменения состояния
}
