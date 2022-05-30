void Display_TestKey () {
  uint8_t x0 = 12, y0 = 23;  
  LCD.setTextColor(WHITE);
  LCD.setTextSize(2);
  LCD.setCursor(40, 2);
  LCD.print("TEST");
  LCD.setTextSize(1);
  //-------------J1X-----------------------
  LCD.setCursor(x0 + 0, y0 + 0);
  LCD.print("J1X");
  LCD.fillRect(x0 + 25, y0 + 0, 25, 8, BLACK);
  LCD.setCursor(x0 + 25, y0 + 0);
  LCD.print(analogRead(J1X_pin));
  //-------------J1Y-----------------------
  LCD.setCursor(x0 + 60, y0 + 0);
  LCD.print("J1Y");
  LCD.fillRect(x0 + 85, y0 + 0, 25, 8, BLACK);
  LCD.setCursor(x0 + 85, y0 + 0);
  LCD.print(analogRead(J1Y_pin));
  //-------------J2X-----------------------
  LCD.setCursor(x0 + 0, y0 + 10);
  LCD.print("J2X");
  LCD.fillRect(x0 + 25, y0 + 10, 25, 8, BLACK);
  LCD.setCursor(x0 + 25, y0 + 10);
  LCD.print(analogRead(J2X_pin));
  //-------------J2Y-----------------------
  LCD.setCursor(x0 + 60, y0 + 10);
  LCD.print("J2Y");
  LCD.fillRect(x0 + 85, y0 + 10, 25, 8, BLACK);
  LCD.setCursor(x0 + 85, y0 + 10);
  LCD.print(analogRead(J2Y_pin));
  //-------------RP1-----------------------
  LCD.setCursor(x0 + 0, y0 + 20);
  LCD.print("RP1");
  LCD.fillRect(x0 + 25, y0 + 20, 25, 8, BLACK);
  LCD.setCursor(x0 + 25, y0 + 20);
  LCD.print(analogRead(RP1_pin));
  //-------------RP2-----------------------
  LCD.setCursor(x0 + 60, y0 + 20);
  LCD.print("RP2");
  LCD.fillRect(x0 + 85, y0 + 20, 25, 8, BLACK);
  LCD.setCursor(x0 + 85, y0 + 20);
  LCD.print(analogRead(RP2_pin));
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
  (!bitRead(LEFT_BYTE, LEFT_BIT)) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("LEFT");
  //-------------UP----------------------
  LCD.setCursor(x0 + 28, y0 + 40);
  (!bitRead(UP_BYTE, UP_BIT)) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("UP");
  //-------------DOWN----------------------
  LCD.setCursor(x0 + 44, y0 + 40);
  (!bitRead(DOWN_BYTE, DOWN_BIT)) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("DOWN");
  //-------------RIGHT----------------------
  LCD.setCursor(x0 + 73, y0 + 40);
  (!bitRead(RIGHT_BYTE, RIGHT_BIT)) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("RIGHT");
  //-------------YELLOW----------------------
  LCD.setCursor(x0 + 0, y0 + 50);
  (!bitRead(YEL_BYTE, YEL_BIT)) ? LCD.setTextColor(BLACK, YELLOW) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("YEL");
  //-------------WHITE----------------------
  LCD.setCursor(x0 + 22, y0 + 50);
  (!bitRead(WHITE_BYTE, WHITE_BIT)) ? LCD.setTextColor(BLACK, WHITE) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("WHITE");
  //-------------BLUE----------------------
  LCD.setCursor(x0 + 57, y0 + 50);
  (!bitRead(BLUE_BYTE, BLUE_BIT)) ? LCD.setTextColor(BLACK, BLUE) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("BLUE");
  //-------------RED----------------------
  LCD.setCursor(x0 + 85, y0 + 50);
  (!bitRead(RED_BYTE, RED_BIT)) ? LCD.setTextColor(BLACK, RED) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("RED");
  //-------------J1KEY----------------------
  LCD.setCursor(x0 + 0, y0 + 60);
  (!bitRead(J1KEY_BYTE, J1KEY_BIT)) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("J1KEY");
  //-------------J2KEY----------------------
  LCD.setCursor(x0 + 72, y0 + 60);
  (!bitRead(J2KEY_BYTE, J2KEY_BIT)) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("J2KEY");
  //-------------SW1----------------------
  LCD.setCursor(x0 + 0, y0 + 70);
  (!bitRead(SW1_BYTE, SW1_BIT)) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("SW1");
  //-------------SW2----------------------
  LCD.setCursor(x0 + 84, y0 + 70);
  (!bitRead(SW2_BYTE, SW2_BIT)) ? LCD.setTextColor(BLACK, GREEN) : LCD.setTextColor(WHITE, BLACK);
  LCD.print("SW2");
  //-------------SW3----------------------
  LCD.setCursor(x0 + 0, y0 + 80);
  LCD.setTextColor(WHITE, BLACK);
  LCD.print("SW3");
  LCD.setCursor(x0 + 25, y0 + 80);
  if (!bitRead(SW3_1_BYTE, SW3_1_BIT))  LCD.print("0");
  if (bitRead(SW3_1_BYTE, SW3_1_BIT) && bitRead(SW3_2_BYTE, SW3_2_BIT)) LCD.print("1");
  if (!bitRead(SW3_2_BYTE, SW3_2_BIT)) LCD.print("2");
   //-------------SW4----------------------
  LCD.setCursor(x0 + 72, y0 + 80);
  LCD.setTextColor(WHITE, BLACK);
  LCD.print("SW4");
  LCD.setCursor(x0 + 96, y0 + 80);
  if (!bitRead(SW4_1_BYTE, SW4_1_BIT))  LCD.print("0");
  if (bitRead(SW4_1_BYTE, SW4_1_BIT) && bitRead(SW4_2_BYTE, SW4_2_BIT)) LCD.print("1");
  if (!bitRead(SW4_2_BYTE, SW4_2_BIT)) LCD.print("2");

  LCD.setTextColor(WHITE, BLACK);
}
