//SPChip Transmitter 2.0

//--------------------- НАСТРОЙКИ ----------------------
#define INIT_ADDR 1023        // номер резервной ячейки для инициализации первого запуска
#define INIT_KEY 55           // ключ первого запуска. 0-99, на выбор, надо поменять на любое значение и будет как впервый раз
#define KEY1_PIN 11           // кнопка KEY1 подключена сюда 
#define KEY2_PIN 5
#define KEY3_PIN 13
#define PinPower_PIN 8  
#define Bat_PIN A5
#define STDBY_PIN 7
#define CHRG_PIN A4
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
#define SIG_POWER RF24_PA_LOW // УРОВЕНЬ МОЩНОСТИ ПЕРЕДАТЧИКА На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
#define SIG_SPEED RF24_1MBPS  // СКОРОСТЬ ОБМЕНА На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS должна быть одинакова на приёмнике и передатчике! при самой низкой скорости имеем самую высокую чувствительность и дальность!! ВНИМАНИЕ!!! enableAckPayload НЕ РАБОТАЕТ НА СКОРОСТИ 250 kbps!
#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define ORANGE 0xFC00
#define LEFT_BYTE hi_byte
#define LEFT_BIT 3
#define UP_BYTE hi_byte
#define UP_BIT 4
#define DOWN_BYTE hi_byte
#define DOWN_BIT 2
#define RIGHT_BYTE hi_byte
#define RIGHT_BIT 1
#define YEL_BYTE lo_byte
#define YEL_BIT 5
#define WHITE_BYTE lo_byte
#define WHITE_BIT 3
#define BLUE_BYTE lo_byte
#define BLUE_BIT 6
#define RED_BYTE lo_byte
#define RED_BIT 4
#define J1KEY_BYTE hi_byte
#define J1KEY_BIT 0
#define J2KEY_BYTE lo_byte
#define J2KEY_BIT 7
#define SW1_BYTE hi_byte
#define SW1_BIT 7
#define SW2_BYTE lo_byte
#define SW2_BIT 0
#define SW3_1_BYTE hi_byte
#define SW3_1_BIT 6
#define SW3_2_BYTE hi_byte
#define SW3_2_BIT 5
#define SW4_1_BYTE lo_byte
#define SW4_1_BIT 2
#define SW4_2_BYTE lo_byte
#define SW4_2_BIT 1
//--------------------- НАСТРОЙКИ ----------------------

//--------------------- БИБЛИОТЕКИ ----------------------
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
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

//--------------------- КОНСТАНТЫ ----------------------
uint8_t set_default[20][3] = {          // настройка каналов по умолчанию
  {0, 0, 255},     //CH1 - J1X
  {1, 0, 255},     //CH2 - J1Y
  {2, 0, 255},     //CH3 - J2X
  {3, 0, 255},     //CH4 - J2Y
  {4, 0, 255},     //CH5 - RP1
  {5, 0, 255},     //CH5 - RP2
  {6, 0, 255},     //CH6 - LEFT
  {7, 0, 255},     //CH7 - UP
  {8, 0, 255},     //CH8 - DOWN
  {9, 0, 255},     //CH9 - RIGHT
  {10, 0, 255},     //CH10 - YELLOW
  {11, 0, 255},     //CH11 - WHITE
  {12, 0, 255},     //CH12 - BLUE
  {13, 0, 255},     //CH13 - RED
  {14, 0, 255},      //CH14 - J1KEY
  {15, 0, 255},      //CH15 - J2KEY
  {16, 0, 255},      //CH16 - SW1
  {17, 0, 255},      //CH17 - SW2
  {18, 0, 255},      //CH18 - SW3
  {19, 0, 255},      //CH19 - SW4
};
//--------------------- КОНСТАНТЫ ----------------------

//--------------------- ПЕРЕМЕННЫЕ ----------------------
uint16_t read_data [20];          // массив опроса кнопок и крутилок
uint8_t current_settings[20][3];   // массив с текущими настройками
uint8_t address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // возможные номера труб
uint16_t transmit_data[10];        // массив пересылаемых данных
uint8_t buf[20];
uint16_t telemetry[2];            // массив принятых от приёмника данных телеметрии
bool rx_connect[6];
//uint16_t rx2_telemetry[2];            // массив принятых от приёмника данных телеметрии
uint8_t rssi;                     //
uint16_t trnsmtd_pack = 1, failed_pack; // переданные и потерянные пакеты
bool first_frame = 0;
uint8_t dysplayMode = 3;
//--------------------- ПЕРЕМЕННЫЕ ----------------------



void setup() {
  Serial.begin(9600);
  pinMode(PinPower_PIN, OUTPUT);       // пин управления питанием
  pinMode(RP1_pin, INPUT);
  pinMode(RP2_pin, INPUT);
  pinMode(Bat_PIN, INPUT);
  pinMode(STDBY_PIN, INPUT_PULLUP);
  pinMode(CHRG_PIN, INPUT_PULLUP);
  digitalWrite(PinPower_PIN, HIGH);    // держим питание включенным
  LCD.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
  LCD.fillScreen(ST77XX_BLACK);
  Wire.begin();
  RadioSetup();
  if (EEPROM.read(INIT_ADDR) != INIT_KEY) { // первый запуск
    EEPROM.write(INIT_ADDR, INIT_KEY);      // записали ключ
    EEPROM.put(100, set_default);           // настройка каналов до заводских
  }
  EEPROM.get(100, current_settings);        // читаем настройки каналов из eeprom
}

void loop() {
  KEY1.tick();                                   // постоянно проверяем первую кнопку
  KEY2.tick();
  KEY3.tick();
  ReadData();                                    // опрашиваем все кнопки и крутилки
  PackForTX();
  Radio_TX_RX(0);
  Radio_TX_RX(1);
  Display();

  //Serial.print(telemetry[0]);
  //Serial.println(" | ");
  if (KEY1.isClick() || KEY1.isStep()) digitalWrite(PinPower_PIN, LOW);    // проверка на один клик

  if (KEY2.isClick() || KEY2.isStep()) {                          // если кнопка 2 нажата переключаем режим отображения
    dysplayMode--;                               // переходим к следующему режиму экрана
    if (dysplayMode < 1) dysplayMode = 1;        // максимум 3 экранов    
    first_frame =0;
  }
  if (KEY3.isClick() || KEY3.isStep()) {                          // если кнопка 2 нажата переключаем режим отображения
    dysplayMode++;                               // переходим к следующему режиму экрана
    if (dysplayMode > 3) dysplayMode = 3;        // максимум 3 экранов    
      first_frame =0;
  }

  
}


void PackForTX() {          // функция упаковки массива для отправки в соответствии с текущими настройками
  for (uint8_t i = 0; i < 20; i++) {
    if  ( current_settings[i][0]   < 6 ) buf[i] = map(read_data [current_settings[i][0]], 0, 1023, current_settings[i][1], current_settings[i][2]);
    else if (current_settings[i][0] >= 6 && current_settings[i][0] < 18 ) buf[i] = map(read_data [current_settings[i][0]], 0, 1, current_settings[i][1], current_settings[i][2]);
    else buf[i] = map(read_data [current_settings[i][0]], 0, 2, current_settings[i][1], current_settings[i][2]);
  }
  for (uint8_t i = 0; i < 10; i++) {          //упаковываем 20 byte в 10 int
    transmit_data[i] = (buf[i * 2] << 8) + buf[i * 2 + 1];
    //Serial.print(transmit_data [i], HEX);
    //Serial.print(" | ");
  }
}

void ReadData() {           // функция опроса всех кнопок, крутилок и джойстиков
  read_data [0] = analogRead(J1X_pin); // J1X
  read_data [1] = analogRead(J1Y_pin); // J1Y
  read_data [2] = analogRead(J2X_pin); // J2X
  read_data [3] = analogRead(J2Y_pin); // J2Y
  read_data [4] = analogRead(RP1_pin); // RP1
  read_data [5] = analogRead(RP2_pin); // RP2
  Wire.requestFrom(0x27, 2); // отправляем запрос в расширитель портов
  uint8_t lo_byte = Wire.read(); // Читаем младший байт (P00...P07)
  uint8_t hi_byte = Wire.read(); // Читаем старший байт (P10...P17)
  read_data [6] = !bitRead(LEFT_BYTE, LEFT_BIT);      // LEFT
  read_data [7] = !bitRead(UP_BYTE, UP_BIT);          // UP
  read_data [8] = !bitRead(DOWN_BYTE, DOWN_BIT);      // DOWN
  read_data [9] = !bitRead(RIGHT_BYTE, RIGHT_BIT);    // RIGHT
  read_data [10] = !bitRead(YEL_BYTE, YEL_BIT);       // YEL
  read_data [11] = !bitRead(WHITE_BYTE, WHITE_BIT);   // WHITE
  read_data [12] = !bitRead(BLUE_BYTE, BLUE_BIT);     // BLUE
  read_data [13] = !bitRead(RED_BYTE, RED_BIT);       // RED
  read_data [14] = !bitRead(J1KEY_BYTE, J1KEY_BIT);   // J1KEY
  read_data [15] = !bitRead(J2KEY_BYTE, J2KEY_BIT);   // J2KEY
  read_data [16] = !bitRead(SW1_BYTE, SW1_BIT);       // SW1
  read_data [17] = !bitRead(SW2_BYTE, SW2_BIT);       // SW2
  if (!bitRead(SW3_1_BYTE, SW3_1_BIT))  read_data [18] = 0;                                       // SW3
  if (bitRead(SW3_1_BYTE, SW3_1_BIT) && bitRead(SW3_2_BYTE, SW3_2_BIT)) read_data [18] = 1;
  if (!bitRead(SW3_2_BYTE, SW3_2_BIT)) read_data [18] = 2;
  if (!bitRead(SW4_1_BYTE, SW4_1_BIT))  read_data [19] = 0;                                       // SW4
  if (bitRead(SW4_1_BYTE, SW4_1_BIT) && bitRead(SW4_2_BYTE, SW4_2_BIT)) read_data [19] = 1;
  if (!bitRead(SW4_2_BYTE, SW4_2_BIT)) read_data [19] = 2;
}
