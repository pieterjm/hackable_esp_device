/*
 * File:      SerialCommandExecuter.cpp
 * Author:    Luke de Munk
 * Class:     SerialCommandExecuter
 * Version:   0.1
 * 
 * Description
 */
#include "SerialCommandExecuter.h"

/**************************************************************************/
/*!
  @brief    Constructor
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
SerialCommandExecuter::SerialCommandExecuter() {
  _isLoggedIn = false;
  Serial.println("Serial commands available. Typ 'help' for help.");
}

/**************************************************************************/
/*!
  @brief    brief
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
void SerialCommandExecuter::setUsers(String* users, uint8_t numUsers) {
  /* Copy users */
    for (uint8_t i = 0; i < MAX_NUMBER_USERS*USER_INFO_LENGTH; i++) {
      _users[i] = users[i];
    }
    _numberUsers = numUsers;
}

/**************************************************************************/
/*!
  @brief    brief
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
void SerialCommandExecuter::executeCommand() {
    String command = Serial.readString();
    if (command != "") {
        if (_isLoggedIn) {
          Serial.print("~# ");
        } else {
          Serial.print("~$ ");
        }
        Serial.print(command);//echo command
        if (!_parseCommand(command)) {
            //debugln("Parse error");
        }
    }
}


/**************************************************************************/
/*!
  @brief    brief Example command 'enableDebug -1'
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
bool SerialCommandExecuter::_parseCommand(String commandString) {
    String* trimmedCmdLine = _trimCommand(commandString);
    String command = trimmedCmdLine[0].c_str();
    String params[MAX_NUMBER_PARAMS] = "";
    uint8_t numParams = 0;
    
    while (numParams < MAX_NUMBER_PARAMS) {
      if (trimmedCmdLine[numParams+1] == "") {                              //+1 because the command counts as well
        break;
      }
      numParams++;
    }

    for (uint8_t i = 1; i-1 < numParams; i++){                              //+1 because the command counts as well
        params[i-1] = trimmedCmdLine[i].c_str();
    }

    if (command == COMMAND_1) {
      _printHelp();
    } else if (command == COMMAND_2) {
        if (!_checkParams(numParams, 1) || !_enableDebug(params[0])) {
            return false;
        }
    } else if (command == COMMAND_3) {
        if (!_checkParams(numParams, 1) || !_superUserLogin(params[0])) {
            return false;
        }
    } else if (command == COMMAND_4) {
        if (!_viewKey()) {
          return false;
        }
    } else if (command == COMMAND_5) {
        _restart();
    } else if (command == COMMAND_6) {
        if (!_viewUsers()) {
          return false;
        }
    } else {
        Serial.println(ERROR_2_TEXT);
        return false;
    }
    return true;
}

/**************************************************************************/
/*!
  @brief    Returns function and params
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
String* SerialCommandExecuter::_trimCommand(String commandString) {
    static String commandItems[1+MAX_NUMBER_PARAMS] = "";
    uint16_t i = 0;
    String item = "";
    uint8_t numParams = 1;                        //1 because the command counts as well
    uint8_t paramCounter = 0;
    uint16_t c = 0;
    
    /* Reset static array */
    for (uint16_t x = 0; x < 1+MAX_NUMBER_PARAMS; x++) {
      commandItems[x] = "";
    }
    
    /* Count number of parameters */
    for (uint16_t c = 0; c < commandString.length(); c++) {
      if (commandString[c] == '-') {
        numParams++;
      }
    }
    
    if (numParams-1 > MAX_NUMBER_PARAMS) {
      //Serial.println(ERROR_1_TEXT);
      numParams == MAX_NUMBER_PARAMS;
    }

    while (paramCounter < numParams) {
        /* If c == 0, the command is extracted */
        if (c != 0) {
            if (commandString.indexOf('-', c) != -1) {
                c = commandString.indexOf('-', c) + 1;
            } else {
              break;
            }
        }
        
        while (commandString[c] != ' ' && commandString[c] != '-' && commandString[c] != '\n'/* && c < commandString.length()*/) {
          item += commandString[c];
          c++;
        }

        commandItems[paramCounter] = item; //Save param
        item = "";                       //Reset param
        paramCounter++;
    }
    return commandItems;
}

bool SerialCommandExecuter::_checkParams(uint8_t numParams, uint8_t checkValue) {
  if (numParams < checkValue) {
      Serial.println(ERROR_4_TEXT);
      return false;
  } else if (numParams > checkValue) {
    Serial.println(ERROR_1_TEXT);
    return false;
  }
  return true;
}

/**************************************************************************/
/*!
  @brief    brief Example command 'enableDebug -1'
  @param    var           desc
*/
/**************************************************************************/
void SerialCommandExecuter::_printHelp() {
  Serial.println("|---------------------------HELP---------------------------|");
  Serial.println("A command consists of the command and sometimes arguments.");
  Serial.println("Arguments can be given with a '-' prefix, like");
  Serial.println("'enableDebug -x'.");
  Serial.println("");
  Serial.println("Available commands:");
  Serial.println("help");
  Serial.println("enableDebug -x -> x = 0 = off, x = 1 = on");
  Serial.println("su -password");
  Serial.println("viewKey");
}

/**************************************************************************/
/*!
  @brief    brief Example command 'enableDebug -1'
  @param    var           desc
*/
/**************************************************************************/
bool SerialCommandExecuter::_enableDebug(String enable) {
  if (enable == "1") {
    //debug = true;
    debugln("debug = true");
  } else if (enable == "0") {
    //debug = false;
    debugln("debug = false");
  } else {
    Serial.println(ERROR_3_TEXT);
    return false;
  }
  return true;
}

/**************************************************************************/
/*!
  @brief    brief Example command 'enableDebug -1'
  @param    var           desc
*/
/**************************************************************************/
bool SerialCommandExecuter::_superUserLogin(String password) {
  if (password == "pw") {
    _isLoggedIn = true;
    debugln("You are now super user.");
  } else {
    Serial.println(ERROR_5_TEXT);
    return false;
  }
  return true;
}

/**************************************************************************/
/*!
  @brief    brief Example command 'enableDebug -1'
  @param    var           desc
*/
/**************************************************************************/
bool SerialCommandExecuter::_viewKey() {
  if (!_isLoggedIn) {
      Serial.println(ERROR_6_TEXT);
      return false;
  }
  Serial.println("Encryption key for config.conf");
  return true;
}

/**************************************************************************/
/*!
  @brief    brief Example command 'enableDebug -1'
  @param    var           desc
*/
/**************************************************************************/
void SerialCommandExecuter::_restart() {
  Serial.print("Restarting in ");
  for (uint8_t s = 3; s > 0; s--) {
    Serial.print(s);
    Serial.print(" ");
    delay(1000);
  }
  ESP.restart();
}

/**************************************************************************/
/*!
  @brief    brief Example command 'enableDebug -1'
  @param    var           desc
*/
/**************************************************************************/
bool SerialCommandExecuter::_viewUsers() {
  String userPrints[USER_INFO_LENGTH] = "";
    
  if (!_isLoggedIn) {
      Serial.println(ERROR_6_TEXT);
      return false;
  }
  
  Serial.println("|-USERNAME------|-PASSWORD------|-ROLE--|");

  for (uint8_t i = 0; i < _numberUsers; i += 3) {
      userPrints[0] = _users[i].c_str();                      //username
      if(atoi(_users[i+2].c_str()) == PERMISSION_LVL_USER) {
          userPrints[1] = _users[i+1].c_str();                //password
          userPrints[2] = "User";
      } else if (atoi(_users[i+2].c_str()) == PERMISSION_LVL_ADMIN) {
          userPrints[1] = "******";
          userPrints[2] = "Admin";
      }
      Serial.printf("| %s\t| %s\t| %s\t|\n", userPrints[0].c_str(), userPrints[1].c_str(), userPrints[2].c_str());
  }
  return true;
}
