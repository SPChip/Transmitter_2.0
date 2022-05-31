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
  LCD.print(read_data [0]);  
//  -------------J1Y-----------------------
  LCD.setCursor(x0 + 60, y0 + 0);
  LCD.print("J1Y");
  LCD.fillRect(x0 + 85, y0 + 0, 25, 8, BLACK);
  LCD.setCursor(x0 + 85, y0 + 0);
  LCD.print(read_data [1]);
  //-------------J2X-----------------------
  LCD.setCursor(x0 + 0, y0 + 10);
  LCD.print("J2X");
  LCD.fillRect(x0 + 25, y0 + 10, 25, 8, BLACK);
  LCD.setCursor(x0 + 25, y0 + 10);
  LCD.print(read_data [2]);
  //-------------J2Y-----------------------
  LCD.setCursor(x0 + 60, y0 + 10);
  LCD.print("J2Y");
  LCD.fillRect(x0 + 85, y0 + 10, 25, 8, BLACK);
  LCD.setCursor(x0 + 85, y0 + 10);
  LCD.print(read_data [3]);
  //-------------RP1-----------------------
  LCD.setCursor(x0 + 0, y0 + 20);
  LCD.print("RP1");
  LCD.fillRect(x0 + 25, y0 + 20, 25, 8, BLACK);
  LCD.setCursor(x0 + 25, y0 + 20);
  LCD.print(read_data [4]);
  //-------------RP2-----------------------
  LCD.setCursor(x0 + 60, y0 + 20);
  LCD.print("RP2");
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
//  -------------KEY3----------------------
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
//  //-------------SW3----------------------
//  LCD.setCursor(x0 + 0, y0 + 80);
//  LCD.setTextColor(WHITE, BLACK);
//  LCD.print("SW3");
//  LCD.setCursor(x0 + 25, y0 + 80);
//  if (!bitRead(SW3_1_BYTE, SW3_1_BIT))  LCD.print("0");
//  if (bitRead(SW3_1_BYTE, SW3_1_BIT) && bitRead(SW3_2_BYTE, SW3_2_BIT)) LCD.print("1");
//  if (!bitRead(SW3_2_BYTE, SW3_2_BIT)) LCD.print("2");
////   -------------SW4----------------------
//  LCD.setCursor(x0 + 72, y0 + 80);
//  LCD.setTextColor(WHITE, BLACK);
//  LCD.print("SW4");
//  LCD.setCursor(x0 + 96, y0 + 80);
//  if (!bitRead(SW4_1_BYTE, SW4_1_BIT))  LCD.print("0");
//  if (bitRead(SW4_1_BYTE, SW4_1_BIT) && bitRead(SW4_2_BYTE, SW4_2_BIT)) LCD.print("1");
//  if (!bitRead(SW4_2_BYTE, SW4_2_BIT)) LCD.print("2");
//
//  LCD.setTextColor(WHITE, BLACK);
}
