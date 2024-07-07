/*Этот код управляет матричной клавиатурой, позволяя переключаться между буквенным и цифровым режимами. 
 * В зависимости от текущего режима, код обрабатывает нажатия клавиш, мигает светодиодом и выводит введенные символы на последовательный порт. 
 * При длительном нажатии клавиши `#` происходит переключение между режимами. 
 * Клавиши могут использоваться для построения строки, которая также выводится на последовательный порт.
*/
#include <AlashKeypadMatrix.h>
#include <ctype.h>

const byte ROWS = 4; // четыре строки
const byte COLS = 3; // три столбца

// Определение карт клавиш. Пустое место (внизу слева) - это пробел.
char alphaKeys[ROWS][COLS] = {
    { 'a','d','g' },
    { 'j','m','p' },
    { 's','v','y' },
    { ' ','.','#' }
};

char numberKeys[ROWS][COLS] = {
    { '1','2','3' },
    { '4','5','6' },
    { '7','8','9' },
    { ' ','0','#' }
};

boolean alpha = false; // Начинаем с цифровой клавиатуры.

byte rowPins[ROWS] = {2, 3, 4, 5}; // подключение к строкам матричной клавиатуры
byte colPins[COLS] = {8, 9, 10}; // подключение к столбцам матричной клавиатуры

// Создаем две новые клавиатуры: одну цифровую и одну буквенную.
AlashKeypadMatrix numpad(makeKeymap(numberKeys), rowPins, colPins, sizeof(rowPins), sizeof(colPins));
AlashKeypadMatrix ltrpad(makeKeymap(alphaKeys), rowPins, colPins, sizeof(rowPins), sizeof(colPins));

unsigned long startTime; // время старта
const byte ledPin = 13; // используем светодиод на пине 13

void setup() {
    Serial.begin(9600); // инициализация последовательного соединения
    pinMode(ledPin, OUTPUT); // настройка пина 13 как выход
    digitalWrite(ledPin, LOW); // выключение светодиода
    ltrpad.begin(makeKeymap(alphaKeys)); // инициализация буквенной клавиатуры
    numpad.begin(makeKeymap(numberKeys)); // инициализация цифровой клавиатуры
    ltrpad.addEventListener(keypadEvent_ltr); // добавление обработчика событий для буквенной клавиатуры
    ltrpad.setHoldTime(500); // установка времени удержания для буквенной клавиатуры (по умолчанию 1000 мс)
    numpad.addEventListener(keypadEvent_num); // добавление обработчика событий для цифровой клавиатуры
    numpad.setHoldTime(500); // установка времени удержания для цифровой клавиатуры (по умолчанию 1000 мс)
}

char key; // переменная для хранения нажатой клавиши

void loop() {
    if (alpha)
        key = ltrpad.getKey(); // получение клавиши с буквенной клавиатуры
    else
        key = numpad.getKey(); // получение клавиши с цифровой клавиатуры

    if (alpha && millis() - startTime > 100) { // мигание светодиода, если используется буквенная клавиатура
        digitalWrite(ledPin, !digitalRead(ledPin)); // инвертирование состояния светодиода
        startTime = millis(); // обновление времени старта
    }
}

static char virtKey = NO_KEY; // хранит последнее виртуальное нажатие клавиши (только для буквенных клавиш)
static char physKey = NO_KEY; // хранит последнее физическое нажатие клавиши (только для буквенных клавиш)
static char buildStr[12]; // строка для построения вводимого текста
static byte buildCount; // счетчик для построения строки
static byte pressCount; // счетчик нажатий одной и той же клавиши

static byte kpadState; // состояние клавиатуры

// Обработка специальных событий

void keypadEvent_ltr(KeypadEvent key) {
    // обработка событий в буквенном режиме
    kpadState = ltrpad.getState(); // получение состояния клавиатуры
    swOnState(key); // вызов функции обработки состояния
}

void keypadEvent_num(KeypadEvent key) {
    // обработка событий в цифровом режиме
    kpadState = numpad.getState(); // получение состояния клавиатуры
    swOnState(key); // вызов функции обработки состояния
}

void swOnState(char key) {
    switch (kpadState) {
        case PRESSED:
            if (isalpha(key)) { // проверка, является ли клавиша буквой
                if (physKey != key) { // новая клавиша, начинаем с первого символа из трех
                    pressCount = 0; // сброс счетчика нажатий
                    virtKey = key; // установка виртуальной клавиши
                    physKey = key; // установка физической клавиши
                }
                else { // нажата та же клавиша снова
                    virtKey++; // выбираем следующий символ на этой клавише
                    pressCount++; // увеличение счетчика нажатий
                }
                if (pressCount > 2) { // достигнут последний символ, возвращаемся к началу
                    pressCount = 0; // сброс счетчика нажатий
                    virtKey = key; // установка виртуальной клавиши
                }
                Serial.print(virtKey); // вывод виртуальной клавиши для тестирования
            }
            if (isdigit(key) || key == ' ' || key == '.') // проверка, является ли клавиша цифрой, пробелом или точкой
                Serial.print(key); // вывод клавиши
            if (key == '#') // проверка, является ли клавиша #
                Serial.println(); // переход на новую строку
            break;

        case HOLD:
            if (key == '#') { // переключение между картами клавиш при удержании #
                if (alpha == true) { // если используется буквенная карта клавиш
                    alpha = false; // переключение на цифровую карту клавиш
                    digitalWrite(ledPin, LOW); // выключение светодиода
                }
                else { // если используется цифровая карта клавиш
                    alpha = true; // переключение на буквенную карту клавиш
                }
            }
            else { // если нажата другая клавиша
                buildStr[buildCount++] = (isalpha(key)) ? virtKey : key; // добавление символа в строку
                buildStr[buildCount] = '\0'; // завершение строки
                Serial.println(); // переход на новую строку
                Serial.println(buildStr); // вывод построенной строки
            }
            break;

        case RELEASED:
            if (buildCount >= sizeof(buildStr)) buildCount = 0; // если строка заполнена, начинаем сначала
            break;
    } // конец switch-case
} // конец функции обработки состояния