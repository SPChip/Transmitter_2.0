//SPChip Transmitter 2.0

//--------------------- НАСТРОЙКИ ----------------------
#define INIT_ADDR 1023        // номер резервной ячейки в EEPROM для инициализации первого запуска
#define INIT_KEY 77           // ключ первого запуска. 0-99, на выбор, надо поменять на любое значение и будет как впервый раз
#define KEY1_PIN 11           // пин кнопки KEY1
#define KEY2_PIN 5            // пин кнопки KEY2
#define KEY3_PIN 13           // пин кнопки KEY3
#define PinPower_PIN 8        // пин удержания питания включенным
#define Bat_PIN A5            // пин контроля напряжения батареи
#define STDBY_PIN 7           // пин контроля подключения зарядки
#define CHRG_PIN A4           // пин контроля завершения зарядки
#define CS_PIN 4              // пин CS дисплея
#define DC_PIN 1              // пин DC дисплея
#define RES_PIN 0             // пин RES дисплея
#define CE_PIN 9              // пин CE радиомодуля
#define CSN_PIN 10            // пин CSN радиомодуля
#define J1X_pin A3            // пин ось X джойстик 1
#define J1Y_pin A2            // пин ось Y джойстик 1
#define J2X_pin A1            // пин ось X джойстик 2
#define J2Y_pin A0            // пин ось Y джойстик 2
#define RP1_pin A11           // пин крутилки 1
#define RP2_pin A7            // пин крутилки 2
#define CH_NUM 0x05           // номер канала радиомодуля (должен совпадать с приёмником)
#define SIG_POWER RF24_PA_LOW // УРОВЕНЬ МОЩНОСТИ ПЕРЕДАТЧИКА На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
#define SIG_SPEED RF24_1MBPS  // СКОРОСТЬ ОБМЕНА На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS должна быть одинакова на приёмнике и передатчике! при самой низкой скорости имеем самую высокую чувствительность и дальность!! ВНИМАНИЕ!!! enableAckPayload НЕ РАБОТАЕТ НА СКОРОСТИ 250 kbps!
#define BLACK 0x0000          // черный цвет для дисплея
#define WHITE 0xFFFF          // белый цвет для дисплея
#define RED 0xF800            // красный цвет для дисплея
#define GREEN 0x07E0          // зеленый цвет для дисплея
#define BLUE 0x001F           // синий цвет для дисплея
#define CYAN 0x07FF           // голубой цвет для дисплея
#define MAGENTA 0xF81F        // фиолетовый цвет для дисплея
#define YELLOW 0xFFE0         // желтый цвет для дисплея
#define ORANGE 0xFC00         // оранжевый цвет для дисплея
#define LEFT_BYTE hi_byte     // байт кнопки LEFT
#define LEFT_BIT 3            // бит кнопки LEFT
#define UP_BYTE hi_byte       // байт кнопки UP
#define UP_BIT 4              // бит кнопки UP
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
#include <Wire.h>             // Библиотека для работы с Wire для расширителя портов
#include <EEPROM.h>           // Библиотека для работы с EEPROM
#include "nRF24L01.h"         // Библиотека для радиомодуля NRF24 
#include "RF24.h"             // Библиотека для радиомодуля NRF24
#include "GyverButton.h"      // Библиотека для кнопок
#include <Adafruit_ST7735.h>  // Библиотека для дисплея ST7735
#include "GyverTimer.h"       // Библиотека для таймеров
//--------------------- БИБЛИОТЕКИ ----------------------

//--------------------- ОБЪЕКТЫ ----------------------
GButton KEY1(KEY1_PIN, LOW_PULL, NORM_OPEN);        // настраиваем кнопку 1
GButton KEY2(KEY2_PIN, HIGH_PULL, NORM_OPEN);       // настраиваем кнопку 2
GButton KEY3(KEY3_PIN, HIGH_PULL, NORM_OPEN);       // настраиваем кнопку 3
Adafruit_ST7735 LCD = Adafruit_ST7735(CS_PIN, DC_PIN, RES_PIN); // создем дисплей
RF24 RADIO(CE_PIN, CSN_PIN);                        // создаем радиомодуль на пинах 9 и 10
GTimer LCD_TIMER(MS, 100);                          // создаем таймер для отрисовки дисплея
GTimer BLINK_TIMER(MS, 250);                        // создаем таймер для мигания настройкой
GTimer RSSI_TIMER_RX1(MS, 2000);                    // создаем таймер для расчета RSSI приемника 1
GTimer RSSI_TIMER_RX2(MS, 2000);                    // создаем таймер для расчета RSSI приемника 2
GTimer RSSI_TIMER_RX3(MS, 2000);                    // создаем таймер для расчета RSSI приемника 3
GTimer RSSI_TIMER_RX4(MS, 2000);                    // создаем таймер для расчета RSSI приемника 4
GTimer RSSI_TIMER_RX5(MS, 2000);                    // создаем таймер для расчета RSSI приемника 5

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
char* SET_NAME[] = {"Default", "Lego", "Train", "Car", "User1", "User2"};    // названия текущей настройки
char* SET_PWR[] = {"Min", "Low", "High", "Max"};                             // названия уровней мощностей
char* NAME_KEY[] = {"J1X", "J1Y", "J2X", "J2Y", "RP1", "RP2", "LFT", "UP", "DWN", "RHT", "YEL", "WHT", "BLU", "RED", "J1K", "J2K", "SW1", "SW2", "SW3", "SW4"};// названия кнопок

//--------------------- КОНСТАНТЫ ----------------------

//--------------------- ПЕРЕМЕННЫЕ ----------------------
uint16_t read_data [20];                                   // массив опроса кнопок и крутилок
uint8_t address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // возможные номера труб
uint16_t transmit_data[10];                                // массив отправляемых данных
uint8_t buf[20], _buf[20];                                 // массив отправляемых данных до упаковки 20 байт в 10 байт
uint16_t telemetry[5], _telemetry[5];                      // массив принятых от приёмника данных телеметрии (до 5 штук), второй для отрисовки только в случае изменеия
bool rx_connect[5], _rx_connect[5];                        // массив контроля подключения приемника (до 5 штук), второй для отрисовки только в случае изменеия
uint8_t rssi[5], _rssi[5];                                 // массив RSSI приемников (до 5 штук), второй для отрисовки только в случае изменеия
uint16_t trnsmtd_pack[] ={1, 1, 1, 1, 1}, failed_pack[5];  // переданные и потерянные пакеты
bool first_frame = 0;                                      // флаг превой отрисовки дисплея (при переключении)
int8_t _cur_set = 0;                                       // значение текущей настройки для отрисовки только в случае изменения                                 
int8_t _cur_pwr = 0;                                       // значение текущей мощности передатчика для отрисовки только в случае изменения 
uint16_t crg = 600;                                        // минимальный уровень заряда батареи
int16_t first_line = 0, cur_y = 0, cur_x = 0;              // для отрисовки таблицы начальная линия отрисовки, положение курсора y, положение курсора x 
uint16_t u_tx;                                             // напряжение передатчика
struct  {                                                  // структура для хранения данных в eeprom
  uint8_t dysplayMode;                                     // режим экрана
  int8_t pwr;                                              // мощность передатчика
  int8_t set;                                              // текущий пресет
  uint8_t data_set [6][20][3];                             // массив с массивами пресетов
} data;                                                    // название структуры
 
//--------------------- ПЕРЕМЕННЫЕ ----------------------

//--------------------- КЛАССЫ ----------------------
class button {                                            //класс кнопки для настройки пресетов, используем не только key1 - key3, но и основные кнопки
  public:
    button (byte in) {   //номер из массива read_data []
      _in = in;
    }
    bool clickBtn() {
      bool btnState = read_data[_in];
      if (btnState && !_flag && millis() - _tmr >= 50) {
        _flag = true;
        _tmr = millis();
        return true;
      }
      if (btnState && _flag && millis() - _tmr >= 125) {
        _tmr = millis ();
        return true;
      }
      if (!btnState && _flag) {
        _flag = false;
        _tmr = millis();
      }
      return false;
    }
  private:
    byte _in;
    uint32_t _tmr;
    bool _flag;
};
//--------------------- КЛАССЫ ----------------------
button UP_KEY(7);     // указываем пин
button DOWN_KEY(8);   // указываем пин
button RIGHT_KEY(9);  // указываем пин
button LEFT_KEY(6);   // указываем пин
button WHITE_KEY(11); // указываем пин
button BLUE_KEY(12);  // указываем пин


void setup() {
  //Serial.begin(9600);
  pinMode(PinPower_PIN, OUTPUT);            // пин управления питанием
  pinMode(RP1_pin, INPUT);
  pinMode(RP2_pin, INPUT);
  pinMode(Bat_PIN, INPUT);
  pinMode(STDBY_PIN, INPUT_PULLUP);
  pinMode(CHRG_PIN, INPUT_PULLUP);
  KEY1.setTimeout(1500);                    // настраиваем таймаут для долгого нажатия кнопки 1
  KEY2.setTimeout(1500);
  KEY3.setTimeout(1500);
  digitalWrite(PinPower_PIN, HIGH);         // держим питание включенным
  LCD.initR(INITR_144GREENTAB);             // Init ST7735R chip, green tab
  Display_Logo ();                          // отрисовываем заставку
  delay(1000);                              // пауза
  LCD.fillScreen(BLACK);                    // очищаем экран
  delay(100);                               // пауза
  Wire.begin();                             //
  if (EEPROM.read(INIT_ADDR) != INIT_KEY) { // первый запуск
    EEPROM.write(INIT_ADDR, INIT_KEY);      // записали ключ
    data.dysplayMode = 1;                   // режим дисплея по умолчанию
    data.pwr = 0;                           // мощность передатчика по умолчанию
    data.set = 0;                           // пресет по умолчанию
    for (uint8_t i = 0; i < 6; i++) {       // для всех 6 пресетов
      for (uint8_t j = 0; j < 20; j++) {    
        for (uint8_t q = 0; q < 3; q++) {
          data.data_set [i][j][q] = set_default[j][q] ; // записываем пресет "заводской"          
        }
      }
    }
    EEPROM.put(0, data);                    // кладем настройку в eeprom
  }
  EEPROM.get(0, data);                      // читаем настройки каналов из eeprom
  RadioSetup(data.pwr);                     // инициализируем радиомодуль
}

void loop() {
  KEY1.tick();                                   // постоянно проверяем первую кнопку
  KEY2.tick();
  KEY3.tick();
  ReadData();                                    // опрашиваем все кнопки и крутилки
  PackForTX();                                   // запаковываем массив
  for (uint8_t i = 0; i < 5; i++) {
    Radio_TX_RX(i);                              // отправляем всем приемникам и получаем от них телеметрию
  }
  Display();                                     // отрисовываем дисплей
  if (KEY1.isHolded()) digitalWrite(PinPower_PIN, LOW);  // если удержать кнопку KEY1 отключаем питание
  if (KEY2.isClick()) {                                  // если кнопка 2 нажата переключаем режим отображения
    first_frame = 0;
    data.dysplayMode--;                                  // переходим к следующему режиму экрана
    if (data.dysplayMode < 1) {
      data.dysplayMode = 1;
      first_frame = 1;
    }
    EEPROM.put(0, data);                 
  }
  if (KEY3.isClick()) {                          // если кнопка 3 нажата переключаем режим отображения
    first_frame = 0;
    data.dysplayMode++;                               // переходим к следующему режиму экрана
    if (data.dysplayMode > 3) {
      data.dysplayMode = 3;        // максимум 3 экранов
      first_frame = 1;
    }
    EEPROM.put(0, data);
  }
  if (data.dysplayMode == 1 && KEY2.isHolded()) {
    Settings_Preset(1);
  }
  if (data.dysplayMode == 1 && KEY3.isHolded()) {
    Settings_PWR();
  }
  if (data.dysplayMode == 3 && KEY2.isHolded()) {
    Settings_Preset(3);
  }
  if (data.dysplayMode == 3 && KEY3.isHolded()) {
    Settings_CH();
  }
}


void PackForTX() {          // функция упаковки массива для отправки в соответствии с текущими настройками
  for (uint8_t i = 0; i < 20; i++) {
    if  ( data.data_set [data.set][i][0]   < 6 ) buf[i] = map(read_data [data.data_set [data.set][i][0]], 0, 1023, data.data_set [data.set][i][1], data.data_set [data.set][i][2]);
    else if (data.data_set [data.set][i][0] >= 6 && data.data_set [data.set][i][0] < 18 ) buf[i] = map(read_data [data.data_set [data.set][i][0]], 0, 1, data.data_set [data.set][i][1], data.data_set [data.set][i][2]);
    else buf[i] = map(read_data [data.data_set [data.set][i][0]], 0, 2, data.data_set [data.set][i][1], data.data_set [data.set][i][2]);
  }
  for (uint8_t i = 0; i < 10; i++) {          //упаковываем 20 byte в 10 int
    transmit_data[i] = (buf[i * 2] << 8) + buf[i * 2 + 1];    
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
