/*
 * File:      FileHandler.cpp
 * Author:    Luke de Munk
 * Class:     name
 * Version:   0.1
 * 
 * Description
 */
#include "FileHandler.h"

/**************************************************************************/
/*!
    @brief    Takes care of the webservices like pageloading.
*/
/**************************************************************************/
void initialize(ESP8266WebServer *server) {
    /*
    * Routes for file management
    */
    /* Route for file upload request */
    server->on("/upload", HTTP_POST, []() {
        server->send(200);                                                   //HTTP code 200 == OK
        debugln("Wait, something got uploaded");
      }, handleFileUpload                                                   //Receive and save the file
    );
    /* Route for file upload request */
    server->on("/download", HTTP_POST, []() {
        server->send(200);                                                   //HTTP code 200 == OK
        debugln("Wait, something got downloaded");
      }, handleFileDownload                                                 //Receive and save the file
    );
    /*
    * End of routes for file management
    */
}

/**************************************************************************/
/*!
  @brief    Handles the file upload to the SPIFFS.
*/
/**************************************************************************/
void handleFileUpload(ESP8266WebServer *server) {
    HTTPUpload& upload = server.upload();
    
    debugln(upload.status);
    
    if(upload.status == UPLOAD_FILE_START) {
        String filename = upload.filename;
        if(!filename.startsWith("/")) {
            filename = "/"+filename;//Really needed????
        }
        debugln(String("Upload file named: ") + filename);
        
        fsUploadFile = SPIFFS.open(filename, "w");                              //Open the file for writing in SPIFFS (create if it doesn't exist)
        filename = String(); //can be removed?
        
    } else if (upload.status == UPLOAD_FILE_WRITE && fsUploadFile ) {
        fsUploadFile.write(upload.buf, upload.currentSize);                   //Write the received bytes to the file
    } else if(upload.status == UPLOAD_FILE_END){
        if(fsUploadFile) {                                                      //If the file was successfully created
            fsUploadFile.close();                                                 //Close the file again
            debugln(String("handleFileUpload Size: ") + upload.totalSize);
            server.sendHeader("Location","/success.html");                        //Redirect the client to the success page
            server.send(303);
            userHandler.updateUsers();
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
void handleFileDownload(ESP8266WebServer *server) {
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
