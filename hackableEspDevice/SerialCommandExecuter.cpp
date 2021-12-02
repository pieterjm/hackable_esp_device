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
        _parseCommand(command);
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
        if (numParams < 1) {
            Serial.println(ERROR_3_TEXT);
            return false;
        } else if (numParams > 1) {
          Serial.println(ERROR_1_TEXT);
          return false;
        }
        _enableDebug(params[0]);
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
      Serial.println(ERROR_1_TEXT);
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
        
        while (commandString[c] != ' ' && commandString[c] != '-' && commandString[c] != '\n' && c < commandString.length()) {
          item += commandString[c];
          c++;
        }

        commandItems[paramCounter] = item; //Save param
        item = "";                       //Reset param
        paramCounter++;
    }
    return commandItems;
}

/**************************************************************************/
/*!
  @brief    brief Example command 'enableDebug -1'
  @param    var           desc
*/
/**************************************************************************/
void SerialCommandExecuter::_printHelp() {
  Serial.println("-----------------------HELP-----------------------");
  Serial.println("A command consists of the command and sometimes arguments.");
  Serial.println("Arguments can be given with a '-' prefix, like 'enableDebug -x'.");
}

/**************************************************************************/
/*!
  @brief    brief Example command 'enableDebug -1'
  @param    var           desc
*/
/**************************************************************************/
void SerialCommandExecuter::_enableDebug(String enable) {
  if (enable == "1") {
    //debug = true;
    debugln("debug = true");
  } else if (enable == "0") {
    //debug = false;
    debugln("debug = false");
  } else {
    Serial.println(ERROR_3_TEXT);
  }
}
