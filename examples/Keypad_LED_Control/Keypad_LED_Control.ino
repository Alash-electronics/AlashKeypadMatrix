/* 
 *  Этот код управляет клавиатурой и светодиодом. При нажатии клавиши # светодиод переключается между включенным и выключенным состоянием. 
 * При удержании клавиши * светодиод начинает мигать, а при отпускании этой клавиши восстанавливается его состояние до мигания.
*/

#include <AlashKeypadMatrix.h>

const byte ROWS = 4; // четыре строки
const byte COLS = 3; // три столбца

// Определяем символы для клавиатуры
char keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

// Пины для подключения строк и столбцов клавиатуры
byte rowPins[ROWS] = {2, 3, 4, 5}; // подключение к строкам матричной клавиатуры
byte colPins[COLS] = {8, 9, 10}; // подключение к столбцам матричной клавиатуры

// Создаем объект для клавиатуры
AlashKeypadMatrix keypad = AlashKeypadMatrix(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
byte ledPin = 13; // Пин для светодиода

boolean isBlinking = false; // Флаг для мигания светодиода
boolean ledState; // Состояние светодиода

void setup(){
    Serial.begin(9600); // Инициализация последовательного соединения
    pinMode(ledPin, OUTPUT); // Настройка пина для светодиода как выход
    digitalWrite(ledPin, HIGH); // Включение светодиода
    ledState = digitalRead(ledPin); // Сохранение начального состояния светодиода
    keypad.addEventListener(keypadEvent); // Добавление обработчика событий для клавиатуры
}

void loop(){
    char key = keypad.getKey(); // Считывание нажатой клавиши

    if (key) {
        Serial.println(key); // Вывод нажатой клавиши в сериал
    }

    if (isBlinking){
        digitalWrite(ledPin, !digitalRead(ledPin)); // Изменение состояния светодиода (вкл/выкл)
        delay(100); // Задержка для мигания светодиода
    }
}

// Обработка специальных событий клавиатуры
void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
    case PRESSED: // Обработка нажатия клавиши
        if (key == '#') {
            digitalWrite(ledPin, !digitalRead(ledPin)); // Переключение состояния светодиода
            ledState = digitalRead(ledPin); // Сохранение состояния светодиода (вкл/выкл)
        }
        break;

    case RELEASED: // Обработка отпускания клавиши
        if (key == '*') {
            digitalWrite(ledPin, ledState); // Восстановление состояния светодиода до мигания
            isBlinking = false; // Остановка мигания светодиода
        }
        break;

    case HOLD: // Обработка удержания клавиши
        if (key == '*') {
            isBlinking = true; // Включение мигания светодиода при удержании клавиши *
        }
        break;
    }
}
