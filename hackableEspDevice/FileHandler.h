/*
 * File:      FileHandler.h
 * Author:    Luke de Munk
 * Class:     name
 * Version:   0.1
 * 
 * Description
 */
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
//#include <Arduino.h>
#include <stdint.h>
#include <ESP8266WebServer.h>                                               //For running the webserver
#include <FS.h>                                                             //For SPIFFS
#include "config.h"                                                         //For the configuration

class FileHandler
{
    public:
        FileHandler(ESP8266WebServer *server);
        void initialize(ESP8266WebServer *server);
    
    private:
        void handleFileUpload();
        void handleFileDownload();
        File _fsUploadFile;                                                          //A File object to temporarily store the received file

};
#endif
