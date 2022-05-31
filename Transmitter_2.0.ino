//SPChip Transmitter 2.0

//--------------------- НАСТРОЙКИ ----------------------
#define INIT_ADDR 1023        // номер резервной ячейки для инициализации первого запуска
#define INIT_KEY 33           // ключ первого запуска. 0-254, на выбор, надо поменять на любое значение и будет как впервый раз
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
//#include <SPI.h>
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

//--------------------- ПЕРЕМЕННЫЕ ----------------------
uint16_t read_data [18];    // массив опроса кнопок и крутилок
uint8_t address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // возможные номера труб
uint16_t transmit_data[7];     // массив пересылаемых данных
uint16_t telemetry[2];          // массив принятых от приёмника данных телеметрии
uint8_t rssi;                 //
uint16_t trnsmtd_pack = 1, failed_pack; // переданные и потерянные пакеты
uint8_t set_default[18][3] = {          // настройка каналов по умолчанию
      {0, 0, 255},     //CH1 - J1X
      {1, 0, 255},     //CH2 - J1Y
      {2, 0, 255},     //CH3 - J2X
      {3, 0, 255},     //CH4 - J2Y
      {4, 0, 255},     //CH5 - RP1
      {5, 0, 255},     //CH5 - RP2
      {6, 0, 1},
      {7, 0, 1},
      {8, 0, 1},
      {9, 0, 1},
      {10, 0, 1},
      {11, 0, 1},
      {12, 0, 1},
      {13, 0, 1},
      {14, 0, 1},
      {15, 0, 1},
      {16, 0, 2},   
      {17, 0, 2},         
    };
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
  if (EEPROM.read(INIT_ADDR) != INIT_KEY) { // первый запуск
    EEPROM.write(INIT_ADDR, INIT_KEY);      // записали ключ    
    EEPROM.put(100, set_default);       // режим экрана 1
  }
}

void loop() {
  KEY1.tick();                                   // постоянно проверяем первую кнопку
  KEY2.tick();
  KEY3.tick();
  ReadData();                                    // опрашиваем все кнопки и крутилки
  if (KEY1.isClick()) digitalWrite(PinPower_PIN, LOW);    // проверка на один клик
  if (LCD_TIMER.isReady()) {
    Display_TestKey();
  }
  for (uint8_t i = 0; i < 18; i++) {
    Serial.print(read_data [i]);
    Serial.print("  ");
  }
  Serial.println("  ");

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





  //  Serial.print(String(transmit_data[0]) + "    " + String(transmit_data[1]) + "    " + String(transmit_data[2]) + "    " + String(transmit_data[3]) + "    " + String(transmit_data[4]) + "    " + String(transmit_data[5]) + "    " + String(transmit_data[6]));
  //  Serial.print("    ");
  //  Serial.println(telemetry[0] * 0.00488);
}

void ReadData() {
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
  if (!bitRead(SW3_1_BYTE, SW3_1_BIT))  read_data [16] = 0;                                       // SW3
  if (bitRead(SW3_1_BYTE, SW3_1_BIT) && bitRead(SW3_2_BYTE, SW3_2_BIT)) read_data [16] = 1;
  if (!bitRead(SW3_2_BYTE, SW3_2_BIT)) read_data [16] = 2;
  if (!bitRead(SW4_1_BYTE, SW4_1_BIT))  read_data [17] = 0;                                       // SW4
  if (bitRead(SW4_1_BYTE, SW4_1_BIT) && bitRead(SW4_2_BYTE, SW4_2_BIT)) read_data [17] = 1;
  if (!bitRead(SW4_2_BYTE, SW4_2_BIT)) read_data [17] = 2;
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
