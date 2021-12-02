/*
 * File:      name.h
 * Author:    Luke de Munk
 * Class:     name
 * Version:   0.1
 * 
 * Description
 */
#ifndef USER_HANDLER
#define USER_HANDLER
//#include <Arduino.h>
#include <stdint.h>
#include "config.h"     //For debugln
#include <ESP8266WebServer.h>                                               //For running the webserver
#include <FS.h>                                                             //For SPIFFS

/* Permission levels */
#define PERMISSION_LVL_ALL      0
#define PERMISSION_LVL_USER     1
#define PERMISSION_LVL_ADMIN    2
#define USER_INFO_LENGTH        3
#define MAX_NUMBER_USERS        10

class UserHandler
{
    public:
        UserHandler(ESP8266WebServer *server);
        void updateUsers();
        bool checkPermission(uint8_t permissionLevel, ESP8266WebServer *server);
    
    private:
        String* _parseLine(String line);
        String _users[MAX_NUMBER_USERS*USER_INFO_LENGTH];
        uint8_t _numberUsers;
};
#endif
