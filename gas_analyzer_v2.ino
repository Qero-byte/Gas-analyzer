/*
 Пороговые значения задаются в переменных normal_level, warning_level, danger_level
 Показания датчика хранятся в переменной data
 Зеленый светодиод на пине 2
 Жёлтый светодиод на пине 3
 Красный светодиод на пине 4
*/

//Подключаем библиотеки для работы
#include <Wire.h>                          //Библиотека для работы с шиной I2C
#define _LCD_TYPE 1 
#include <LCD_1602_RUS_ALL.h>
#include <LiquidMenu.h>

//Обьявление констант и переменных
const long data = 1200;
const long warning_level = 500;
const long danger_level = 1000;
int control = 0;                           //Переменная для единократного звука при некритическом превышении порога
unsigned int myTimer;                      //Переменная таймера для работы с millis()
bool flag = true;                          //Флаг для переключения пищалки
String str_buffer, str;

//Различные обьявления и установки
LCD_1602_RUS lcd(0x27,16,2);          //Обьявление дисплея по адрессу 0x27 с 2 строками и 16 столбцами

void setup() {
  lcd.init();                              //Инициализация дисплея
  lcd.backlight();                         //Включение подсветки дисплея
  pinMode(2, OUTPUT);                      //Установка пина 2 как выход для зеленого светодиода
  pinMode(3, OUTPUT);                      //Установка пина 3 как выход для желтого светодиода
  pinMode(4, OUTPUT);                      //Установка пина 4 как выход для красного светодиода
  pinMode(13, OUTPUT);                     //Установка пина 13 как выход для пищалки
}

void loop() {
  /*
   * Главное условие программы;
   * В первой части рассматривается случай, когда значение меньше чем некритический порог
   * Во второй части рассматривается случай, когда значение больше чем некритический порог, но меньше чем критический
   * В третей части рассматривается случай, когда значение больше критический порог
   */
  if (data < warning_level) { 
    control = 0;                           //Сброс переменной control для правильной работы условия
    str = "OK";                            //Выводим OK
    digitalWrite(13, LOW);                 //Отключаем пищалку
    green();
  } else if (data >= warning_level && data < danger_level) {
    str = "Контроль";                      //Выводим Контроль
    if (control != 100) {                  //Условие для включения пищалки на 100 тактов
      digitalWrite(13, HIGH);              //Включаем пищалку
      control++;                           //Увеличиваем переменную счетчик на 1
    } else {
      digitalWrite(13, LOW);               //Выключаем пищалку через 100 тактов
    }
    yellow();
  } else if (data >= danger_level) {
    control = 0;                           //Сброс переменной control для правильной работы условия
    str = "Критический";                   //Выводим Критический
    if (millis() - myTimer >= 500) {       //Таймер на 500 мс (2 раза в сек)
      myTimer = millis();                  //Сброс таймера
      digitalWrite(13, flag);              //Включаем пищалку
      flag = !flag;
    }
    red();
  }
 if (str_buffer != str) {
    lcd.clear();
    lcd.print(str);
    str_buffer = str;
  }
}

void green() {
  digitalWrite(2, HIGH);                   //Включаем зеленый светодиод
  digitalWrite(3, LOW);                    //Отключаем жёлтый и красный светодиоды
  digitalWrite(4, LOW);
}

void yellow() {
  digitalWrite(2, LOW);                    //Отключаем зеленый светодиод
  digitalWrite(3, HIGH);                   //Включаем жёлтый светодиод
  digitalWrite(4, LOW);                    //Отключаем красный светодиод
}

void red() {
  digitalWrite(2, LOW);                    //Отключаем зелёный и жёлтый светодиоды
  digitalWrite(3, LOW); 
  digitalWrite(4, HIGH);                   //Включаем красный светодиод
}
