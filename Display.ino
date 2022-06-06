void Display() {
  Serial.println(dysplayMode);
  if (LCD_TIMER.isReady()) {
    switch (dysplayMode) {
      case 1:
        Display1 ();
        if (_cur_set != cur_set || first_frame == 0) {
          _cur_set = cur_set;
          LCD.setTextColor(YELLOW);
          LCD.fillRect(25, 15, 45, 8, BLACK);
          LCD.setCursor(25, 15);
          LCD.print (SET_NAME[cur_set]);
        }
        if (_cur_pwr != cur_pwr || first_frame == 0) {
          _cur_pwr = cur_pwr;
          LCD.setTextColor(YELLOW);
          LCD.fillRect(100, 15, 45, 8, BLACK);
          LCD.setCursor(100, 15);
          LCD.print (SET_PWR[cur_pwr]);
        }
        first_frame = 1;
        break;
      case 2:
        Display2();
        break;
      case 3:
        Display3();
        break;
      case 4:
        Display_TestKey ();
        break;
    }
  }
}

void Display1 () {
  uint8_t x0 = 0, y0 = 0;
  if (!first_frame) {
    LCD.fillScreen(BLACK);
    LCD.setTextColor(CYAN);
    LCD.setTextSize(1);
    LCD.drawFastVLine(x0 + 4, y0 + 27, 101, WHITE);
    LCD.drawFastVLine(x0 + 4 + 29, y0 + 27, 101, WHITE);
    LCD.drawFastVLine(x0 + 4 + 53, y0 + 27, 101, WHITE);
    LCD.drawFastHLine(x0 + 4, y0 + 27, 53, WHITE);
    for (uint8_t i = 0; i < 10; i++) {
      LCD.drawFastHLine(x0 + 4, y0 + 37 + 10 * i, 53, WHITE);
      LCD.setCursor(x0 + 7, y0 + 29 + 10 * i);
      LCD.print("CH");
      if (i < 9)  LCD.print(" ");
      LCD.print(i + 1);
    }

    LCD.drawFastVLine(x0 + 70, y0 + 27, 101, WHITE);
    LCD.drawFastVLine(x0 + 70 + 29, y0 + 27, 101, WHITE);
    LCD.drawFastVLine(x0 + 70 + 53, y0 + 27, 101, WHITE);
    LCD.drawFastHLine(x0 + 70, y0 + 27, 53, WHITE);
    for (uint8_t i = 10; i < 20; i++) {
      LCD.drawFastHLine(x0 + 70, y0 + 37 + 10 * (i - 10), 53, WHITE);
      LCD.setCursor(x0 + 73, y0 + 29 + 10 * (i - 10));
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
      LCD.setCursor(x0 + i * 21, y0);
      LCD.print("RX");
      LCD.print(i + 1);
    }
  }


  if (digitalRead(STDBY_PIN)) DrawBat (analogRead(Bat_PIN), 109, 0);
    else {
      static uint16_t crg;
      DrawBat (crg, 109, 0);
      crg=crg+40;
      if (crg > 1022) crg = 0;
    }

  LCD.setTextColor(ORANGE);
  for (uint8_t i = 0; i < 10; i++) {
    if (abs(_buf[i] - buf[i]) > 1 || first_frame == 0 ) {
      _buf[i] = buf[i];
      LCD.fillRect(x0 + 37, y0 + 29 + 10 * i, 20, 8, BLACK);
      LCD.setCursor(x0 + 37, y0 + 29 + 10 * i);
      LCD.print(buf[i]);
    }
  }
  for (uint8_t i = 10; i < 20; i++) {
    if (abs(_buf[i] - buf[i]) > 1 || first_frame == 0 ) {
      _buf[i] = buf[i];
      LCD.fillRect(x0 + 103, y0 + 29 + 10 * (i - 10), 20, 8, BLACK);
      LCD.setCursor(x0 + 103, y0 + 29 + 10 * (i - 10));
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
    first_frame = 1;
    LCD.fillScreen(BLACK);
  }
  DrawBat (read_data [0], 109, 0);
  DrawBat (read_data [2], 2, 0);
}

void DrawBat (uint16_t charge, uint8_t x0, uint8_t y0) {
  if (charge >= 768) {
    LCD.drawRect(x0, y0, 19, 7, GREEN);
    LCD.drawRect(x0 - 2, y0 + 2, 2, 3, GREEN);
    LCD.fillRect(x0 + 2, y0 + 2, 3, 3, GREEN);
    LCD.fillRect(x0 + 6, y0 + 2, 3, 3, GREEN);
    LCD.fillRect(x0 + 10, y0 + 2, 3, 3, GREEN);
    LCD.fillRect(x0 + 14, y0 + 2, 3, 3, GREEN);
  } else if (charge >= 512 && charge < 768 ) {
    LCD.drawRect(x0, y0, 19, 7, GREEN);
    LCD.drawRect(x0 - 2, y0 + 2, 2, 3, GREEN);
    LCD.fillRect(x0 + 2, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 6, y0 + 2, 3, 3, GREEN);
    LCD.fillRect(x0 + 10, y0 + 2, 3, 3, GREEN);
    LCD.fillRect(x0 + 14, y0 + 2, 3, 3, GREEN);
  } else if (charge >= 256 && charge < 512 ) {
    LCD.drawRect(x0, y0, 19, 7, YELLOW);
    LCD.drawRect(x0 - 2, y0 + 2, 2, 3, YELLOW);
    LCD.fillRect(x0 + 2, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 6, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 10, y0 + 2, 3, 3, YELLOW);
    LCD.fillRect(x0 + 14, y0 + 2, 3, 3, YELLOW);
  } else if (charge <256) {
    LCD.drawRect(x0, y0, 19, 7, RED);
    LCD.drawRect(x0 - 2, y0 + 2, 2, 3, RED);
    LCD.fillRect(x0 + 2, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 6, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 10, y0 + 2, 3, 3, BLACK);
    LCD.fillRect(x0 + 14, y0 + 2, 3, 3, RED);
  }
}




void Display_TestKey () {
  uint8_t x0 = 12, y0 = 23;
  if (!first_frame) {
    first_frame = 1;
    LCD.fillScreen(BLACK);
    LCD.setTextColor(WHITE);
    LCD.setTextSize(2);
    LCD.setCursor(40, 2);
    LCD.print("TEST");
    LCD.setTextSize(1);
    //-------------J1X-----------------------
    LCD.setCursor(x0 + 0, y0 + 0);
    LCD.print("J1X");
    //  -------------J1Y-----------------------
    LCD.setCursor(x0 + 60, y0 + 0);
    LCD.print("J1Y");
    //-------------J2X-----------------------
    LCD.setCursor(x0 + 0, y0 + 10);
    LCD.print("J2X");
    //-------------J2Y-----------------------
    LCD.setCursor(x0 + 60, y0 + 10);
    LCD.print("J2Y");
    //-------------RP1-----------------------
    LCD.setCursor(x0 + 0, y0 + 20);
    LCD.print("RP1");
    //-------------RP2-----------------------
    LCD.setCursor(x0 + 60, y0 + 20);
    LCD.print("RP2");
    //-------------SW3----------------------
    LCD.setCursor(x0 + 0, y0 + 80);
    LCD.setTextColor(WHITE, BLACK);
    LCD.print("SW3");
    //-------------SW4----------------------
    LCD.setCursor(x0 + 72, y0 + 80);
    LCD.setTextColor(WHITE, BLACK);
    LCD.print("SW4");
  }
  //-------------J1X-----------------------
  LCD.fillRect(x0 + 25, y0 + 0, 25, 8, BLACK);
  LCD.setCursor(x0 + 25, y0 + 0);
  LCD.print(read_data [0]);
  //  -------------J1Y-----------------------
  LCD.fillRect(x0 + 85, y0 + 0, 25, 8, BLACK);
  LCD.setCursor(x0 + 85, y0 + 0);
  LCD.print(read_data [1]);
  //-------------J2X-----------------------
  LCD.fillRect(x0 + 25, y0 + 10, 25, 8, BLACK);
  LCD.setCursor(x0 + 25, y0 + 10);
  LCD.print(read_data [2]);
  //-------------J2Y-----------------------
  LCD.fillRect(x0 + 85, y0 + 10, 25, 8, BLACK);
  LCD.setCursor(x0 + 85, y0 + 10);
  LCD.print(read_data [3]);
  //-------------RP1-----------------------
  LCD.fillRect(x0 + 25, y0 + 20, 25, 8, BLACK);
  LCD.setCursor(x0 + 25, y0 + 20);
  LCD.print(read_data [4]);
  //-------------RP2-----------------------
  LCD.fillRect(x0 + 85, y0 + 20, 25, 8, BLACK);
  LCD.setCursor(x0 + 85, y0 + 20);
  LCD.print(read_data [5]);
  //-------------KEY1----------------------
  LCD.setCursor(x0 + 0, y0 + 30);
  (KEY1.state()) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("KEY1");
  //-------------KEY2----------------------
  LCD.setCursor(x0 + 39, y0 + 30);
  (KEY2.state()) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("KEY2");
  //-------------KEY3----------------------
  LCD.setCursor(x0 + 79, y0 + 30);
  (KEY3.state()) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("KEY3");
  //-------------LEFT----------------------
  LCD.setCursor(x0 + 0, y0 + 40);
  (read_data [6]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("LEFT");
  //-------------UP----------------------
  LCD.setCursor(x0 + 28, y0 + 40);
  (read_data [7]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("UP");
  //-------------DOWN----------------------
  LCD.setCursor(x0 + 44, y0 + 40);
  (read_data [8]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("DOWN");
  //-------------RIGHT----------------------
  LCD.setCursor(x0 + 73, y0 + 40);
  (read_data [9]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("RIGHT");
  //-------------YELLOW----------------------
  LCD.setCursor(x0 + 0, y0 + 50);
  (read_data [10]) ? LCD.setTextColor(BLACK, YELLOW) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("YEL");
  //-------------WHITE----------------------
  LCD.setCursor(x0 + 22, y0 + 50);
  (read_data [11]) ? LCD.setTextColor(BLACK, WHITE) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("WHITE");
  //-------------BLUE----------------------
  LCD.setCursor(x0 + 57, y0 + 50);
  (read_data [12]) ? LCD.setTextColor(BLACK, BLUE) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("BLUE");
  //-------------RED----------------------
  LCD.setCursor(x0 + 85, y0 + 50);
  (read_data [13]) ? LCD.setTextColor(BLACK, RED) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("RED");
  //-------------J1KEY----------------------
  LCD.setCursor(x0 + 0, y0 + 60);
  (read_data [14]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("J1KEY");
  //-------------J2KEY----------------------
  LCD.setCursor(x0 + 72, y0 + 60);
  (read_data [15]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("J2KEY");
  //-------------SW1----------------------
  LCD.setCursor(x0 + 0, y0 + 70);
  (read_data [16]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("SW1");
  //-------------SW2----------------------
  LCD.setCursor(x0 + 84, y0 + 70);
  (read_data [17]) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("SW2");
  //-------------SW3----------------------
  LCD.setCursor(x0 + 25, y0 + 80);
  LCD.print(read_data [18]);
  //-------------SW4----------------------
  LCD.setCursor(x0 + 96, y0 + 80);
  LCD.print(read_data [19]);
  LCD.setTextColor(WHITE, BLACK);
}

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
