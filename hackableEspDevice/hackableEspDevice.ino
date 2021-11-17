/*
 * File:      hackableEspDevice.ino
 * Authors:   ESPinoza (Team 1)
 * Version:   0.1
 * 
 * The main file of the firmware of a vunerable-by-design ESP8266 controller.
 * For more information, go to: https://gitlab.fdmci.hva.nl/munkl/hackable_esp_device
 * 
 */
#include "ESPAsyncWebServer.h"                                              //For running the webserver
#include <FS.h>                                                             //For SPIFFS

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
#define WIFI_SSID       "ssid"
#define WIFI_PASSWORD   "password"

#define HTTP_USERNAME   "admin"
#define HTTP_PASSWORD   "admin"

AsyncWebServer server(80);                                                  //Create AsyncWebServer object on port 80

/**************************************************************************/
/*!
  @brief    Replaces placeholders with actual data in HTML page.
*/
/**************************************************************************/
String processor(const String& var){
    if (var == "IS_ON") {
        return (String) 1;
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
    if (DEBUG) {
        Serial.begin(115200);                                                     //Serial port for debugging purposes
    }

    /* Initialize SPIFFS */
    if(!SPIFFS.begin()) {
        debugln("An Error has occurred while mounting SPIFFS");
        return;
    }

    connectWifi();
    initializeServer();
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
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    debug("Connecting to WiFi");
    delay(50);                                                                //Wait for setup, to prevent strange behaviour

    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        debug(".");
    }

    debugln("");
    debug("IP: ");
    debugln(WiFi.localIP());                                           //Print ESP32 Local IP Address
}

/**************************************************************************/
/*!
    @brief    Takes care of the webservices like pageloading.
*/
/**************************************************************************/
void initializeServer() {
    /*
    *  Routes for loading all the necessary files
    */
    /* Load homepage or login page */
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(!request->authenticate(HTTP_USERNAME, HTTP_PASSWORD)) {
            return request->requestAuthentication();   
        }
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    
    /* Load style_desktop.css file, styling for desktop version */
    server.on("/style_desktop.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style_desktop.css", "text/css");
    });
    
    /* Load style_mobile.css file, styling for mobile version */
    server.on("/style_mobile.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style_mobile.css", "text/css");
    });

    /* Load favicon.ico file, site icon */
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.ico", "image/x-icon");
    });

    /* Load jquery.min.js file, not needed yet */
    //server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    //    request->send(SPIFFS, "/jquery.min.js", "text/script");
    //});

    /* Load base.js file, JavaScript for site */
    //server.on("/base.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    //    request->send(SPIFFS, "/base.js", "text/script");
    //});
    /*
    * End of file loading
    */

    /*
    * Routes for receiving data
    */
    /* Route for login EXAMPLE */
    server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("username") && request->hasParam("password")) {
            String username = request->getParam("username")->value().c_str();
            String password = request->getParam("password")->value().c_str();
        }

        request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    /*
    * End of data receiving
    */

    server.begin();                                                           //Start server
}
