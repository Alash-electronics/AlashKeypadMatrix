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
byte colPins[COLS] = {8, 9, 10, 11}; // подключение к выводам столбцов клавиатуры

// инициализация экземпляра класса AlashKeypadMatrix
AlashKeypadMatrix customKeypad = AlashKeypadMatrix( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600); // инициализация последовательной связи
}
void loop(){
  char customKey = customKeypad.getKey(); // получение нажатой клавиши
  if (customKey){
    Serial.println(customKey); // вывод нажатой клавиши в последовательный порт
  }
}
