/*
 * File:      UserHandler.cpp
 * Author:    Luke de Munk & Twenne Elffers
 * Class:     UserHandler
 * Version:   0.1
 * 
 * Class for the http authentication process.
 */
#include "UserHandler.h"

/**************************************************************************/
/*!
  @brief    Constructor.
  @param    server          Webserver object
*/
/**************************************************************************/
UserHandler::UserHandler(ESP8266WebServer *server) {
    _numberUsers = 0;
}

/**************************************************************************/
/*!
  @brief    Updates the users from the config file in workmemory.
*/
/**************************************************************************/
void UserHandler::updateUsers() {
    /* If there is no file, return 0 users */
    if (!SPIFFS.exists(HTTP_CONFIG_LOCATION)) {
        _numberUsers = 0;
        return;
    }

    File configFile = SPIFFS.open(HTTP_CONFIG_LOCATION, "r");
    String line;
    String* user;

    /* Extract user information line by line */
    for(uint8_t i = 0; i < MAX_NUMBER_USERS*USER_INFO_LENGTH; i+=USER_INFO_LENGTH) {
        line = configFile.readStringUntil('\n');                            //Read a line from the file
        if (line != "" && line.indexOf(":") != -1) {
            user = _parseLine(line);
            _users[i] = user[0].c_str();
            _users[i+1] = user[1].c_str();
            _users[i+2] = user[2].c_str();
        } else {
            _numberUsers = i-1;
            break;
        }
        _numberUsers = i-1;
    }
    configFile.close();
}

/**************************************************************************/
/*!
  @brief    Gets users
*/
/**************************************************************************/
String* UserHandler::getUsers() {
    return _users;
}

/**************************************************************************/
/*!
  @brief    Gets number users
*/
/**************************************************************************/
uint8_t UserHandler::getNumberOfUsers() {
    return _numberUsers;
}

/**************************************************************************/
/*!
  @brief    Checks if user has permission.
  @param    permissionLevel 0 = not logged in, 1 = user, 2 = admin
  @param    server          Webserver object
  @return   bool            If user has permission
*/
/**************************************************************************/
bool UserHandler::checkPermission(uint8_t permissionLevel, ESP8266WebServer *server) {
    bool isLoggedIn = false;
    bool hasPermission = false;
    uint8_t userIndex = 0;
    
    if (permissionLevel == PERMISSION_LVL_ALL) {
        return true;
    } else {
        for (uint8_t i = 0; i < _numberUsers; i += 3) {
            if (server->authenticate(_users[i].c_str(), _users[i+1].c_str())) {
                userIndex = i;
                isLoggedIn = true;
                break;
            }
        }

        if (isLoggedIn && atoi(_users[userIndex+2].c_str()) >= permissionLevel) {
            return true;
        }
    }
    return false;
}

/**************************************************************************/
/*!
  @brief    Parses config file line to user information array.
  @param    line          The string to parse
  @return   String*       Array of user information
*/
/**************************************************************************/
String* UserHandler::_parseLine(String line) {
  static String userInfo[3];

  uint8_t indexForUsername = line.indexOf(":");                             //gets loc of first ":"
  uint8_t indexForPassword = line.indexOf(":", indexForUsername+1);         //gets loc of second ":"

  userInfo[0] = line.substring(0, indexForUsername);                        //Selects xxxx from xxxx:yyyy:zzzz, username
  userInfo[1] = line.substring(indexForUsername+1, indexForPassword);       //Selects yyyy from xxxx:yyyy:zzzz, password
  userInfo[2] = line.substring(indexForPassword+1);                         //Selects zzzz from xxxx:yyyy:zzzz, usertype
  return userInfo;
}
