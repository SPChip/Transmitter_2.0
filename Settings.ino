void Settings_PWR() {
  bool flagBlink = 1;                           // флаг для мигания изменяемого параметра
  while (!KEY3.isHolded()) {                    // пока не будет удержана кнопка 2
    KEY1.tick();                                // постоянно проверяем первую кнопку
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
        LCD.print (SET_PWR[cur_pwr]);
      } else {
        LCD.setTextColor(BLACK);
        LCD.print (SET_PWR[cur_pwr]);
      }
      flagBlink = !flagBlink;
    }
    if (KEY2.isClick()) {
      cur_pwr--;
      if (cur_pwr < 0) cur_pwr = 0;
    }
    if (KEY3.isClick()) {
      cur_pwr++;
      if (cur_pwr > 3) cur_pwr = 3;
    }
  }
  RadioSetup(cur_pwr);
  LCD.setTextColor(YELLOW);
  LCD.fillRect(100, 15, 45, 8, BLACK);
  LCD.setCursor(100, 15);
  LCD.print (SET_PWR[cur_pwr]);

}






void Settings_Preset() {                        // выбор пресета
  bool flagBlink = 1;                           // флаг для мигания изменяемого параметра
  while (!KEY2.isHolded()) {                    // пока не будет удержана кнопка 2
    KEY1.tick();                                // постоянно проверяем первую кнопку
    KEY2.tick();                                // постоянно проверяем вторую кнопку
    KEY3.tick();                                // постоянно проверяем третью кнопку
    ReadData();
    PackForTX();
    if (LCD_TIMER.isReady())  Display1();
    if (BLINK_TIMER.isReady()) {
      LCD.fillRect(25, 15, 45, 8, BLACK);
      LCD.setCursor(25, 15);
      if (flagBlink) {
        LCD.setTextColor(YELLOW);
        LCD.print (SET_NAME[cur_set]);
      } else {
        LCD.setTextColor(BLACK);
        LCD.print (SET_NAME[cur_set]);
      }
      flagBlink = !flagBlink;
    }
    if (KEY2.isClick()) {
      cur_set--;
      if (cur_set < 0) cur_set = 0;
    }
    if (KEY3.isClick()) {
      cur_set++;
      if (cur_set > 3) cur_set = 3;
    }
  }
  LCD.setTextColor(YELLOW);
  LCD.fillRect(25, 15, 45, 8, BLACK);
  LCD.setCursor(25, 15);
  LCD.print (SET_NAME[cur_set]);
}
