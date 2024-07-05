// CORE 
#include "TFT_eSPI.h" /* Please use the TFT library provided in the library. */
#include "Logo.h"
#include "WiFi.h"
#include "time.h"

// Font
#include "QuinqueFive5pt7b.h" 
#include "Bene88pt7b.h"           /// Task fost
#include "Bene812pt7b.h"
#include "Bene89pt7b.h"
#include "Bene810pt7b.h"

// Assets
#include "BatteryBase.h"
#include "BatteryGreen.h"
#include "BulbBase.h"
#include "BulbYellow.h"

#include "brightnessIcon.h"
#include "todoIcon.h"


// Dimmer buttons
#define topbutton 0       // Top Button
#define PIN_POWER_ON 15   // LCD
#define PIN_LCD_BL 38     // Backlight

// Background color
//#define TFT_CUSTOM1 0x4975 
#define TFT_CUSTOM1 0x2952

// Foreground color
#define TFT_FORE 0x28ED  



TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

// NTP Time
const char* ntpServer = "time.nist.gov";
const long gmtOffset_sec = -1800L * 10; // CST -6hrs 180000
const int daylightOffset_sec = 0;
struct tm timeinfo;

// PUT TASK HERE
char task1[] = "Take oil to autozone.";
char task2[] = "Drop wheel junk yard."; 

// Set Brightness
int brightness = 20; // 0-255

// NETWORK CREDS
const char* ssid = "WolvIoT";
const char* password = "Robards0604*!";

// WIFI STATUS
  String get_wifi_status(int status){
      switch(status)
      {
          case WL_IDLE_STATUS:
          return "WL_IDLE_STATUS";
          case WL_SCAN_COMPLETED:
          return "WL_SCAN_COMPLETED";
          case WL_NO_SSID_AVAIL:
          return "WL_NO_SSID_AVAIL";
          case WL_CONNECT_FAILED:
          return "WL_CONNECT_FAILED";
          case WL_CONNECTION_LOST:
          return "WL_CONNECTION_LOST";
          case WL_CONNECTED:
          return "WL_CONNECTED";
          case WL_DISCONNECTED:
          return "WL_DISCONNECTED";
      }
}

//------------------------ CUSTOM VOID SETUP ------------------------//



//------------------------ VOID SETUP ------------------------//

void setup() {

  Serial.begin(115200);
  tft.init();

// Enable backlight with external battery.
  pinMode(PIN_POWER_ON, OUTPUT);  //enables the LCD and to run on battery
  pinMode(PIN_LCD_BL, OUTPUT);    //controls the LCD backlight
 
  digitalWrite(PIN_POWER_ON, HIGH);
  digitalWrite(PIN_LCD_BL, HIGH);

// Screen Rotation
  tft.setRotation(3);
  sprite.createSprite(320,170);
  tft.setSwapBytes(true);

// Set backlight
  ledcSetup(0, 10000, 8);
  ledcAttachPin(38, 0);
  ledcWrite(0, brightness);

// Splash screen
  tft.pushImage(0, 0, 320, 170, Logo);
  delay(3000); 


// Wifi Initilization
  int status = WL_IDLE_STATUS;
 //     WiFi.mode(WIFI_STA); //Optional
      WiFi.begin(ssid, password);
      delay(100);
      Serial.println("\nConnecting");
      Serial.println(get_wifi_status(status));

      while(WiFi.status() != WL_CONNECTED)
      {
          delay(500);
          status = WiFi.status();
          Serial.println(get_wifi_status(status));
          Serial.print(".");
          //delay(1000);        // Wait 5 seconds. If it can't get connected to wifi, break from loop.
   //       break;
      }

      Serial.println("\nConnected to the WiFi network");
      Serial.print("Local ESP32 IP: ");
      Serial.println(WiFi.localIP());



  // Sync time with NTP Server and update ESP32 RTC getLocalTime() return false if time is not set. 
  Serial.print("Syncing time with NTP server ");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo))
    {   
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        Serial.print(".");  
        delay(4000);              // Wait 5 seconds if it can't set, break from the loop. 
        break;
    }


WiFi.disconnect(true);
//WiFi.mode(WIFI_OFF);
myBackground();
}


//------------------------ VOID CUSTOM ------------------------//


void myBackground(){

  sprite.fillSprite(TFT_FORE);
  sprite.pushSprite(0,0);
}

// TOP RIGHT TIME - TIME & AM/PM
void timeContainer(){


getLocalTime(&timeinfo);
Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

sprite.fillRoundRect(240, 3, 80, 16, 3, TFT_FORE);    

sprite.setFreeFont(&Bene88pt7b);
sprite.setTextColor(TFT_WHITE, TFT_FORE);
sprite.setCursor(245, 15);
sprite.println(&timeinfo, "%I:%M");

sprite.setFreeFont(&Bene88pt7b);
sprite.setTextColor(TFT_WHITE, TFT_FORE);
sprite.setCursor(290, 15);
sprite.println(&timeinfo, "%p");
 sprite.pushSprite(0,0);
}


// Left Panel
  void weekdayContainer(){

    // Main panel background
    sprite.fillRoundRect(5, 5, 84, 158, 7, TFT_CUSTOM1);

    // Battery Icon
    sprite.drawBitmap(10, 115, BatteryBase, 8, 13, 0xFFFF);
    sprite.drawBitmap(12, 118, BatteryGreen, 4, 8, 0x57EA);

     // Brightness Icon
    sprite.drawBitmap(9, 135, BulbBase, 10, 13, 0xFFFF);
    sprite.drawBitmap(10, 136, BulbYellow, 8, 7, 0xF760);

    // Weekday - Top
    sprite.setFreeFont(&Bene812pt7b);
    sprite.setTextColor(TFT_WHITE, TFT_CUSTOM1);
    sprite.setCursor(38, 35);
    sprite.println(&timeinfo, "%a");

    // Month 
    sprite.setFreeFont(&Bene88pt7b);
    sprite.setTextColor(TFT_WHITE, TFT_CUSTOM1);
    sprite.setCursor(16, 48);
    sprite.println(&timeinfo, "[%b, %d]"); 
/*
    // Date
    sprite.setFreeFont(&Bene88pt7b);
    sprite.setTextColor(TFT_WHITE, TFT_CUSTOM1);
    sprite.setCursor(40, 74);
    sprite.println(&timeinfo, "%d,"); 

    // Year
    sprite.setFreeFont(&Bene88pt7b);
    sprite.setTextColor(TFT_WHITE, TFT_CUSTOM1);
    sprite.setCursor(60, 74);
    sprite.println(&timeinfo, "%y"); 
*/
    // System 
    sprite.setFreeFont(&Bene88pt7b);
    sprite.setTextColor(TFT_WHITE, TFT_CUSTOM1);
    sprite.setCursor(20, 110);
    sprite.println("System"); 

    // Battery Value
    sprite.setFreeFont(&Bene88pt7b);
    sprite.setTextColor(TFT_WHITE, TFT_CUSTOM1);
    sprite.setCursor(26, 125);
    sprite.println("[ 50% ]"); 

    // Brightness Value
    sprite.setFreeFont(&Bene88pt7b);
    sprite.setTextColor(TFT_WHITE, TFT_CUSTOM1);
    sprite.setCursor(26, 125);
    sprite.println("[ 20% ]"); 



    sprite.pushSprite(0,0);
  }

// Task Holder
  void mydayContainer(){
  //sprite.fillRoundRect(98, 3, 72, 16, 3, 0x2FFE);   
      
  sprite.drawBitmap(98, 7, todoIcon, 10, 12, 0xFFFF);

  sprite.setFreeFont(&Bene88pt7b);
  sprite.setTextColor(TFT_WHITE);
  sprite.drawString("Task", 110, 7);
  sprite.pushSprite(0,0);
}


// TASK ITEM #1
  void taskItem1(){

  sprite.fillRoundRect(95, 30, 220, 24, 7, TFT_CUSTOM1);      // Task background

  sprite.drawEllipse(110, 42, 7, 7, 0xFB40);
  sprite.drawEllipse(110, 42, 6, 6, 0xFB40);

  sprite.setFreeFont(&Bene88pt7b);
  sprite.setTextColor(TFT_WHITE, TFT_CUSTOM1);
  sprite.drawString(task1, 126, 37);                          // Task c
  sprite.pushSprite(0,0);
  }

// TASK ITEM #2
  void taskItem2(){

  sprite.fillRoundRect(95, 60, 220, 24, 7, TFT_CUSTOM1);      // Task background

  sprite.drawEllipse(110, 97, 7, 7, 0x1EE0);
  sprite.drawEllipse(110, 97, 6, 6, 0x1EE0);

  sprite.setFreeFont(&Bene88pt7b);
  sprite.setTextColor(TFT_WHITE, TFT_CUSTOM1);
  sprite.drawString(task2, 126, 92);                          // Task content
  sprite.pushSprite(0,0);
  }


void counter(){

   sprite.fillRoundRect(130, 118, 60, 20, 7, TFT_FORE);
  sprite.setFreeFont(&Bene88pt7b);
  sprite.setTextColor(TFT_WHITE, TFT_FORE);
  sprite.setCursor(136, 130);
  sprite.println(&timeinfo, "%S");
   sprite.pushSprite(0,0);
}




//------------------------ VOID LOOP ------------------------//

void loop() {

//-----------------------TOP NAVIGATION



/*
if (!digitalRead(topbutton)) {  //pressed is off
    digitalWrite(PIN_LCD_BL, LOW);
   // delayMicroseconds(200);
    digitalWrite(PIN_LCD_BL, HIGH);
  }
*/

//-----------------------TOP NAVIGATION
 timeContainer();

//-----------------------MY DAY CONTAINER
  mydayContainer();

//-----------------------TASK ITEM #1
  taskItem1();

//-----------------------TASK ITEM #2
  taskItem2();
counter();
//-----------------------RIGHT PANEL
  weekdayContainer();

  delay(100);
  }
