void Settings_CH() {
  uint8_t _i, _j;                       //сюда запишем координаты настраиваемой ячейки массива текущих настроек
  LCD.fillScreen(BLACK);
  LCD.setTextSize(1);
  LCD.setTextColor(MAGENTA);
  LCD.setCursor(0, 0);
  LCD.print ("Settings channel");
  LCD.setTextColor(BLUE);
  LCD.setCursor(0, 15);
  LCD.print ("SET:");                  //печатем
  // табличка вертикальные линии
  LCD.drawFastVLine(4, 27, 101, WHITE);
  LCD.drawFastVLine(33, 27, 101, WHITE);
  LCD.drawFastVLine(56, 27, 101, WHITE);
  LCD.drawFastVLine(79, 27, 101, WHITE);
  LCD.drawFastVLine(102, 27, 101, WHITE);
  // верхняя горизонтальная линия
  LCD.drawFastHLine(4, 27, 98, WHITE);
  // табличка горизонтальные линии и надпись CH
  LCD.setTextColor(CYAN);
  for (uint8_t i = 0; i < 10; i++) {
    LCD.drawFastHLine(4, 37 + 10 * i, 98, WHITE);
    LCD.setCursor(7, 29 + i * 10);
    LCD.print("CH");
  }
  first_frame = 0;                       //опускаем флаг отрисовки
  while (!KEY3.isHolded()) {             //пок не будет удержана кнопка 3    
    KEY3.tick();                         // постоянно проверяем третью кнопку
    ReadData();                          //опрашиваем кнопки
    PackForTX();                         //пакуем пакет

    if  (UP_KEY.clickBtn()) {            //если нажата кнопка вверх
      cur_y--;                           //двигаем курсор вверх
      if (cur_y < 0) {
        cur_y = 0;
        first_line--;                    //если доши до нуля двигаем все значения вверх
        if (first_line < 0) first_line = 0;
      }
      first_frame = 0;
    }

    if  (DOWN_KEY.clickBtn()) {
      cur_y++;
      if (cur_y > 9) {
        cur_y = 9;
        first_line++;
        if (first_line > 10) first_line = 10;
      }
      first_frame = 0;
    }

    if  (LEFT_KEY.clickBtn()) {
      cur_x--;
      if (cur_x < 0) cur_x = 0;
      first_frame = 0;
    }

    if  (RIGHT_KEY.clickBtn()) {
      cur_x++;
      if (cur_x > 2) cur_x = 2;
      first_frame = 0;
    }

    if  (WHITE_KEY.clickBtn()) {
      data.data_set [data.set][_i + first_line][_j]++;
      if (_j == 0) {
        if (data.data_set [data.set][_i + first_line][_j] > 19)data.data_set [data.set][_i + first_line][_j] = 0;
      }
      first_frame = 0;
    }

    if  (BLUE_KEY.clickBtn()) {
      if (_j == 0) {
        if (data.data_set [data.set][_i + first_line][_j] == 0) {
          data.data_set [data.set][_i + first_line][_j] = 19;
        } else data.data_set [data.set][_i + first_line][_j]--;
      } else data.data_set [data.set][_i + first_line][_j]--;
      first_frame = 0;
    }
    //значок батареи
    if (LCD_TIMER.isReady()) {
      if (digitalRead(STDBY_PIN)) DrawBat (analogRead(Bat_PIN), 109, 0);
      else {
        DrawBat (crg, 109, 0);
        crg = crg + 10;
        if (crg > 860) crg = 600;
      }
    }

    if (!first_frame) {
      first_frame = 1;
      //текущий пресет
      LCD.setTextColor(YELLOW);
      LCD.fillRect(25, 15, 45, 8, BLACK);
      LCD.setCursor(25, 15);
      LCD.print (SET_NAME[data.set]);
      // значения массива текущих настроек
      for (uint8_t i = 0; i < 10; i++) {
        uint16_t _y = 28 + i * 10;
        LCD.fillRect(18 , _y, 15, 9, BLACK);
        LCD.setTextColor(CYAN);
        if (i + 1 + first_line < 10) LCD.setCursor(23, _y + 1);
        else LCD.setCursor(19, _y + 1);
        LCD.print (i + 1 + first_line);     //номера каналов
        for (uint8_t j = 0; j < 3; j++) {
          uint16_t _x = 34 + j * 23;
          LCD.fillRect(_x , _y, 22, 9, BLACK);

          if ((28 + cur_y * 10 == _y) &&   (34 + cur_x * 23 == _x)) {    //если попали в квадрат курсором
            LCD.fillRect(34 + cur_x * 23, 28 + cur_y * 10, 22, 9, YELLOW); // на желтом фоне
            LCD.setTextColor(BLACK);                                       //пишем черным
            _i = i;      //запоминаем настраиваемую ячейку массива (строка)
            _j = j;      //запоминаем настраиваемую ячейку массива (столбец)
          } else LCD.setTextColor(CYAN);

          LCD.setCursor(_x + 2, _y + 1);
          if (j == 0)LCD.print (NAME_KEY[data.data_set [data.set][i + first_line][j]]);
          else LCD.print (data.data_set [data.set][i + first_line][j]);
        }
      }

    }
  }
  EEPROM.put(0, data);
  first_frame = 0;
}

void Settings_PWR() {
  bool flagBlink = 1;                           // флаг для мигания изменяемого параметра
  while (!KEY3.isHolded()) {                    // пока не будет удержана кнопка 2    
    KEY2.tick();                                // постоянно проверяем вторую кнопку
    KEY3.tick();                                // постоянно проверяем третью кнопку
    ReadData();
    PackForTX();
    if (LCD_TIMER.isReady())  Display1();
    if (BLINK_TIMER.isReady()) {
      LCD.fillRect(100, 15, 45, 8, BLACK);
      LCD.setCursor(100, 15);
      if (flagBlink) {
        LCD.setTextColor(YELLOW);
        LCD.print (SET_PWR[data.pwr]);
      } else {
        LCD.setTextColor(BLACK);
        LCD.print (SET_PWR[data.pwr]);
      }
      flagBlink = !flagBlink;
    }
    if (KEY2.isClick()) {
      data.pwr--;
      if (data.pwr < 0) data.pwr = 0;
    }
    if (KEY3.isClick()) {
      data.pwr++;
      if (data.pwr > 3) data.pwr = 3;
    }
  }
  EEPROM.put(0, data);
  RadioSetup(data.pwr);
  LCD.setTextColor(YELLOW);
  LCD.fillRect(100, 15, 45, 8, BLACK);
  LCD.setCursor(100, 15);
  LCD.print (SET_PWR[data.pwr]);
}


void Settings_Preset(uint8_t disp) {                        // выбор пресета
  bool flagBlink = 1;                           // флаг для мигания изменяемого параметра
  while (!KEY2.isHolded()) {                    // пока не будет удержана кнопка 2
    KEY2.tick();                                // постоянно проверяем вторую кнопку
    KEY3.tick();                                // постоянно проверяем третью кнопку
    ReadData();
    PackForTX();
    if (disp == 1 && LCD_TIMER.isReady()) {
      Display1();
      if (_cur_pwr != data.pwr || first_frame == 0) {
        _cur_pwr = data.pwr;
        LCD.setTextColor(YELLOW);
        LCD.fillRect(100, 15, 45, 8, BLACK);
        LCD.setCursor(100, 15);
        LCD.print (SET_PWR[data.pwr]);
      }
      first_frame = 1;
    }
    if (disp == 3 && LCD_TIMER.isReady())  {
      Display3();
      first_frame = 1;
    }

    if (BLINK_TIMER.isReady()) {
      LCD.fillRect(25, 15, 45, 8, BLACK);
      LCD.setCursor(25, 15);
      if (flagBlink) {
        LCD.setTextColor(YELLOW);
        LCD.print (SET_NAME[data.set]);
      } else {
        LCD.setTextColor(BLACK);
        LCD.print (SET_NAME[data.set]);
      }
      flagBlink = !flagBlink;
    }
    if (KEY2.isClick()) {
      data.set--;
      if (data.set < 0) data.set = 0;
      first_frame = 0;
    }
    if (KEY3.isClick()) {
      data.set++;
      if (data.set > 5) data.set = 5;
      first_frame = 0;
    }
  }
  EEPROM.put(0, data);
  LCD.setTextColor(YELLOW);
  LCD.fillRect(25, 15, 45, 8, BLACK);
  LCD.setCursor(25, 15);
  LCD.print (SET_NAME[data.set]);
  first_frame = 0;
}
