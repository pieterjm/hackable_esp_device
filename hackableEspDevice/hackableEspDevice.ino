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
#include <neotimer.h>                                                       //For non-blocking timers (used for code execution in intervals)
#include "config.h"                                                         //For the configuration. If not exists: copy "config_template.h", add your configuration and rename to "config.h"

/* On and off are inverted because the built-in led is active low */
#define ON                      LOW
#define OFF                     HIGH

/* Permission levels */
#define PERMISSION_LVL_ALL      0
#define PERMISSION_LVL_USER     1
#define PERMISSION_LVL_ADMIN    2
#define USER_INFO_LENGTH        3
#define MAX_NUMBER_USERS        10

#define HTTP_CONFIG_LOCATION    "/conf.txt"

ESP8266WebServer server(80);                                                //Object that listens for HTTP requests on port 80
Neotimer timer = Neotimer(30000);                                           //Setup a 30 second timer, to execute code with a 30 interval
uint8_t ledState = OFF;                                                     //Declare led state variable

File fsUploadFile;                                                          //A File object to temporarily store the received file
String users[MAX_NUMBER_USERS*USER_INFO_LENGTH];
uint8_t numberUsers = 0;

/**************************************************************************/
/*!
    @brief    Setup microchip.
*/
/**************************************************************************/
void setup() {
    if (DEBUG) {
        Serial.begin(115200);                                               //Serial port for debugging purposes
    }

    /* Initialize SPIFFS */
    if(!SPIFFS.begin()) {
        debugln("An Error has occurred while mounting SPIFFS");
        return;
    }
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, ledState);
    
    connectWifi();
    updateUsers();
    initializeServer();
}

/**************************************************************************/
/*!
    @brief    Connects to Wi-Fi.
*/
/**************************************************************************/
void connectWifi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    debug("Connecting to WiFi");
    delay(50);                                                              //Wait for setup, to prevent strange behaviour

    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        debug(".");
    }

    debugln("");
    debug("IP: ");
    debugln(WiFi.localIP());                                                //Print ESP32 Local IP Address

    debug("Copy and paste the following URL: http://");
    debugln(WiFi.localIP());
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
    server.on("/style_desktop.css", HTTP_GET, []() {
      handleFileRequest("/style_desktop.css", PERMISSION_LVL_ALL);
    });
    
    /* Load style_mobile.css file, styling for mobile version */
    server.on("/style_mobile.css", HTTP_GET, []() {
      handleFileRequest("/style_mobile.css", PERMISSION_LVL_ALL);
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
            digitalWrite(LED_BUILTIN, !ledState);
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
        server.send(200);                                                   //HTTP code 200 == OK
        debugln("Wait, something got downloaded");
      }, handleFileDownload                                                 //Receive and save the file
    );
    /*
    * End of routes for file management
    */

    /* Not found */
    server.onNotFound([]() {                                                //If the client requests any URI
        handleFileRequest(server.uri(), PERMISSION_LVL_ALL);                //send it if it exists
        Serial.printf("NOT_FOUND?");
    });

    server.begin();                                                         //Start server
}

/**************************************************************************/
/*!
    @brief    Mainloop.
*/
/**************************************************************************/
void loop() {
  server.handleClient();
  if(timer.repeat()){
      debugln(WIFI_PASSWORD);
  }
}

/**************************************************************************/
/*!
  @brief    Replaces placeholders with actual data in HTML page.
*/
/**************************************************************************/
String processor(const String& var){
    if (var == "LED_STATE") {
        return (String) ledState;
    } else {
        return " placeholder_error ";
    }
    return String();
}

/**************************************************************************/
/*!
  @brief    Converts the file extension to the MIME type.
  @param    filename        Name of the file
  @returns  String          MIME type of the file
*/
/**************************************************************************/
String getContentType(String filename) { // 
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
  if(!checkPermission(permissionLevel)) {
      server.requestAuthentication();
      return;
  }
  debugln(String("Requested file: ") + path);
  String contentType = getContentType(path);                                //Get the MIME type
  String pathWithGz = path + ".gz";
  
  if (SPIFFS.exists(pathWithGz)) {                                          //If there's a compressed version available
    path += ".gz";                                                          //Use the compressed verion
  }
  
  if (SPIFFS.exists(path)) { // If the file exists
    File file = SPIFFS.open(path, "r");                                     //Open the file
    size_t sent = server.streamFile(file, contentType);                     //Send it to the client
    file.close();                                                           //Close the file again
    debugln(String("Sent file: ") + path);
    return;
  }
  
  debugln(String("File Not Found: ") + path);                               //If the file doesn't exist, return false
  server.send(404, "text/plain", "404: Not Found");                         //otherwise, respond with a 404 (Not Found) error
}

/**************************************************************************/
/*!
  @brief    Checks if user has permission.
  @param    permissionLevel 0 = not logged in, 1 = user, 2 = admin
  @returns  bool            If user has permission
*/
/**************************************************************************/
bool checkPermission(uint8_t permissionLevel) {
  bool isLoggedIn = false;
  bool hasPermission = false;
  uint8_t userIndex = 0;
  
  if (permissionLevel == PERMISSION_LVL_ALL) {
    return true;
  } else {
    for (uint8_t i = 0; i < numberUsers; i += 3) {
      if (server.authenticate(users[i].c_str(), users[i+1].c_str())) {
        userIndex = i;
        isLoggedIn = true;
        break;
      }
    }
    if (isLoggedIn && atoi(users[userIndex+2].c_str()) >= permissionLevel) {
      return true;
    }
  }
  return false;
}

/**************************************************************************/
/*!
  @brief    Handles the file upload to the SPIFFS.
*/
/**************************************************************************/
void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  
  debugln(upload.status);
  
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) {
      filename = "/"+filename;//Really needed????
    }
    debugln(String("Upload file named: ") + filename);
    
    fsUploadFile = SPIFFS.open(filename, "w");                              //Open the file for writing in SPIFFS (create if it doesn't exist)
    filename = String(); //can be removed?
    
  } else if (upload.status == UPLOAD_FILE_WRITE && fsUploadFile ){
      fsUploadFile.write(upload.buf, upload.currentSize);                   //Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile) {                                                      //If the file was successfully created
      fsUploadFile.close();                                                 //Close the file again
      debugln(String("handleFileUpload Size: ") + upload.totalSize);
      server.sendHeader("Location","/success.html");                        //Redirect the client to the success page
      server.send(303);
      updateUsers();
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
    String filename = "conf.txt";                                           //Get user input for filename

    File download = SPIFFS.open("/conf.txt", "r");
    
    if (!download) {
      server.send(404, "text/plain", "404: file not found!");
      return;
    }
    
    debugln("Start writing file");
    server.sendHeader("Content-Type", "text/text");
    server.sendHeader("Content-Disposition", "attachment; filename="+filename);
    server.sendHeader("Connection", "close");
    server.streamFile(download, "application/octet-stream");
    download.close();
}

/**************************************************************************/
/*!
  @brief    Updates the users from the config file in global memory.
*/
/**************************************************************************/
void updateUsers() {
  /* If there is no file, return 0 users */
  if (!SPIFFS.exists(HTTP_CONFIG_LOCATION)) {
    numberUsers = 0;
    return;
  }

  File configFile = SPIFFS.open(HTTP_CONFIG_LOCATION, "r");
  String line;
  String* user;
  
  /* Extract user information line by line */
  for(uint8_t i = 0; i < MAX_NUMBER_USERS*USER_INFO_LENGTH; i+=USER_INFO_LENGTH) {
      line = configFile.readStringUntil('\n');                              //Read a line from the file
      if (line != "" && line.indexOf(":") != -1) {
          user = parseLine(line);
          users[i] = user[0].c_str();
          users[i+1] = user[1].c_str();
          users[i+2] = user[2].c_str();
      } else {
        numberUsers = i-1;
        break;
      }
      numberUsers = i-1;
  }
  debugln(numberUsers);
  configFile.close();
}

/**************************************************************************/
/*!
  @brief    Parses config file line to user information array.
  @param    line          The string to parse
  @returns  String*       Array of user information
*/
/**************************************************************************/
String* parseLine(String line) {
  static String userInfo[3];

  uint8_t indexForUsername = line.indexOf(":");                             //gets loc of first ":"
  uint8_t indexForPassword = line.indexOf(":", indexForUsername+1);         //gets loc of second ":"

  userInfo[0] = line.substring(0, indexForUsername);                        //Selects xxxx from xxxx:yyyy:zzzz, username
  userInfo[1] = line.substring(indexForUsername+1, indexForPassword);       //Selects yyyy from xxxx:yyyy:zzzz, password
  userInfo[2] = line.substring(indexForPassword+1);                         //Selects zzzz from xxxx:yyyy:zzzz, usertype
  return userInfo;
}
