//SPChip Transmitter 2.0

//--------------------- НАСТРОЙКИ ----------------------
#define KEY1_PIN 11            // кнопка KEY1 подключена сюда 
#define KEY2_PIN 5
#define KEY3_PIN 13
#define PinPower_PIN 8            // кнопка KEY1 подключена сюда 
#define CS_PIN 4
#define DC_PIN 1
#define RES_PIN 0
#define CE_PIN 9
#define CSN_PIN 10
#define J1X_pin A3
#define J1Y_pin A2
#define J2X_pin A1
#define J2Y_pin A0
#define RP1_pin A11
#define RP2_pin A7
#define CH_NUM 0x7a           // номер канала (должен совпадать с приёмником)
#define SIG_POWER RF24_PA_HIGH // УРОВЕНЬ МОЩНОСТИ ПЕРЕДАТЧИКА На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
#define SIG_SPEED RF24_1MBPS  // СКОРОСТЬ ОБМЕНА На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS должна быть одинакова на приёмнике и передатчике! при самой низкой скорости имеем самую высокую чувствительность и дальность!! ВНИМАНИЕ!!! enableAckPayload НЕ РАБОТАЕТ НА СКОРОСТИ 250 kbps!

//--------------------- НАСТРОЙКИ ----------------------

//--------------------- БИБЛИОТЕКИ ----------------------
//#include <SPI.h>
#include <Wire.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "GyverButton.h"      // Библиотека для кнопок
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "GyverTimer.h"       // Библиотека для таймеров
//--------------------- БИБЛИОТЕКИ ----------------------

//--------------------- ОБЪЕКТЫ ----------------------
GButton KEY1(KEY1_PIN, LOW_PULL, NORM_OPEN);       // настраиваем кнопку 1
GButton KEY2(KEY2_PIN, HIGH_PULL, NORM_OPEN);       // настраиваем кнопку 1
GButton KEY3(KEY3_PIN, HIGH_PULL, NORM_OPEN);       // настраиваем кнопку 1

Adafruit_ST7735 LCD = Adafruit_ST7735(CS_PIN, DC_PIN, RES_PIN);
RF24 RADIO(CE_PIN, CSN_PIN);                       // "создать" модуль на пинах 9 и 10
GTimer LCD_TIMER(MS, 100);                        // создаем таймер для отрисовки дисплея
//--------------------- ОБЪЕКТЫ ----------------------

//--------------------- ПЕРЕМЕННЫЕ ----------------------
uint16_t t = 0;
uint8_t hi, lo;

uint8_t address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // возможные номера труб
uint16_t transmit_data[7];     // массив пересылаемых данных
uint16_t telemetry[2];          // массив принятых от приёмника данных телеметрии
uint8_t rssi;                 //
uint16_t trnsmtd_pack = 1, failed_pack; // переданные и потерянные пакеты
//--------------------- ПЕРЕМЕННЫЕ ----------------------


void setup() {
  Serial.begin(9600);
  pinMode(PinPower_PIN, OUTPUT);       // пин управления питанием
  pinMode(RP1_pin, INPUT); 
  pinMode(RP2_pin, INPUT);
  digitalWrite(PinPower_PIN, HIGH);    // держим питание включенным
  LCD.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
  LCD.fillScreen(ST77XX_BLACK);
  Wire.begin();
  RadioSetup();
}

void loop() {
  transmit_data[0] = 0x55;
  transmit_data[1] = map(analogRead(J1Y_pin), 0, 1023, 0, 255);         // считываем показания потенциометра с вывода A1 и записываем их в 2 элемент массива data
  transmit_data[4] = analogRead(J2Y_pin);

  // отправка пакета transmit_data
  if (RADIO.write(&transmit_data, sizeof(transmit_data))) {
    trnsmtd_pack++;
    if (!RADIO.available()) {   // если получаем пустой ответ
    } else {
      while (RADIO.available() ) {                    // если в ответе что-то есть
        RADIO.read(&telemetry, sizeof(telemetry));    // читаем
        // получили забитый данными массив telemetry ответа от приёмника
      }
    }
  } else {
    failed_pack++;
  }


  KEY1.tick();                                   // постоянно проверяем первую кнопку
  KEY2.tick();
  KEY3.tick();
  if (KEY1.isClick()) digitalWrite(PinPower_PIN, LOW);    // проверка на один клик
  if (LCD_TIMER.isReady()) {

    Display_TestKey();
  }
  Wire.requestFrom(0x27, 2);
  lo = Wire.read(); // Читаем младший байт (P00...P07)
  hi = Wire.read(); // Читаем старший байт (P10...P17)
  if (bitRead(hi, 0)) Serial.print(1);
  if (!bitRead(hi, 0)) Serial.print(0);
  if (bitRead(hi, 1)) Serial.print(1);
  if (!bitRead(hi, 1)) Serial.print(0);
  if (bitRead(hi, 2)) Serial.print(1);
  if (!bitRead(hi, 2)) Serial.print(0);
  if (bitRead(hi, 3)) Serial.print(1);
  if (!bitRead(hi, 3)) Serial.print(0);
  if (bitRead(hi, 4)) Serial.print(1);
  if (!bitRead(hi, 4)) Serial.print(0);
  if (bitRead(hi, 5)) Serial.print(1);
  if (!bitRead(hi, 5)) Serial.print(0);
  if (bitRead(hi, 6)) Serial.print(1);
  if (!bitRead(hi, 6)) Serial.print(0);
  if (bitRead(hi, 7)) Serial.print(1);
  if (!bitRead(hi, 7)) Serial.print(0);
  if (bitRead(lo, 0)) Serial.print(1);
  if (!bitRead(lo, 0)) Serial.print(0);
  if (bitRead(lo, 1)) Serial.print(1);
  if (!bitRead(lo, 1)) Serial.print(0);
  if (bitRead(lo, 2)) Serial.print(1);
  if (!bitRead(lo, 2)) Serial.print(0);
  if (bitRead(lo, 3)) Serial.print(1);
  if (!bitRead(lo, 3)) Serial.print(0);
  if (bitRead(lo, 4)) Serial.print(1);
  if (!bitRead(lo, 4)) Serial.print(0);
  if (bitRead(lo, 5)) Serial.print(1);
  if (!bitRead(lo, 5)) Serial.print(0);
  if (bitRead(lo, 6)) Serial.print(1);
  if (!bitRead(lo, 6)) Serial.print(0);
  if (bitRead(lo, 7)) Serial.print(1);
  if (!bitRead(lo, 7)) Serial.print(0);
  Serial.print("  ");
  Serial.print(analogRead(J1X_pin));
  Serial.print("  ");
  Serial.print(analogRead(J1Y_pin));
  Serial.print("  ");
  Serial.print(analogRead(J2X_pin));
  Serial.print("  ");
  Serial.print(analogRead(J2Y_pin));
  Serial.print("  ");
  Serial.print(String(transmit_data[0]) + "    " + String(transmit_data[1]) + "    " + String(transmit_data[2]) + "    " + String(transmit_data[3]) + "    " + String(transmit_data[4]) + "    " + String(transmit_data[5]) + "    " + String(transmit_data[6]));
  Serial.print("    ");
  Serial.println(telemetry[0] * 0.00488);
}

void Display_TestKey () {
  uint8_t x0 = 0, y0 = 10;
  LCD.setTextColor(ST77XX_WHITE);
  LCD.setTextSize(1);
  LCD.setCursor(x0 + 0, y0 + 0);
  LCD.print("J1X");
  LCD.fillRect(x0 + 25, y0 + 0, 25, 8, ST77XX_BLACK);
  LCD.setCursor(x0 + 25, y0 + 0);
  LCD.print(analogRead(J1X_pin));
  LCD.setCursor(x0 + 60, y0 + 0);
  LCD.print("J1Y");
  LCD.fillRect(x0 + 85, y0 + 0, 25, 8, ST77XX_BLACK);
  LCD.setCursor(x0 + 85, y0 + 0);
  LCD.print(analogRead(J1Y_pin));

  
  
  LCD.setCursor(x0 + 0, y0 + 10);
  LCD.print("J2X");
  LCD.fillRect(x0 + 25, y0 + 10, 25, 8, ST77XX_BLACK);
  LCD.setCursor(x0 + 25, y0 + 10);
  LCD.print(analogRead(J2X_pin));
  LCD.setCursor(x0 + 60, y0 + 10);
  LCD.print("J2Y");
  LCD.fillRect(x0 + 85, y0 + 10, 25, 8, ST77XX_BLACK);
  LCD.setCursor(x0 + 85, y0 + 10);
  LCD.print(analogRead(J2Y_pin));

  LCD.setCursor(x0 + 0, y0 + 20);
  LCD.print("RP1");
  LCD.fillRect(x0 + 25, y0 + 20, 25, 8, ST77XX_BLACK);
  LCD.setCursor(x0 + 25, y0 + 20);
  LCD.print(analogRead(RP1_pin));
  LCD.setCursor(x0 + 60, y0 + 20);
  LCD.print("RP2");
  LCD.fillRect(x0 + 85, y0 + 20, 25, 8, ST77XX_BLACK);
  LCD.setCursor(x0 + 85, y0 + 20);
  LCD.print(analogRead(RP2_pin));



  LCD.setCursor(x0 + 0, y0 + 30);
  if (KEY1.state()) LCD.setTextColor(ST77XX_GREEN);
  LCD.print("KEY1");
  LCD.setTextColor(ST77XX_WHITE);
  LCD.setCursor(x0 + 40, y0 + 30);
  if (KEY2.state()) LCD.setTextColor(ST77XX_GREEN);
  LCD.print("KEY2");
  LCD.setTextColor(ST77XX_WHITE);
  LCD.setCursor(x0 + 80, y0 + 30);
  if (KEY3.state()) LCD.setTextColor(ST77XX_GREEN);
  LCD.print("KEY3");
  LCD.setTextColor(ST77XX_WHITE);


}

void RadioSetup() {
  RADIO.begin();              // активировать модуль
  RADIO.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  RADIO.setRetries(0, 5);    // (время между попыткой достучаться, число попыток)
  RADIO.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  RADIO.setPayloadSize(32);   // размер пакета, в байтах
  RADIO.openWritingPipe(address[0]);   // мы - труба 0, открываем канал для передачи данных
  RADIO.setChannel(CH_NUM);            // выбираем канал (в котором нет шумов!)
  RADIO.setPALevel(SIG_POWER);         // уровень мощности передатчика
  RADIO.setDataRate(SIG_SPEED);        // скорость обмена
  // должна быть одинакова на приёмнике и передатчике!
  // при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  RADIO.powerUp();         // начать работу
  RADIO.stopListening();   // не слушаем радиоэфир, мы передатчик
}
