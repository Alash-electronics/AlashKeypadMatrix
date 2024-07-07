/*Библиотека AlashKeypadMatrix предоставляет инструменты для работы с матричными клавиатурами в Arduino. 
 * Она поддерживает определение пользовательских клавиатурных раскладок, обработку нескольких одновременных нажатий (multiKey), 
 * и предоставляет методы для обнаружения состояний клавиш (например, нажатие, удержание, отпускание). 
 * Это делает её подходящей для создания интерактивных интерфейсов и управления в Arduino проектах, где требуется обработка ввода с клавиатуры.
*/

#include <AlashKeypadMatrix.h>

const byte ROWS = 4; // четыре строки
const byte COLS = 4; // четыре столбца
// определение символов на кнопках клавиатуры
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; // подключение к выводам строк клавиатуры
byte colPins[COLS] = {8, 9, 10, 11}; // подключение к выводам столбцов 

// инициализация экземпляра класса AlashKeypadMatrix
AlashKeypadMatrix kpd = AlashKeypadMatrix( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

unsigned long loopCount;
unsigned long startTime;
String msg;

void setup() {
    Serial.begin(9600); // инициализация последовательной связи
    loopCount = 0;
    startTime = millis();
    msg = "";
}
void loop() {
    loopCount++;
    if ( (millis()-startTime)>5000 ) {
        Serial.print("Average loops per second = ");
        Serial.println(loopCount/5);
        startTime = millis();
        loopCount = 0;
    }
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Сканирование всего списка клавиш.
        {
            if ( kpd.key[i].stateChanged )   // Найти только те клавиши, состояние которых изменилось.
            {
                switch (kpd.key[i].kstate) {  // Сообщить о текущем состоянии активной клавиши: IDLE, PRESSED, HOLD или RELEASED
                    case PRESSED:
                    msg = " PRESSED.";          //PRESSED (нажатие): Клавиша была нажата. Это состояние активируется, когда клавиша только что была физически нажата, но еще не отпущена.
                break;
                    case HOLD:
                    msg = " HOLD.";             //HOLD (удержание): Клавиша удерживается нажатой. Это состояние активируется, когда клавиша была нажата и удерживается в течение определенного периода времени (например, для длительного нажатия).
                break;
                    case RELEASED:
                    msg = " RELEASED.";         //RELEASED (отпускание): Клавиша была отпущена после нажатия. Это состояние активируется, когда клавиша, которая была нажата, наконец отпускается.
                break;
                    case IDLE:
                    msg = " IDLE.";             //IDLE (ожидание): Клавиша находится в начальном состоянии ожидания. Это состояние активируется, когда клавиша не нажата и не удерживается.
                }
                Serial.print("Key "); // Вывод строки "Key "
                Serial.print(kpd.key[i].kchar); // Вывод символа нажатой клавиши
                Serial.println(msg); // Вывод сообщения о состоянии клавиши
            }
        }
    }
}  // Конец loop
