/********************************************************
  Include these libraries
********************************************************/
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <RTCZero.h>
#include <SPI.h>
#include <SdFat.h>
SdFat SD;             //Quick way to make SdFat work with standard SD.h sketches
#include <Adafruit_GFX.h>
#include <TimeLib.h> //include the Arduino Time library
#include <stdio.h>  // include the C++ standard IO library

/********************************************************
  Set up OLED screen
********************************************************/
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

/********************************************************
  Feather pins being used
********************************************************/
#define RED_LED 13
#define GREEN_LED 8
#define LeftSip 10
#define RightSip 11
#define VBATPIN A7
#define cardSelect 4
#define buttonA 9
#define buttonB 6
#define buttonC 5
#define outLeft A1
#define outRight A3

/********************************************************
  Initialize variables
********************************************************/
int LeftCount = 0;
int RightCount = 0;
double LeftDuration = 0.0;
double RightDuration = 0.0;
bool LeftActive = false;
bool RightActive = false;
bool logReady = false;
int logtimer = 0;
float measuredvbat = 1.00;
bool PongActive = false;
unsigned long BlinkMillis = millis();
unsigned long leftmillis = millis();
unsigned long rightmillis = millis();
unsigned long startmillis = millis();
char buf[21];
int logfreq = 10;  // Change this to edit # of seconds between data points
int SetSequence = 0;

/********************************************************
  Pong Setup
********************************************************/
unsigned long processTime;
int buttonAState = 0;
int buttonBState = 0;
int buttonCState = 0;
int score = 0;
int max_score = 1;

class Ball {
  public:
    int r = 2;
    int x = 100;
    int y = rand() % (31 - 2 * r) + r;
    int dx = -2;
    int dy = 2;

    int draw() {
      display.fillCircle(x, y, r, WHITE);
      return 0;
    }

    int update() {
      if (x >= 109 - r - 1) {
        dx *= -1;
      }
      if ((y <= r + 1) or (y >= 31 - r - 1)) {
        dy *= -1;
      }

      x += dx;
      y += dy;

      return 0;
    }
};

class Paddle {
  public:
    int height = 10;
    int width = 2;
    int centerx = width;
    int centery = 16;

    int draw() {
      display.fillRect(centerx - width / 2, centery - height / 2, width, height, WHITE);
      return 0;
    }

    int update() {
      if ((buttonAState == LOW) and (centery >= height / 2 + 1)) {
        centery -= 4;
      }
      if ((buttonCState == LOW) and (centery <= 31 - height / 2 - 1)) {
        centery += 4;
      }
      return 0;
    }
};

Ball ball;
Paddle paddle;

/********************************************************
  Setup RTC and filenames on SD card
********************************************************/
RTCZero rtc;

void dateTime(uint16_t* date, uint16_t* time) {
  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(rtc.getYear() - 48, rtc.getMonth(), rtc.getDay());

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
}

File logfile;         // Create file object
File configfile;         // Create another file object
char filename[15];    // Array for file name data logged to named in setup
int Sip;

/********************************************************
  Setup date and time variables
********************************************************/
char s_month[5];
int tmonth, tday, tyear, thour, tminute, tsecond;
static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
unsigned long elapsed = ((millis() - startmillis) / 1000);
int runHours = elapsed / 3600;
int secsRemaining = elapsed % 3600;
int runMinutes = secsRemaining / 60;
int runSeconds = secsRemaining % 60;
