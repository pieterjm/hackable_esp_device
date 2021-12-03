/*
 * File:      UserHandler.h
 * Author:    Luke de Munk
 * Class:     UserHandler
 * Version:   0.1
 * 
 * Class for the http authentication process.
 */
#ifndef USER_HANDLER
#define USER_HANDLER
#include <stdint.h>
#include <ESP8266WebServer.h>                                               //For running the webserver
#include <FS.h>                                                             //For SPIFFS
#include "config.h"                                                         //For the configuration

class UserHandler
{
    public:
        UserHandler(ESP8266WebServer *server);
        void updateUsers();
        String* getUsers();
        uint8_t getNumberOfUsers();
        bool checkPermission(uint8_t permissionLevel, ESP8266WebServer *server);
    
    private:
        String* _parseLine(String line);
        String _users[MAX_NUMBER_USERS*USER_INFO_LENGTH];
        uint8_t _numberUsers;
};
#endif
