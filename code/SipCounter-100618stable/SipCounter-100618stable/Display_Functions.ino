/********************************************************
  Update Sipper Interaction Duration and Counts on OLED display
********************************************************/
void UpdateDisplay() {
  if (PongActive == false) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    //display left sips
    display.setCursor(0, 0);
    display.println("Left");
    display.setCursor(1, 0);
    display.println("Left");

    display.setCursor(0, 13);
    if (LeftDuration < 1000) { // if it goes beyond 1000 seconds, drop the decimal place on the display (this precision will still be captured on the SD card)
      display.print(LeftDuration, 1);
    }
    else {
      display.print(LeftDuration, 0);
    }

    display.print("s");

    display.setCursor(0, 23);
    display.print("#:");
    display.println(LeftCount);

    //display right sips
    display.setCursor(50, 0);
    display.println("Right");
    display.setCursor(51, 0);
    display.println("Right");

    display.setCursor(50, 13);
    if (RightDuration < 1000) { // if it goes beyond 1000 seconds, drop the decimal place on the display (this precision will still be captured on the SD card)
      display.print(RightDuration, 1);
    }
    else {
      display.print(RightDuration, 0);
    }
    display.print("s");

    display.setCursor(50, 23);
    display.print("#:");
    display.println(RightCount);

    display.drawFastHLine(97, 1, 1, WHITE);
    display.drawFastHLine(97, 8, 1, WHITE);
    display.drawFastHLine(97, 15, 1, WHITE);
    display.drawFastHLine(97, 22, 1, WHITE);
    display.drawFastHLine(97, 29, 1, WHITE);

    //display Sip device #
    display.setCursor(105, 0);
    display.print("Sip");
    display.setCursor(105, 10);
    display.print(Sip);
    //display.print(measuredvbat);

    //Display battery graphic
    BatteryGraphic ();
    display.display();
  }
}
/********************************************************
  Display SD Card error
********************************************************/
void DisplaySDError() {
  Blink (RED_LED, 50, 3);
  delay (25);
  Blink (GREEN_LED, 50, 3);
  delay (25);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  display.display();
  display.setRotation(4);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Check SD card!");
  display.display();
}

/********************************************************
   HELPER FUNCTION FOR BLINKING ONBOARD LEDS ON FEATHER
 ********************************************************/
void Blink(byte PIN, byte DELAY_MS, byte loops) {
  for (byte i = 0; i < loops; i++)  {
    digitalWrite(PIN, HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN, LOW);
    delay(DELAY_MS);
  }
}

/********************************************************
   PONG MODE splash screen
 ********************************************************/
void PongSplash() {
  display.clearDisplay();
  display.display();
  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 50);
  display.println("PONG MODE!");
  display.display();
  delay (500);
}

/********************************************************
      Battery graphic indicating voltage levels
********************************************************/
void BatteryGraphic () {
  display.drawRoundRect (105, 24, 16, 8, 1, WHITE);
  display.drawRoundRect (120, 26, 3, 4, 0, WHITE);
  display.drawRoundRect (119, 27, 3, 2, 0, BLACK);

  display.drawFastHLine(10, 96, 1, WHITE);
  display.drawFastHLine(15, 96, 1, WHITE);
  display.drawFastHLine(20, 96, 1, WHITE);


  if (measuredvbat > 3.9) {
    display.fillRoundRect (107, 26, 12, 4, 0, WHITE);
  }

  else if (measuredvbat > 3.75) {
    display.fillRoundRect (107, 26, 9, 4, 0, WHITE);
  }

  else if (measuredvbat > 3.55) {
    display.fillRoundRect (107, 26, 6, 4, 0, WHITE);
  }

  // if voltage is less than 3.55 volts, it's about to die so blink a rectangle on the bottom part
  else {
    if (millis() - BlinkMillis >= 800) {
      display.fillRoundRect (107, 26, 3, 4, 0, WHITE);
      BlinkMillis = millis();
    }
  }
}

/********************************************************
      Screen showing total time elapsed
********************************************************/
void DisplayTimeElapsed() {
  calcTimeElapsed();
  sprintf(buf, "%02d:%02d:%02d", runHours, runMinutes, runSeconds);
  display.clearDisplay();
  display.setCursor(15, 0);
  display.println("Time since start");
  display.setCursor(40, 10);
  display.println(buf);
  display.setCursor(0, 20);
  display.print(filename);
  display.print (".CSV");
  display.display();
  delay(200);
}


/********************************************************
     Start screen
********************************************************/
void DisplayStartScreen() {
  display.setCursor(5, 0);
  if (rtc.getMonth() < 10)
    display.print('0');   // Trick to add leading zero for formatting
  display.print(rtc.getMonth());
  display.print("/");
  if (rtc.getDay() < 10)
    display.print('0');      // Trick to add leading zero for formatting
  display.print(rtc.getDay());
  display.print("/");
  display.print(rtc.getYear() + 2000);
  display.print("  ");
  if (rtc.getHours() < 10)
    display.print('0');      // Trick to add leading zero for formatting
  display.print(rtc.getHours());
  display.print(":");
  if (rtc.getMinutes() < 10)
    display.print('0');      // Trick to add leading zero for formatting
  display.print(rtc.getMinutes());
  display.print(":");
  if (rtc.getSeconds() < 10)
    display.print('0');      // Trick to add leading zero for formatting
  display.print(rtc.getSeconds());

  display.setCursor(5, 12);
  display.print ("  Press A to start");

  display.setCursor(5, 25);
  display.println(filename);
  display.display();
}
