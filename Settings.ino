void Settings_Display1() {
  while (!KEY2.isHolded()) {
    KEY1.tick();                                   // постоянно проверяем первую кнопку
    KEY2.tick();
    KEY3.tick();
    ReadData();
    PackForTX();
    Display1 ();
    LCD.setTextColor(YELLOW);
    LCD.fillRect(31, 15, 40, 8, BLACK);
    LCD.setCursor(31, 15);
    LCD.print (SET_NAME[cur_set]);
    if (KEY2.isClick()) {
      cur_set--;
      if (cur_set < 1) cur_set = 1;
    }
    if (KEY3.isClick()) {
      cur_set++;
      if (cur_set > 4) dysplayMode = 4;
    }
  }
}
