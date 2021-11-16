/*
 * File:      wakeupStrip.ino
 * Author:    Luke de Munk
 * Version:   1.0
 * 
 * Code for a WS2801 LEDstrip as wake up and chill light.
 */
#include <FastLED.h>
#include <IRremote.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "WS2801strip.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

//Pins:
#define DATA_OUT_PIN    16
#define CLOCK_OUT_PIN   17
#define IR_RECEIVE_PIN  15
#define IR_INT_PIN      19

//IR receiver codes:
#define IR_BRGT_UP      0xFFA25D
#define IR_BRGT_DOWN    0xFF629D
#define IR_OFF          0xFFE21D
#define IR_ON           0xFF22DD
#define IR_RED          0xFF02FD
#define IR_GREEN        0xFFC23D
#define IR_BLUE         0xFFE01F
#define IR_WHITE        0xFFA857
#define IR_ORANGE       0xFF906F
#define IR_LGREEN       0x1
#define IR_LBLUE        0x2
#define IR_FLASH        0x3
#define IR_ORANGE2      0xFF6897
#define IR_LCYAN        0xFF9867
#define IR_DPURPLE      0xFFB04F
#define IR_STROBE       0xFF30CF
#define IR_ORANGE3      0xFF18E7
#define IR_CYAN         0xFF7A85
#define IR_PURPLE       0xFF10EF
#define IR_FADE         0xFF38C7
#define IR_YELLOW       0xFF5AA5
#define IR_DCYAN        0xFF42BD
#define IR_PINK         0xFF4AB5
#define IR_SMOOTH       0xFF52AD

#define NUM_LEDS        96
#define BRGT_STEPS      20

//Modes:
#define COLOR           0
#define FADE            1
#define RAINBOW         2
#define FLASH           3
#define STROBE          4
#define NUM_MODES       5                                                   //Just add up num of modes

WS2801strip strip(DATA_OUT_PIN, CLOCK_OUT_PIN, NUM_LEDS);                   //Ledstrip
bool wasOn = false;                                                         //If was on before interrupt: true
byte prevMode = COLOR;
CRGB prevColor = CRGB (255, 255, 255);

IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

//Network credentials
const char* ssid = "Wifi Boven";
const char* password = "abCDef1245";
AsyncWebServer server(80);                                                  //Create AsyncWebServer object on port 80

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String timeStamp;

long loopCount = 0;                                                         //Used to prevent time check from spamming

/**************************************************************************/
/*!
  @brief    Replaces placeholders with actual data in HTML page.
*/
/**************************************************************************/
String processor(const String& var){
  if (var == "IS_ON") {
    return (String) strip.getIsOn();
  } else if (var == "COLOR") {
    CRGB tempColor = strip.getFullColor();
    return (String) ((0 << 24) | (tempColor.r << 16) | (tempColor.g << 8) | tempColor.b);
  } else if (var == "MODE") {
    return (String) strip.getMode();
  } else if (var == "BRIGHTNESS") {
    return (String) strip.getBrgt();
  }  else if (var == "WAKEUP_TIME_H") {
    return (String) strip.getAlarmH();
  } else if (var == "WAKEUP_TIME_M") {
    return (String) strip.getAlarmM();
  } else if (var == "WAKEUP_DURATION") {
    return (String) strip.getAlarmDur();
  } else if (var == "ALARM_ENABLED") {
    return (String) strip.getAlarmEnabled();
  } else {
    return " placeholder_error ";
  }
  return String();
}

/**************************************************************************/
/*!
  @brief    Setup Arduino.
*/
/**************************************************************************/
void setup() {
  Serial.begin(115200);                                                     //Serial port for debugging purposes
  
  irrecv.enableIRIn();                                                      //Start the receiver
  
  pinMode(IR_INT_PIN, INPUT_PULLUP);
  attachInterrupt(IR_INT_PIN, newData, FALLING);
  
  strip.colorAllLeds(CRGB (255, 255, 255));
  strip.turnOn();

  //Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
    
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());                                           //Print ESP32 Local IP Address

  timeClient.begin();                                                       //Initialize a NTPClient to get time
  timeClient.setTimeOffset(3600);                                           //GMT +2 = 7200 (Summer time otherwise 3600)

  /*
   *  Routes for loading all the necessary files
   */
  //Load index.html file
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Request");
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  //Load style_desktop.css file
  server.on("/style_desktop.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style_desktop.css", "text/css");
  });
  
  //Load style_mobile.css file
  server.on("/style_mobile.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style_mobile.css", "text/css");
  });

  //Load spectrum.css file
  server.on("/spectrum.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/spectrum.css", "text/css");
  });

  //Load style_powerbutton.css file
  server.on("/style_switches.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style_switches.css", "text/css");
  });

  //Load favicon.ico file
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.ico", "image/x-icon");
  });

  //Load jquery.min.js file
  server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery.min.js", "text/script");
  });

  //Load spectrum.js file
  server.on("/spectrum.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/spectrum.js", "text/script");
  });

  //Load powerbutton.js file
  server.on("/switches.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/switches.js", "text/script");
  });

  //Load powerbutton.js file
  server.on("/base.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/base.js", "text/script");
  });
  /*
   * End of file loading
   */

  /*
   * Routes for receiving data
   */
  //Route for power
  server.on("/set_power", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("jsdata")) {
      bool isOn = (bool) atoi(request->getParam("jsdata")->value().c_str());
      if (isOn && !strip.getIsOn()) {
        strip.turnOn();
      } else if (!isOn && strip.getIsOn()) {
        strip.turnOff();
      }
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  //Route for updating color
  server.on("/update_color", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("jsdata")) {
      String str = request->getParam("jsdata")->value();
      int color = strtol(&str[1], NULL, 16);

      int red_mask = 0xFF0000;
      int green_mask = 0xFF00;
      int blue_mask = 0xFF;

      int r = (color & red_mask) >> 16;
      int g = (color & green_mask) >> 8;
      int b = (color & blue_mask);
      strip.colorAllLeds(CRGB (r, g, b));
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  //Route for updating brightness
  server.on("/update_brightness", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("jsdata")) {
      int brightness = atoi(request->getParam("jsdata")->value().c_str());
      strip.setAbsoluteBrightness(brightness, true);
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  //Route for updating mode
  server.on("/set_mode", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("jsdata")) {
      byte m = (byte) atoi(request->getParam("jsdata")->value().c_str());
      if (m < NUM_MODES) {
        strip.setMode(m);
        strip.updateMode();
      }
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  //Route for updating alarm time settings
  server.on("/set_times", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("start_h")) {
      strip.setAlarmH((byte) atoi(request->getParam("start_h")->value().c_str()));
      strip.setAlarmM((byte) atoi(request->getParam("start_m")->value().c_str()));
      strip.setAlarmDur((byte) atoi(request->getParam("cycle_dur")->value().c_str()));
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  //Route for alarm enabled
  server.on("/set_alarm", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("jsdata")) {
      
      bool alarmEnabled = (bool) atoi(request->getParam("jsdata")->value().c_str());
      if (strip.getAlarmEnabled() != alarmEnabled) {
        strip.setAlarmEnabled(alarmEnabled);
      }
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  /*
   * End of data receiving
   */

  server.begin();                                                           //Start server
}

void newData() {
  detachInterrupt(IR_INT_PIN);                                              //Disable IR interrupt (keeps from getting triggered by the IR message)
  strip.setBuff();
}

/**************************************************************************/
/*!
  @brief    Mainloop.
*/
/**************************************************************************/
void loop() {
  if (strip.getBuff()) {
    while (!irrecv.decode(&results)) continue;                              //Wait till value is received
    Serial.println(results.value, HEX);
    if (results.value == IR_ON && !strip.getIsOn()) {
      strip.turnOn();
    } else if (strip.getIsOn()) {
      switch (results.value) {
        case IR_BRGT_UP:
          strip.setRelativeBrightness(BRGT_STEPS);
          break;
        case IR_BRGT_DOWN:
          strip.setRelativeBrightness(-BRGT_STEPS);
          break;
        case IR_OFF:
          if (strip.getIsOn())
            strip.turnOff();
          break;
        case IR_RED:
          strip.colorAllLeds(CRGB(255, 0, 0));
          break;
        case IR_GREEN:
          strip.colorAllLeds(CRGB(0, 255, 0));
          break;
        case IR_BLUE:
          strip.colorAllLeds(CRGB(0, 0, 255));
          break;
        case IR_WHITE:
          strip.colorAllLeds(CRGB(255, 255, 255));
          break;
        case IR_ORANGE:
          strip.colorAllLeds(CRGB(255, 49, 0));
          break;
        case IR_LGREEN:
          strip.colorAllLeds(CRGB(173, 255, 47));
          break;
        case IR_LBLUE:
          strip.colorAllLeds(CRGB(0, 191, 255));
          break;
        case IR_FLASH:
          strip.setMode(FLASH);
          break;
        case IR_ORANGE2:
          strip.colorAllLeds(CRGB(255, 100, 0));
          break;
        case IR_LCYAN:
          strip.colorAllLeds(CRGB(0, 255, 255));
          break;
        case IR_DPURPLE:
          strip.colorAllLeds(CRGB(75, 0, 130));
          break;
        case IR_STROBE:
          strip.setMode(STROBE);
          break;
        case IR_ORANGE3:
          strip.colorAllLeds(CRGB(255, 115, 0));
          break;
        case IR_CYAN:
          strip.colorAllLeds(CRGB(32, 178, 170));
          break;
        case IR_PURPLE:
          strip.colorAllLeds(CRGB(220, 0, 128));
          break;
        case IR_FADE:
          strip.setMode(FADE);
          break;
        case IR_YELLOW:
          strip.colorAllLeds(CRGB(255, 205, 0));
          break;
        case IR_DCYAN:
          strip.colorAllLeds(CRGB(0, 128, 128));
          break;
        case IR_PINK:
          strip.colorAllLeds(CRGB(255, 105, 180));
          break;
        case IR_SMOOTH:
          strip.setMode(RAINBOW);
          break;
        default:
          break;  
      }
    }
    
    irrecv.resume();                                                        //Receive the next value
    attachInterrupt(IR_INT_PIN, newData, FALLING);                          //Re-enable IR interrupt
    strip.resetBuff();                                                      //Reset buffer to 0
  } else {
    if (strip.getAlarmEnabled()) {                                          //If alarm is not enabled, there is no need for checking time
      if (loopCount >= 100) {                                               //Prevent from spamming the server
        loopCount = 0;
        if (checkTime() && !strip.getIsOn())                                //If time is same as setted alarm time and strip is off, do wake up cycle
          strip.wakeupCycle();
      }
    }
    loopCount++;
    if (loopCount >= 200)
      loopCount = 100;
      
    //Serial.println(loopCount);
  }
  strip.updateMode();                                                       //Reload the stripdriver
  if (strip.getMode() == COLOR || !strip.getIsOn()) {
    delay(100);
  }
}

bool checkTime() {
  Serial.println("Time check");
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  formattedDate = timeClient.getFormattedDate();

  int splitT = formattedDate.indexOf("T");
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);

  int h = atoi(timeStamp.substring(0, 2).c_str());
  int m = atoi(timeStamp.substring(3, 5).c_str());

  if (h == strip.getAlarmH() && m == strip.getAlarmM()) {
    return true;
  }
  return false;
}
