/*
 * File:      hackableEspDevice.ino
 * Authors:   ESPinoza (Team 1)
 * Version:   0.1
 * 
 * The main file of the firmware of a vunerable-by-design ESP8266 controller.
 * For more information, go to: https://gitlab.fdmci.hva.nl/munkl/hackable_esp_device
 * 
 */
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

#define DEBUG 0                                                             //For defining serial debugging

/*
 * This precompiler functions are there for switching on and off the serial debugger
 */
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

//Network credentials
const char* ssid = "Wifi Boven";
const char* password = "abCDef1245";
AsyncWebServer server(80);                                                  //Create AsyncWebServer object on port 80

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
    @brief    Setup microchip.
*/
/**************************************************************************/
void setup() {
  Serial.begin(115200);                                                     //Serial port for debugging purposes

  //Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

/**************************************************************************/
/*!
    @brief    Mainloop.
*/
/**************************************************************************/
void loop() {

}

/**************************************************************************/
/*!
    @brief    Connects to Wi-Fi.
*/
/**************************************************************************/
void connectWifi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi.");
    }

    Serial.println(WiFi.localIP());                                           //Print ESP32 Local IP Address
}

void initializeServer() {
    /*
    *  Routes for loading all the necessary files
    */
    //Load homepage or login page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (isLoggedIn()) {
            request->send(SPIFFS, "/index.html", String(), false, processor);
        } else {
            request->send(SPIFFS, "/login.html", String(), false, processor);
        }
    });

    //Load index.html file, homepage make this login?
    //server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    //    request->send(SPIFFS, "/index.html", String(), false, processor);
    //});
    
    //Load style_desktop.css file, styling for desktop version
    server.on("/style_desktop.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style_desktop.css", "text/css");
    });
    
    //Load style_mobile.css file, styling for mobile version
    server.on("/style_mobile.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style_mobile.css", "text/css");
    });

    //Load favicon.ico file, site icon
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.ico", "image/x-icon");
    });

    //Load jquery.min.js file, not needed yet
    server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/jquery.min.js", "text/script");
    });

    //Load base.js file, JavaScript for site
    server.on("/base.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/base.js", "text/script");
    });
    /*
    * End of file loading
    */

    /*
    * Routes for receiving data
    */
    //Route for login
    server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("username") && request->hasParam("password")) {
            String username = request->getParam("username")->value().c_str();
            String password = request->getParam("password")->value().c_str();
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
    /*
    * End of data receiving
    */

    server.begin();                                                           //Start server
}