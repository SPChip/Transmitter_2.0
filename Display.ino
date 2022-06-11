void Display() {
  //Serial.println(dysplayMode);
  if (LCD_TIMER.isReady()) {
    switch (data.dysplayMode) {
      case 1:
        Display1 ();
        if (_cur_set != data.set || first_frame == 0) {
          _cur_set = data.set;
          LCD.setTextColor(YELLOW);
          LCD.fillRect(25, 15, 45, 8, BLACK);
          LCD.setCursor(25, 15);
          LCD.print (SET_NAME[data.set]);
        }
        if (_cur_pwr != data.pwr || first_frame == 0) {
          _cur_pwr = data.pwr;
          LCD.setTextColor(YELLOW);
          LCD.fillRect(100, 15, 45, 8, BLACK);
          LCD.setCursor(100, 15);
          LCD.print (SET_PWR[data.pwr]);
        }
        first_frame = 1;
        break;
      case 2:
        Display2();
        first_frame = 1;
        break;
      case 3:
        Display3();
        if (_cur_set != data.set || first_frame == 0) {
          _cur_set = data.set;
          LCD.setTextColor(YELLOW);
          LCD.fillRect(25, 15, 45, 8, BLACK);
          LCD.setCursor(25, 15);
          LCD.print (SET_NAME[data.set]);
        }
        first_frame = 1;
        break;
      case 4:
        //Display_TestKey ();
        first_frame = 1;
        break;
    }
  }
}

void Display1 () {
  if (!first_frame) {
    LCD.fillScreen(BLACK);
    LCD.setTextColor(CYAN);
    LCD.setTextSize(1);
    // табличка левая вертикальные линии
    LCD.drawFastVLine(4, 27, 101, WHITE);
    LCD.drawFastVLine(33, 27, 101, WHITE);
    LCD.drawFastVLine(57, 27, 101, WHITE);
    LCD.drawFastHLine(4, 27, 53, WHITE);
    // табличка первая горизонтальные линии и надпись CH
    for (uint8_t i = 0; i < 10; i++) {
      LCD.drawFastHLine(4, 37 + 10 * i, 53, WHITE);
      LCD.setCursor(7, 29 + 10 * i);
      LCD.print("CH");
      if (i < 9)  LCD.print(" ");
      LCD.print(i + 1);
    }

    LCD.drawFastVLine(70, 27, 101, WHITE);
    LCD.drawFastVLine(99, 27, 101, WHITE);
    LCD.drawFastVLine(123, 27, 101, WHITE);
    LCD.drawFastHLine(70, 27, 53, WHITE);
    for (uint8_t i = 10; i < 20; i++) {
      LCD.drawFastHLine(70, 37 + 10 * (i - 10), 53, WHITE);
      LCD.setCursor(73, 29 + 10 * (i - 10));
      LCD.print("CH");
      LCD.print(i + 1);
    }

    LCD.setTextColor(BLUE);
    LCD.setCursor(0, 15);
    LCD.print ("SET:");

    LCD.setCursor(75, 15);
    LCD.print ("PWR:");


  }
  for (uint8_t i = 0; i < 5; i++) {
    if (_rx_connect[i] != rx_connect[i] || first_frame == 0 ) {
      _rx_connect[i] = rx_connect[i];
      if (rx_connect[i]) LCD.setTextColor(GREEN);
      else LCD.setTextColor(RED);
      LCD.setCursor(i * 21, 0);
      LCD.print("RX");
      LCD.print(i + 1);
    }
  }


  if (digitalRead(STDBY_PIN)) DrawBat (analogRead(Bat_PIN), 109, 0);
  else {
    DrawBat (crg, 109, 0);
    crg = crg + 10;
    if (crg > 860) crg = 600;
  }

  LCD.setTextColor(ORANGE);
  for (uint8_t i = 0; i < 10; i++) {
    if (abs(_buf[i] - buf[i]) > 1 || first_frame == 0 ) {
      _buf[i] = buf[i];
      LCD.fillRect(37, 29 + 10 * i, 20, 8, BLACK);
      LCD.setCursor(37, 29 + 10 * i);
      LCD.print(buf[i]);
    }
  }
  for (uint8_t i = 10; i < 20; i++) {
    if (abs(_buf[i] - buf[i]) > 1 || first_frame == 0 ) {
      _buf[i] = buf[i];
      LCD.fillRect(103, 29 + 10 * (i - 10), 20, 8, BLACK);
      LCD.setCursor(103, 29 + 10 * (i - 10));
      LCD.print(buf[i]);
    }
  }

}



void Display2 () {
  if (!first_frame) {
    first_frame = 1;
    LCD.fillScreen(BLACK);
    LCD.setTextColor(WHITE);
    LCD.setTextSize(2);
    LCD.setCursor(40, 2);
    LCD.print("DISP 1");
    LCD.setTextSize(1);

  }
  LCD.fillRect(20, 40, 25, 8, BLACK);
  LCD.setCursor(20, 40);
  LCD.print(digitalRead(STDBY_PIN));
  LCD.fillRect(20, 60, 25, 8, BLACK);
  LCD.setCursor(20, 60);
  LCD.print(digitalRead(CHRG_PIN));
  LCD.setCursor(20, 80);
  LCD.fillRect(20, 80, 25, 8, BLACK);
  LCD.print(analogRead(Bat_PIN) * 0.00488);
  LCD.setCursor(20, 100);
  LCD.fillRect(20, 100, 25, 8, BLACK);
  LCD.print(rx_connect[0]);

}


void Display3 () {
  if (!first_frame) {
    LCD.fillScreen(BLACK);
    LCD.setTextSize(1);
    LCD.setTextColor(MAGENTA);
    LCD.setCursor(0, 0);
    LCD.print ("Settings channel");
    LCD.setTextColor(BLUE);
    LCD.setCursor(0, 15);
    LCD.print ("SET:");
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
  }
  //значок батареи
  if (digitalRead(STDBY_PIN)) DrawBat (analogRead(Bat_PIN), 109, 0);
  else {
    DrawBat (crg, 109, 0);
    crg = crg + 10;
    if (crg > 860) crg = 600;
  }


  if (!first_frame) {
    for (uint8_t i = 0; i < 20; i++) {
      uint16_t _y = 28 + i * 10;
      LCD.setTextColor(CYAN);
      if (i + 1 + first_line < 10) LCD.setCursor(23, _y + 1);
      else LCD.setCursor(19, _y + 1);
      LCD.print (i + 1 + first_line);     //номера каналов
      for (uint8_t j = 0; j < 3; j++) {
        uint16_t _x = 34 + j * 23;
        LCD.setCursor(_x + 2, _y + 1);
        if (j == 0)LCD.print (NAME_KEY[current_settings[i + first_line][j]]);
        else LCD.print (current_settings[i + first_line][j]);
      }
    }
  }
}



void DrawBat (uint16_t charge, uint8_t x0, uint8_t y0) {
  if (charge >= 791) {
    LCD.drawRect(x0, y0, 19, 7, GREEN);
    LCD.drawRect(x0 - 2, y0 + 2, 2, 3, GREEN);
    LCD.fillRect(x0 + 2, y0 + 2, 3, 3, GREEN);
    LCD.fillRect(x0 + 6, y0 + 2, 3, 3, GREEN);
    LCD.fillRect(x0 + 10, y0 + 2, 3, 3, GREEN);
    LCD.fillRect(x0 + 14, y0 + 2, 3, 3, GREEN);
  } else if (charge >= 723 && charge < 791 ) {
    LCD.drawRect(x0, y0, 19, 7, GREEN);
    LCD.drawRect(x0 - 2, y0 + 2, 2, 3, GREEN);
    LCD.fillRect(x0 + 2, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 6, y0 + 2, 3, 3, GREEN);
    LCD.fillRect(x0 + 10, y0 + 2, 3, 3, GREEN);
    LCD.fillRect(x0 + 14, y0 + 2, 3, 3, GREEN);
  } else if (charge >= 655 && charge < 723 ) {
    LCD.drawRect(x0, y0, 19, 7, YELLOW);
    LCD.drawRect(x0 - 2, y0 + 2, 2, 3, YELLOW);
    LCD.fillRect(x0 + 2, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 6, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 10, y0 + 2, 3, 3, YELLOW);
    LCD.fillRect(x0 + 14, y0 + 2, 3, 3, YELLOW);
  } else if (charge < 655) {
    LCD.drawRect(x0, y0, 19, 7, RED);
    LCD.drawRect(x0 - 2, y0 + 2, 2, 3, RED);
    LCD.fillRect(x0 + 2, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 6, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 10, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 14, y0 + 2, 3, 3, RED);
  }
}




//void Display_TestKey () {
//  uint8_t x0 = 12, y0 = 23;
//  if (!first_frame) {
//    first_frame = 1;
//    LCD.fillScreen(BLACK);
//    LCD.setTextColor(WHITE);
//    LCD.setTextSize(2);
//    LCD.setCursor(40, 2);
//    LCD.print("TEST");
//    LCD.setTextSize(1);
//    //-------------J1X-----------------------
//    LCD.setCursor(x0 + 0, y0 + 0);
//    LCD.print("J1X");
//    //  -------------J1Y-----------------------
//    LCD.setCursor(x0 + 60, y0 + 0);
//    LCD.print("J1Y");
//    //-------------J2X-----------------------
//    LCD.setCursor(x0 + 0, y0 + 10);
//    LCD.print("J2X");
//    //-------------J2Y-----------------------
//    LCD.setCursor(x0 + 60, y0 + 10);
//    LCD.print("J2Y");
//    //-------------RP1-----------------------
//    LCD.setCursor(x0 + 0, y0 + 20);
//    LCD.print("RP1");
//    //-------------RP2-----------------------
//    LCD.setCursor(x0 + 60, y0 + 20);
//    LCD.print("RP2");
//    //-------------SW3----------------------
//    LCD.setCursor(x0 + 0, y0 + 80);
//    LCD.setTextColor(WHITE, BLACK);
//    LCD.print("SW3");
//    //-------------SW4----------------------
//    LCD.setCursor(x0 + 72, y0 + 80);
//    LCD.setTextColor(WHITE, BLACK);
//    LCD.print("SW4");
//  }
//  //-------------J1X-----------------------
//  LCD.fillRect(x0 + 25, y0 + 0, 25, 8, BLACK);
//  LCD.setCursor(x0 + 25, y0 + 0);
//  LCD.print(read_data [0]);
//  //  -------------J1Y-----------------------
//  LCD.fillRect(x0 + 85, y0 + 0, 25, 8, BLACK);
//  LCD.setCursor(x0 + 85, y0 + 0);
//  LCD.print(read_data [1]);
//  //-------------J2X-----------------------
//  LCD.fillRect(x0 + 25, y0 + 10, 25, 8, BLACK);
//  LCD.setCursor(x0 + 25, y0 + 10);
//  LCD.print(read_data [2]);
//  //-------------J2Y-----------------------
//  LCD.fillRect(x0 + 85, y0 + 10, 25, 8, BLACK);
//  LCD.setCursor(x0 + 85, y0 + 10);
//  LCD.print(read_data [3]);
//  //-------------RP1-----------------------
//  LCD.fillRect(x0 + 25, y0 + 20, 25, 8, BLACK);
//  LCD.setCursor(x0 + 25, y0 + 20);
//  LCD.print(read_data [4]);
//  //-------------RP2-----------------------
//  LCD.fillRect(x0 + 85, y0 + 20, 25, 8, BLACK);
//  LCD.setCursor(x0 + 85, y0 + 20);
//  LCD.print(read_data [5]);
//  //-------------KEY1----------------------
//  LCD.setCursor(x0 + 0, y0 + 30);
//  (KEY1.state()) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("KEY1");
//  //-------------KEY2----------------------
//  LCD.setCursor(x0 + 39, y0 + 30);
//  (KEY2.state()) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("KEY2");
//  //-------------KEY3----------------------
//  LCD.setCursor(x0 + 79, y0 + 30);
//  (KEY3.state()) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("KEY3");
//  //-------------LEFT----------------------
//  LCD.setCursor(x0 + 0, y0 + 40);
//  (read_data [6]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("LEFT");
//  //-------------UP----------------------
//  LCD.setCursor(x0 + 28, y0 + 40);
//  (read_data [7]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("UP");
//  //-------------DOWN----------------------
//  LCD.setCursor(x0 + 44, y0 + 40);
//  (read_data [8]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("DOWN");
//  //-------------RIGHT----------------------
//  LCD.setCursor(x0 + 73, y0 + 40);
//  (read_data [9]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("RIGHT");
//  //-------------YELLOW----------------------
//  LCD.setCursor(x0 + 0, y0 + 50);
//  (read_data [10]) ? LCD.setTextColor(BLACK, YELLOW) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("YEL");
//  //-------------WHITE----------------------
//  LCD.setCursor(x0 + 22, y0 + 50);
//  (read_data [11]) ? LCD.setTextColor(BLACK, WHITE) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("WHITE");
//  //-------------BLUE----------------------
//  LCD.setCursor(x0 + 57, y0 + 50);
//  (read_data [12]) ? LCD.setTextColor(BLACK, BLUE) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("BLUE");
//  //-------------RED----------------------
//  LCD.setCursor(x0 + 85, y0 + 50);
//  (read_data [13]) ? LCD.setTextColor(BLACK, RED) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("RED");
//  //-------------J1KEY----------------------
//  LCD.setCursor(x0 + 0, y0 + 60);
//  (read_data [14]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("J1KEY");
//  //-------------J2KEY----------------------
//  LCD.setCursor(x0 + 72, y0 + 60);
//  (read_data [15]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("J2KEY");
//  //-------------SW1----------------------
//  LCD.setCursor(x0 + 0, y0 + 70);
//  (read_data [16]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("SW1");
//  //-------------SW2----------------------
//  LCD.setCursor(x0 + 84, y0 + 70);
//  (read_data [17]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
//  LCD.print("SW2");
//  //-------------SW3----------------------
//  LCD.setCursor(x0 + 25, y0 + 80);
//  LCD.print(read_data [18]);
//  //-------------SW4----------------------
//  LCD.setCursor(x0 + 96, y0 + 80);
//  LCD.print(read_data [19]);
//  LCD.setTextColor(WHITE, BLACK);
//}

void Display_Logo () {
  LCD.fillScreen(BLACK);
  LCD.setTextColor(WHITE);
  LCD.setTextSize(2);
  LCD.setCursor(0, 30);
  LCD.print("TRANSM");
  LCD.setCursor(70, 30);
  LCD.print("I");
  LCD.setCursor(80, 30);
  LCD.print("TTER");
  LCD.setTextSize(1);
  LCD.setTextColor(BLUE);
  LCD.setCursor(37, 50);
  LCD.print("by SPChip");
  LCD.setCursor(41, 70);
  LCD.setTextColor(RED);
  LCD.print("ver 2.0");
}
