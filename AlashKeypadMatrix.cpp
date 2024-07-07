// Написан для Alash Education

#include <AlashKeypadMatrix.h>

// <<constructor>> Позволяет использовать пользовательскую карту клавиш, конфурацию пинов и размеры клавиатуры.
AlashKeypadMatrix::AlashKeypadMatrix(char *userKeymap, byte *row, byte *col, byte numRows, byte numCols) {
    // Инициализация переменных класса
    rowPins = row;
    columnPins = col;
    sizeKpd.rows = numRows;
    sizeKpd.columns = numCols;

    // Инициализация пользовательской карты клавиш
    begin(userKeymap);

    // Установка времени подавления дребезга и времени удержания
    setDebounceTime(10);
    setHoldTime(500);

    // Инициализация указателя на функцию события
    keypadEventListener = 0;

    // Инициализация начального времени и флага одноклавишного режима
    startTime = 0;
    single_key = false;
}

// Позволяет пользователю задать карту клавиш - предполагается, что количество строк и столбцов такое же, как указано в конструкторе.
void AlashKeypadMatrix::begin(char *userKeymap) {
    keymap = userKeymap;
}

// Возвращает только одну клавишу. Сохранено для обратной совместимости.
char AlashKeypadMatrix::getKey() {
    // Установка флага одноклавишного режима
    single_key = true;

    // Проверка наличия нажатой клавиши и её состояния
    if (getKeys() && key[0].stateChanged && (key[0].kstate == PRESSED))
        return key[0].kchar;

    // Сброс флага одноклавишного режима
    single_key = false;

    return NO_KEY;
}

// Заполняет список клавиш.
bool AlashKeypadMatrix::getKeys() {
    bool keyActivity = false;

    // Ограничение частоты сканирования клавиатуры для ускорения выполнения основного цикла
    if ((millis() - startTime) > debounceTime) {
        // Сканирование клавиш и обновление списка
        scanKeys();
        keyActivity = updateList();

        // Обновление времени старта
        startTime = millis();
    }

    return keyActivity;
}

// Приватный метод: аппаратное сканирование
void AlashKeypadMatrix::scanKeys() {
    // Повторная инициализация пинов строк, позволяющая совместное использование этих пинов с другим оборудованием
    for (byte r = 0; r < sizeKpd.rows; r++) {
        pin_mode(rowPins[r], INPUT_PULLUP);
    }

    // bitMap хранит все нажатые клавиши
    for (byte c = 0; c < sizeKpd.columns; c++) {
        pin_mode(columnPins[c], OUTPUT);
        pin_write(columnPins[c], LOW);  // Начало импульсного вывода на столбец
        for (byte r = 0; r < sizeKpd.rows; r++) {
            // Запись состояния пина строки в битовую карту
            bitWrite(bitMap[r], c, !pin_read(rowPins[r]));  // Нажатие клавиши активно на низком уровне, поэтому инвертируем до высокого
        }
        // Установка пина в состояние высокой импедансности, что фактически завершает импульс на столбце
        pin_write(columnPins[c], HIGH);
        pin_mode(columnPins[c], INPUT);
    }
}

// Управляет списком без перестановки клавиш. Возвращает true, если какое-либо состояние клавиш в списке изменилось.
bool AlashKeypadMatrix::updateList() {
    bool anyActivity = false;

    // Удаляет все клавиши в состоянии IDLE
    for (byte i = 0; i < LIST_MAX; i++) {
        if (key[i].kstate == IDLE) {
            key[i].kchar = NO_KEY;
            key[i].kcode = -1;
            key[i].stateChanged = false;
        }
    }

    // Добавляет новые клавиши в пустые слоты списка клавиш
    for (byte r = 0; r < sizeKpd.rows; r++) {
        for (byte c = 0; c < sizeKpd.columns; c++) {
            boolean button = bitRead(bitMap[r], c);
            char keyChar = keymap[r * sizeKpd.columns + c];
            int keyCode = r * sizeKpd.columns + c;
            int idx = findInList(keyCode);
            // Клавиша уже в списке, устанавливаем её следующее состояние
            if (idx > -1) {
                nextKeyState(idx, button);
            }
            // Клавиша НЕ в списке, добавляем её
            if ((idx == -1) && button) {
                for (byte i = 0; i < LIST_MAX; i++) {
                    if (key[i].kchar == NO_KEY) {  // Найти пустой слот или не добавлять клавишу в список
                        key[i].kchar = keyChar;
                        key[i].kcode = keyCode;
                        key[i].kstate = IDLE;  // Клавиши, НЕ находящиеся в списке, имеют начальное состояние IDLE
                        nextKeyState(i, button);
                        break;  // Не заполнять все пустые слоты одной и той же клавишей
                    }
                }
            }
        }
    }

    // Сообщает, изменил ли пользователь состояние какой-либо клавиши
    for (byte i = 0; i < LIST_MAX; i++) {
        if (key[i].stateChanged)
            anyActivity = true;
    }

    return anyActivity;
}

// Приватный метод
// Эта функция является машиной состояний, но также используется для подавления дребезга клавиш.
void AlashKeypadMatrix::nextKeyState(byte idx, boolean button) {
    // Сброс флага изменения состояния
    key[idx].stateChanged = false;

    // Переход между состояниями клавиши в зависимости от её текущего состояния и состояния кнопки
    switch (key[idx].kstate) {
        case IDLE:
            if (button == CLOSED) {
                transitionTo(idx, PRESSED);
                holdTimer = millis();  // Подготовка к следующему состоянию HOLD
            }
            break;
        case PRESSED:
            if ((millis() - holdTimer) > holdTime)  // Ожидание удержания клавиши...
                transitionTo(idx, HOLD);
            else if (button == OPEN)  // или отпускание клавиши
                transitionTo(idx, RELEASED);
            break;
        case HOLD:
            if (button == OPEN)
                transitionTo(idx, RELEASED);
            break;
        case RELEASED:
            transitionTo(idx, IDLE);
            break;
    }
}

// Новое в версии 2.1
// Проверяет, нажата ли указанная клавиша
bool AlashKeypadMatrix::isPressed(char keyChar) {
    for (byte i = 0; i < LIST_MAX; i++) {
        if (key[i].kchar == keyChar) {
            if ((key[i].kstate == PRESSED) && key[i].stateChanged)
                return true;
        }
    }
    return false;  // Не нажата
}

// Поиск клавиши в списке активных клавиш по символу
// Возвращает -1, если не найдено, или индекс в списке активных клавиш
int AlashKeypadMatrix::findInList(char keyChar) {
    for (byte i = 0; i < LIST_MAX; i++) {
        if (key[i].kchar == keyChar) {
            return i;
        }
    }
    return -1;
}

// Поиск клавиши в списке активных клавиш по коду
// Возвращает -1, если не найдено, или индекс в списке активных клавиш
int AlashKeypadMatrix::findInList(int keyCode) {
    for (byte i = 0; i < LIST_MAX; i++) {
        if (key[i].kcode == keyCode) {
            return i;
        }
    }
    return -1;
}

// Новое в версии 2.0
// Ожидание нажатия клавиши
char AlashKeypadMatrix::waitForKey() {
    char waitKey = NO_KEY;
    // Блокирует всё, ожидая нажатия клавиши
    while ((waitKey = getKey()) == NO_KEY);
    return waitKey;
}

// Функция для обратной совместимости
KeyState AlashKeypadMatrix::getState() {
    return key[0].kstate;
}

// Конечный пользователь может проверить любые изменения состояния перед тем,
// как решить, нужно ли обновлять какие-либо переменные и т.д. в их коде.
bool AlashKeypadMatrix::keyStateChanged() {
    return key[0].stateChanged;
}

// Количество клавиш в списке key[LIST_MAX] равно количеству
// байт в списке клавиш, делённому на количество байт в объекте Key.
byte AlashKeypadMatrix::numKeys() {
    return sizeof(key) / sizeof(Key);
}

// Минимальное значение debounceTime составляет 1 мс. Любое меньшее значение замедлит цикл().
void AlashKeypadMatrix::setDebounceTime(uint debounce) {
    debounce < 1 ? debounceTime = 1 : debounceTime = debounce;
}

// Установка времени удержания
void AlashKeypadMatrix::setHoldTime(uint hold) {
    holdTime = hold;
}

// Добавление слушателя событий для клавиатуры
void AlashKeypadMatrix::addEventListener(void (*listener)(char)) {
    keypadEventListener = listener;
}

// Переход между состояниями клавиши и вызов функции-слушателя событий
void AlashKeypadMatrix::transitionTo(byte idx, KeyState nextState) {
    // Установка следующего состояния и флага изменения состояния
    key[idx].kstate = nextState;
    key[idx].stateChanged = true;

    // Программа использовала функцию getKey().
    // Вызов keypadEventListener только при изменении состояния первой клавиши в слоте 0.
    if (single_key) {
        if ((keypadEventListener != NULL) && (idx == 0)) {
            keypadEventListener(key[0].kchar);
        }
    }
    // Программа использовала функцию getKeys().
    // Вызов keypadEventListener для любой клавиши, которая меняет состояние.
    else {
        if (keypadEventListener != NULL) {
            keypadEventListener(key[idx].kchar);
        }
    }
}
