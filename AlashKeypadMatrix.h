// Написан для Alash Education

#ifndef ALASHKEYPADMATRIX_H
#define ALASHKEYPADMATRIX_H

#include "utility/Key.h"

// Версии Arduino.
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h" // Подключение библиотеки для версий Arduino 1.0 и выше
#else
#include "WProgram.h" // Подключение библиотеки для версий Arduino ниже 1.0
#endif

#ifndef INPUT_PULLUP
#warning "Использование pinMode() INPUT_PULLUP AVR эмуляция" // Предупреждение об эмуляции INPUT_PULLUP
#define INPUT_PULLUP 0x2
#define pinMode(_pin, _mode) _mypinMode(_pin, _mode) // Замена pinMode макросом _mypinMode
#define _mypinMode(_pin, _mode)  \
do {							 \
	if(_mode == INPUT_PULLUP)	 \
		pinMode(_pin, INPUT);	 \
		digitalWrite(_pin, 1);	 \
	if(_mode != INPUT_PULLUP)	 \
		pinMode(_pin, _mode);	 \
}while(0)
#endif

#define OPEN LOW // Определение состояния "открыто"
#define CLOSED HIGH // Определение состояния "закрыто"

typedef char KeypadEvent; // Определение типа для события клавиатуры
typedef unsigned int uint; // Определение типа для беззнакового целого числа
typedef unsigned long ulong; // Определение типа для беззнакового длинного числа

typedef struct {
    byte rows; // Количество строк
    byte columns; // Количество столбцов
} AlashKeypadMatrixSize;

#define LIST_MAX 10 // Максимальное количество клавиш в активном списке
#define MAPSIZE 10 // MAPSIZE - это количество строк (умноженное на 16 столбцов)
#define makeKeymap(x) ((char*)x) // Макрос для создания карты клавиш

// Определение класса AlashKeypadMatrix, который наследуется от класса Key
class AlashKeypadMatrix : public Key {
public:
	AlashKeypadMatrix(char *userKeymap, byte *row, byte *col, byte numRows, byte numCols); // Конструктор

	virtual void pin_mode(byte pinNum, byte mode) { pinMode(pinNum, mode); } // Метод для установки режима пина
	virtual void pin_write(byte pinNum, boolean level) { digitalWrite(pinNum, level); } // Метод для записи значения на пин
	virtual int  pin_read(byte pinNum) { return digitalRead(pinNum); } // Метод для чтения значения с пина

	uint bitMap[MAPSIZE]; // Массив битов 10 строк x 16 столбцов. За исключением Due, который имеет 32 столбца.
	Key key[LIST_MAX]; // Массив клавиш
	unsigned long holdTimer; // Таймер удержания

	char getKey(); // Метод для получения клавиши
	bool getKeys(); // Метод для получения всех клавиш
	KeyState getState(); // Метод для получения состояния клавиши
	void begin(char *userKeymap); // Метод для инициализации клавиатуры
	bool isPressed(char keyChar); // Метод для проверки нажатия клавиши
	void setDebounceTime(uint); // Метод для установки времени подавления дребезга
	void setHoldTime(uint); // Метод для установки времени удержания
	void addEventListener(void (*listener)(char)); // Метод для добавления слушателя событий
	int findInList(char keyChar); // Метод для поиска клавиши в списке по символу
	int findInList(int keyCode); // Метод для поиска клавиши в списке по коду
	char waitForKey(); // Метод для ожидания нажатия клавиши
	bool keyStateChanged(); // Метод для проверки изменения состояния клавиши
	byte numKeys(); // Метод для получения количества клавиш

private:
	unsigned long startTime; // Время начала
	char *keymap; // Карта клавиш
    byte *rowPins; // Пины строк
    byte *columnPins; // Пины столбцов
	AlashKeypadMatrixSize sizeKpd; // Размер клавиатуры
	uint debounceTime; // Время подавления дребезга
	uint holdTime; // Время удержания
	bool single_key; // Флаг одиночной клавиши

	void scanKeys(); // Метод для сканирования клавиш
	bool updateList(); // Метод для обновления списка
	void nextKeyState(byte n, boolean button); // Метод для перехода к следующему состоянию клавиши
	void transitionTo(byte n, KeyState nextState); // Метод для перехода к следующему состоянию
	void (*keypadEventListener)(char); // Указатель на функцию-слушатель событий клавиатуры
};

#endif 
