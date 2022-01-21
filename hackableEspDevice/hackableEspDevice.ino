/*
 * File:      hackableEspDevice.ino
 * Authors:   ESPinoza (Team 1)
 * Version:   0.1
 * 
 * The main file of the firmware of a vunerable-by-design ESP8266 controller.
 * For more information, go to: https://gitlab.fdmci.hva.nl/munkl/hackable_esp_device
 * 
 */
#include <ESP8266WebServer.h>                                               //For running the webserver
#include <FS.h>                                                             //For SPIFFS
#include <stdint.h>                                                         //For defining bits per integer
#include <WiFiManager.h>                                                    //For web-based wifi configuration
#include "config.h"                                                         //For the configuration
#include "UserHandler.h"                                                    //For handling the users from the config.conf
#include "SerialCommandExecuter.h"                                          //For handling serial commands
#include "Debugger.h"                                                       //For handling debug messages
#include "HostnameWrite.h"                                                  //For handling the hostname changes
#include "StartupText.h"                                                    //For printing startup log files

/* On and off are inverted because the built-in led is active low */
#define ON                      HIGH
#define OFF                     LOW

#define MIN_BRIGHTNESS          1022                                        //analogWrite() on ESP8266 D1 Mini board is inverted

ESP8266WebServer server(80);                                                //Object that listens for HTTP requests on port 80
uint8_t ledState = OFF;                                                     //Declare led state variable
uint16_t brightness = 1023;                                                 //For LED brightnesss

UserHandler userHandler(&server);                                           //For handling the authentication
SerialCommandExecuter cliExecuter;

File fsUploadFile;                                                          //A File object to temporarily store the received file

/**************************************************************************/
/*!
    @brief    Setup microchip.
*/
/**************************************************************************/
void setup() {
    Serial.begin(115200);                                                   //Serial port for debugging purposes
    
    /* Initialize SPIFFS */
    if (!SPIFFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    
    debugln("Debug is enabled");
    
    /* If debug is enabled, the root password is printed in a big string of text */
    if (getDebugEnabled()) {
      String mess = "ROOT: " + String(ROOT_PASSWORD);
      printStartupText(mess);
    }
    
    pinMode(LED_BUILTIN, OUTPUT);
    analogWrite(LED_BUILTIN, 1023);

    initializeHostname();
    setupWifi();    
    initializeServer();
    userHandler.updateUsers();
    cliExecuter.setUsers(userHandler.getUsers(), userHandler.getNumberOfUsers());
    
    debugln("Serial commands available. Typ 'help' for help.");
}

/**************************************************************************/
/*!
    @brief    Initializes hostname.
*/
/**************************************************************************/
void initializeHostname() {
    String customHostname = getHostname();
    /* Check if custom hostname is set, otherwise use default */
    if (customHostname != "") {
        /* Check if hostname can be set */
        if (WiFi.hostname(customHostname)){
            debug(customHostname);
            debugln(" is the hostname.");
        } else {
            debug("Could not set '");
            debug(customHostname);
            debugln("' as hostname.");
        }
    } else {
        if (WiFi.hostname(DEFAULT_HOSTNAME)) {
            debug(DEFAULT_HOSTNAME);
            debugln(" is the hostname.");
        } else {
            debug("Could not set '");
            debug(DEFAULT_HOSTNAME);
            debugln("' as hostname.");
        }
    }
}

/**************************************************************************/
/*!
    @brief    Connects to WiFi if it can, otherwise starts as AP to
              configure WiFI.
*/
/**************************************************************************/
void setupWifi() {
    WiFiManager wifiManager;
    
    if (wifiManager.autoConnect(WIFI_CONF_AP_NAME)) {
        Serial.print("Connected to: ");
        Serial.println(WiFi.SSID());
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Failed to connect, connect with AP");
        ESP.restart();
    }

    debug("Copy and paste the following URL: http://");
    
    if (WiFi.hostname(DEFAULT_HOSTNAME)) {
        debugln(DEFAULT_HOSTNAME);
    } else {
        debugln(WiFi.hostname().c_str());
    }
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
    /* Route for home page */
    server.on("/", HTTP_GET, []() {
        handleFileRequest("/index.html", PERMISSION_LVL_ALL);
    });

    server.on("/state", HTTP_GET, []() {
        sendToFrontend("ledState");
    });

    server.on("/brightness", HTTP_GET, []() {
        sendToFrontend("brightness");
    });
    
    /* Route for admin controls */
    server.on("/admin", HTTP_GET, []() {
        handleFileRequest("/admin.html", PERMISSION_LVL_ADMIN);
    });
    
    /* Route for user controls */
    server.on("/user", HTTP_GET, []() {
        handleFileRequest("/user.html", PERMISSION_LVL_USER);
    });
    
    /* Route for file upload page */
    server.on("/upload", HTTP_GET, []() {
        handleFileRequest("/upload.html", PERMISSION_LVL_ADMIN);
    });

    /* Route for file download page */
    server.on("/download", HTTP_GET, []() {
        handleFileRequest("/download.html", PERMISSION_LVL_USER);
    });
    
    /* Load style_desktop.css file, styling for desktop version */
    server.on("/styles.css", HTTP_GET, []() {
        handleFileRequest("/styles.css", PERMISSION_LVL_ALL);
    });
    
    /* Load style_mobile.css file, styling for mobile version */
    server.on("/styles_mobile.css", HTTP_GET, []() {
        handleFileRequest("/styles_mobile.css", PERMISSION_LVL_ALL);
    });

    /* Load style_switch.css file, styling for the on/off switch */
    server.on("/style_switch.css", HTTP_GET, []() {
        handleFileRequest("/style_switch.css", PERMISSION_LVL_ALL);
    });

    /* Load favicon.ico file, site icon */
    server.on("/favicon.ico", HTTP_GET, []() {
        handleFileRequest("/favicon.ico", PERMISSION_LVL_ALL);
    });

    /* Load jquery.min.js file, for ajax */
    server.on("/jquery.min.js", HTTP_GET, []() {
        handleFileRequest("/jquery.min.js", PERMISSION_LVL_ALL);
    });

    /* Load base.js file, JavaScript for site */
    server.on("/base.js", HTTP_GET, []() {
        handleFileRequest("/base.js", PERMISSION_LVL_ALL);
    });

    /* Load switch.js file, JavaScript for on/off switch */
    server.on("/switch.js", HTTP_GET, []() {
        handleFileRequest("/switch.js", PERMISSION_LVL_ALL);
    });
    /*
    * End of file loading
    */

    /*
    * Routes for JavaScript data receiving
    */
    /* Route for setting power */
    server.on("/set_power", HTTP_GET, []() {
        if (server.arg("state")) {
            ledState = atoi(server.arg("state").c_str());
            if(ledState == ON) {
                analogWrite(LED_BUILTIN, MIN_BRIGHTNESS-brightness);
            } else {
                analogWrite(LED_BUILTIN, 1023);
            }
        }
        handleFileRequest("/index.html", PERMISSION_LVL_ALL);
    });
    
    /* Route for brightness */
    server.on("/update_brightness", HTTP_GET, []() {
        if (server.arg("brightness")) {
            brightness = atoi(server.arg("brightness").c_str());
            if(ledState == ON) {
                analogWrite(LED_BUILTIN, MIN_BRIGHTNESS-brightness);
            }
        }
        handleFileRequest("/index.html", PERMISSION_LVL_ALL);
    });


    /* Route for restarting the server */
    server.on("/restart", HTTP_GET, []() {
        handleFileRequest("/", PERMISSION_LVL_ALL);
        ESP.restart();
    });
    /*
    * End of JavaScript data receiving
    */
    /*
    * Routes for file management
    */
    /* Route for file upload request */
    server.on("/upload", HTTP_POST, []() {
        server.send(200);                                                   //HTTP code 200 == OK
        debugln("Wait, something got uploaded");
      }, handleFileUpload                                                   //Receive and save the file
    );
    /* Route for file upload request */
    server.on("/download", HTTP_POST, []() {
        debugln("File download request");
      }, handleFileDownload                                                 //Receive and save the file
    );
    /*
    * End of routes for file management
    */
    /* Not found */
    server.onNotFound([]() {                                                //If the client requests any URI
        handleFileRequest(server.uri(), PERMISSION_LVL_ALL);                //send it if it exists
        debugln("NOT_FOUND?");
    });
    server.begin();                                                         //Start server
}

/**************************************************************************/
/*!
  @brief    Replaces placeholders with actual data in HTML page.
*/
/**************************************************************************/
void sendToFrontend(String var){
    if (var == "ledState") {
        server.send(200, "text/plain", String (ledState));
    } else if (var == "brightness") {
        server.send(200, "text/plain", String (brightness));
    }
}

/**************************************************************************/
/*!
    @brief    Mainloop.
*/
/**************************************************************************/
void loop() {
  server.handleClient();

  if (Serial.available()) {
    cliExecuter.executeCommand();
  }
}

/**************************************************************************/
/*!
  @brief    Converts the file extension to the MIME type.
  @param    filename        Name of the file
  @returns  String          MIME type of the file
*/
/**************************************************************************/
String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  else if (filename.endsWith(".txt")) return "text/plain";
  return "text/plain";
}

/**************************************************************************/
/*!
  @brief    Sends the requested file if the user has permission.
  @param    path            Path to the file
  @param    permissionLevel 0 = not logged in, 1 = user, 2 = admin
*/
/**************************************************************************/
void handleFileRequest(String path, uint8_t permissionLevel) {
    if(!userHandler.checkPermission(permissionLevel, &server)) {
        server.requestAuthentication();
        return;
    }
    
    debugln(String("Requested file: ") + path);
    
    String contentType = getContentType(path);                              //Get the MIME type
    String pathWithGz = path + ".gz";
    
    if (SPIFFS.exists(pathWithGz)) {                                        //If there's a compressed version available
        path += ".gz";                                                      //Use the compressed verion
    }
    
    if (SPIFFS.exists(path)) {
        File file = SPIFFS.open(path, "r");                                 //Open the file
        size_t sent = server.streamFile(file, contentType);                 //Send it to the client
        file.close();                                                       //Close the file again
        debugln(String("Sent file: ") + path);
        return;
    }
    
    debugln(String("File Not Found: ") + path);                             //If the file doesn't exist, return false
    server.send(404, "text/plain", "404: Not Found");                       //otherwise, respond with a 404 (Not Found) error
}

/**************************************************************************/
/*!
  @brief    Handles the file upload to the SPIFFS.
*/
/**************************************************************************/
void handleFileUpload() {
    HTTPUpload& upload = server.upload();
    
    if (upload.status == UPLOAD_FILE_START) {
        String filename = upload.filename;
        
        if (!filename.startsWith("/")) {
            filename = "/" + filename;
        }
        
        debugln(String("Upload file named: ") + filename);
        
        fsUploadFile = SPIFFS.open(filename, "w");                          //Open the file for writing in SPIFFS (create if it doesn't exist)

    } else if (upload.status == UPLOAD_FILE_WRITE && fsUploadFile ) {
        fsUploadFile.write(upload.buf, upload.currentSize);                 //Write the received bytes to the file
    } else if (upload.status == UPLOAD_FILE_END) {
        if (fsUploadFile) {                                                 //If the file was successfully created
            fsUploadFile.close();                                           //Close the file again
            debugln(String("handleFileUpload Size: ") + upload.totalSize);
            server.sendHeader("Location","/success.html");                  //Redirect the client to the success page
            server.send(303);
            userHandler.updateUsers();
            cliExecuter.setUsers(userHandler.getUsers(), userHandler.getNumberOfUsers()); //Update users for cli as well
        } else {
            server.send(500, "text/plain", "500: couldn't create file");
        }
    }
}

/**************************************************************************/
/*!
  @brief    Handles the file download to the SPIFFS.
*/
/**************************************************************************/
void handleFileDownload() {
    String filename = server.arg("filekey");                                //Get user input for filename

    if (!filename.startsWith("/")) {
        filename = "/" + filename;
    }

    if (!SPIFFS.exists(filename)) {
        server.send(404, "text/plain", "404: file not found!");
        return;
    }

    File download = SPIFFS.open(filename, "r");

    debugln("Start sending file");

    server.sendHeader("Content-Type", "text/text");
    server.sendHeader("Content-Disposition", "attachment; filename="+filename);
    server.sendHeader("Connection", "close");
    server.streamFile(download, "application/octet-stream");
    download.close();
    server.send(200);                                                       //HTTP code 200 == OK
}
