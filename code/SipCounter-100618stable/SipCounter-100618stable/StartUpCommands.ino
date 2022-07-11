/********************************************************
  Start Up Commands
********************************************************/
void StartUpCommands () {
  Serial.begin(115200);

  /********************************************************
    These commands are associated with the low power sleep states
  ********************************************************/
  SYSCTRL->VREG.bit.RUNSTDBY = 1;
  SYSCTRL->DFLLCTRL.bit.RUNSTDBY = 1;
  USBDevice.detach();
  for (byte i = 0; i <= 20; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  /********************************************************
     Start RTC
   ********************************************************/
  rtc.begin(); // initialize RTC

  /********************************************************
    Setting digital pin modes
  ********************************************************/
  pinMode(LeftSip, INPUT);
  pinMode(RightSip, INPUT);
  pinMode (RED_LED, OUTPUT);
  pinMode (GREEN_LED, OUTPUT);
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  pinMode (outLeft, OUTPUT);
  pinMode (outRight, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(outLeft, LOW);
  digitalWrite(outRight, LOW);

  /********************************************************
    SD Card setup function
  ********************************************************/
  CreateFile();
  writeHeader();

  /********************************************************
    Start, clear, and setup the display
  ********************************************************/
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0); // Where c is a value from 0 to 255 (sets contrast e.g. brightness)

  display.setRotation(4);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  /********************************************************
     Display start date time and filename
    ********************************************************/
  display.clearDisplay();
  DisplayStartScreen();

  //delay until button A is pressed to start sipper
  while (digitalRead(buttonA) == HIGH) {
    if (PongActive == false) {
      display.fillRoundRect (00, 0, 132, 11, 1, BLACK);
      leftmillis = millis();
      rightmillis = millis();
      startmillis = millis();
      DisplayStartScreen();
    }

    //Code for starting the sequence to set the clock and sipper #
    if (digitalRead(buttonB) == LOW) {  //if button B is held
      delay (500);
      if (digitalRead(buttonB) == LOW) {
        SetSequence = 1;
        SetClockVars();
        display.clearDisplay(); //after setting variables, return to start screen
        DisplayStartScreen();
      }
    }

    //Activate Pong game!
    if (PongActive == false & digitalRead(buttonC) == LOW) {
      delay (1000);
      if (PongActive == false & digitalRead(buttonC) == LOW) {
        PongActive = true;
        PongSplash();
      }
    }

    //Code for starting PONG!
    if (PongActive == true) {
      Pong();
    }

  }
  Blink (RED_LED, 50, 3);
}
