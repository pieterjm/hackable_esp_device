/*
 * File:      SerialCommandExecuter.cpp
 * Author:    Luke de Munk & Twenne Elffers
 * Class:     SerialCommandExecuter
 * Version:   0.1
 * 
 * Parses and executes serial terminal commands.
 */
#include "SerialCommandExecuter.h"

/**************************************************************************/
/*!
  @brief    Constructor.
*/
/**************************************************************************/
SerialCommandExecuter::SerialCommandExecuter() {
    _isLoggedIn = false;
}

/**************************************************************************/
/*!
  @brief    Sets the users for user list.
  @param    users         Array of the users
  @return   numUsers      Number of users
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
  @brief    Reads the commands and sends them to the parser.
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
        Serial.print(command);                                              //Echo command
        
        if (!_parseCommand(command)) {
            debugln("Parse error");
        }
    }
}

/**************************************************************************/
/*!
  @brief    Looks at the command and parameters and executes when valid.
  @param    commandString String with command and parameters
  @return   bool          true == success, false == error
*/
/**************************************************************************/
bool SerialCommandExecuter::_parseCommand(String commandString) {
    String* trimmedCmdLine = _trimCommand(commandString);
    String command = trimmedCmdLine[0].c_str();
    String params[MAX_NUMBER_PARAMS] = {""};
    uint8_t numParams = 0;
    
    while (numParams < MAX_NUMBER_PARAMS) {
        if (trimmedCmdLine[numParams+1] == "") {                            //+1, because the command is in the first cell
            break;
        }
        numParams++;
    }

    for (uint8_t i = 1; i-1 < numParams; i++){                              //+1, because the command is in the first cell
        params[i-1] = trimmedCmdLine[i].c_str();
    }

    /* Check which command is given */
    if (command == COMMAND_1) {
        _printHelp(COMMAND_1);
    } else {
        /* If help needs to be printed, print it and return */
        if (_checkHelp(params[0], command)) {
          return true;
        }
    }
    
    if (command == COMMAND_2) {
        if (!_checkParams(numParams, 1, 1) || !_enableDebug(params[0])) {
            return false;
        }
    } else if (command == COMMAND_3) {
        if (!_checkParams(numParams, 1, 1) || !_superUserLogin(params[0])) {
            return false;
        }
    } else if (command == COMMAND_4) {
        if (!_viewKey()) {
            return false;
        }
    } else if ((command == COMMAND_5)) {
        _restart();
    } else if (command == COMMAND_6) {
        if (!_viewUsers()) {
          return false;
        }
    } else if (command == COMMAND_7) {
        if (!_checkParams(numParams, 0, 2) || !_hostname(params)) {
          return false;
        }
    } else if (command == COMMAND_8) {
        ls();
    } else if (command == COMMAND_9) {
        if (_checkParams(numParams, 1, 1)) {
          if (params[0] == "./testprogram.c" || params[0] == "testprogram.c") {
            vi();
          } else {
              Serial.println(ERROR_7_TEXT);
              return false;
          }
        }
    } else if (command == COMMAND_10) {
        if (_checkParams(numParams, 0, 1)) {
          if (numParams == 1) {
            /* If buffer overflow is done correctly,
             * user is logged in.
             */
            if (runCProgram(params[0])) {
              _isLoggedIn = true;
              Serial.println("You are now super user.");
            }
          } else {
            runCProgram("");
          }
        }
    } else if (command == COMMAND_11) {
      if (_checkParams(numParams, 2, 2)) {
        if (params[0] != "-d") {
            Serial.println(ERROR_3_TEXT);
            return false;
        }
          if (params[1] == "./testprogram" || params[1] == "testprogram") {
            objectDump();
          } else {
              Serial.println(ERROR_7_TEXT);
            return false;
          }
        }
    } else {
        Serial.println(ERROR_2_TEXT);
        return false;
    }
    return true;
}

/**************************************************************************/
/*!
  @brief    Divides the commandstring to command and parameters.
  @param    commandString String with command and parameters
  @return   String array  Array with first element the command and then params
*/
/**************************************************************************/
String* SerialCommandExecuter::_trimCommand(String commandString) {
    static String commandItems[1+MAX_NUMBER_PARAMS] = {""};                 //To save command and parameters, each in own cell
    String item = "";                                                       //Can be a command or parameter
    uint8_t paramCounter = 0;
    
    /* Reset static array */
    for (uint16_t x = 0; x < 1+MAX_NUMBER_PARAMS; x++) {
        commandItems[x] = "";
    }
    
    /* Count number of parameters by adding to temp variable if not a whitespace or end of line*/
    for (uint16_t c = 0; c < commandString.length(); c++) {
        if (commandString[c] == ' ' || commandString[c] == '\n') {
            /* If item is not empty: add to item array */
            if (item != ""){                                                      
                commandItems[paramCounter] = item;                          //Save param to items list
                item = "";                                                  //Reset item value
                paramCounter++;
           }
        } else { // if not a whitepace add to item
            item += commandString[c];
        }
    }
    return commandItems;
}

/**************************************************************************/
/*!
  @brief    Checks if parameters are valid.
  @param    numParams     Number of given parameters
  @param    checkValue    Number of needed parameters
  @return   bool          true == valid, false == error
*/
/**************************************************************************/
bool SerialCommandExecuter::_checkParams(uint8_t numParams, uint8_t minNumberParams, uint8_t maxNumberParams) {
  if (numParams < minNumberParams) {
      Serial.println(ERROR_4_TEXT);
      return false;
  } else if (numParams > maxNumberParams) {
      Serial.println(ERROR_8_TEXT);
      return false;
  }
  return true;
}

/**************************************************************************/
/*!
  @brief    Gives help based on command put in
  @param    command       contains the command that help is given about
*/
/**************************************************************************/
void SerialCommandExecuter::_printHelp(String command) {
    /* Print help lines according to command */
    if (command == "" || command == COMMAND_1) {                            //Default help
        Serial.println("|---------------------------HELP---------------------------|");
        Serial.println("This is a commandline interface that allows access to the smartlight config");
        _printCommands();
    } else if (command == COMMAND_2) {
        Serial.println("Usage: enableDebug [--off]              Turns the debug off");
        Serial.println("       enableDebug [--on]               Turns the debug on");
    } else if (command == COMMAND_3) {
      Serial.println("Usage: su {passwd}                        Login as superuser");
    } else if (command == COMMAND_4) {
        Serial.println("Usage: sshkeys                          Shows ssh keys");
    } else if (command == COMMAND_5) {
        Serial.println("Usage: reboot                           Reboots the device");
    } else if (command == COMMAND_6) {
        Serial.println("Usage: users                            Shows usertable of website");
    } else if (command == COMMAND_7) {
        Serial.println("Usage: hostname                         Gives the current hostname");
        Serial.println("       hostname [--set] {newhostname}   Set new hostname. (needs reboot)");
        Serial.println("       hostname [-i]                    Gives the current ip-address");
        Serial.println("       hostname [--default]             Sets the hostname to the default hostname");
    } else if (command == COMMAND_8) {
        Serial.println("Usage: ls                               Shows files in current folder");
    } else if (command == COMMAND_9) {
        Serial.println("Usage: vi {filename}                    Opens file in text editor");
    }else if (command == COMMAND_10) {
        Serial.println("Usage: ./{filename}                     Runs an executable file");
    } else if (command == COMMAND_11) {
        Serial.println("Usage: objdump -d {filename}            Prints disassembled code of an executable file");
    } else {
        Serial.println(ERROR_2_TEXT);
    }
}

/**************************************************************************/
/*!
  @brief    Prints available commands.
*/
/**************************************************************************/
void SerialCommandExecuter::_printCommands() {
    Serial.println("Available commands:");
    Serial.println(COMMAND_1);
    Serial.println(COMMAND_2);
    Serial.println(COMMAND_3);
    Serial.println(COMMAND_4);
    Serial.println(COMMAND_5);
    Serial.println(COMMAND_6);
    Serial.println(COMMAND_7);
    Serial.println(COMMAND_8);
    Serial.println(COMMAND_9);
    Serial.println(COMMAND_11);
}

/**************************************************************************/
/*!
  @brief    Executes 'debugEnable [--off|--on]' command, enables or disables
            debug info.
  @param    enable        If the debug is enabled
  @return   bool          true == success, false == error
*/
/**************************************************************************/
bool SerialCommandExecuter::_enableDebug(String enable) {
    if (enable == "--on") {
        setDebugEnabled(true);
        Serial.println("debug = true");
    } else if (enable == "--off") {
        setDebugEnabled(false);
        Serial.println("debug = false");
    } else {
        Serial.println(ERROR_3_TEXT);
        return false;
    }
    return true;
}

/**************************************************************************/
/*!
  @brief    Executes 'su {passwrd' command, checks authentication.
  @param    password      Password for the superuser
  @return   bool          true == success, false == error
*/
/**************************************************************************/
bool SerialCommandExecuter::_superUserLogin(String password) {
    if (password == ROOT_PASSWORD) {
        _isLoggedIn = true;
        Serial.println("You are now super user.");
    } else {
        Serial.println(ERROR_5_TEXT);
        return false;
    }
    return true;
}

/**************************************************************************/
/*!
  @brief    Executes 'viewKey' command, prints encryption keys.
  @return   bool          true == success, false == error
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
  @brief    Executes 'restart' command, restarts the ESP.
*/
/**************************************************************************/
void SerialCommandExecuter::_restart() {
    Serial.print("Restarting in ");

    /* Wait 3 seconds */
    for (uint8_t s = 3; s > 0; s--) {
        Serial.print(s);
        Serial.print(" ");
        delay(1000);
    }
    ESP.restart();
}

/**************************************************************************/
/*!
  @brief    Executes 'users' command, prints userlist of webpage.
  @return   bool          true == success, false == error
*/
/**************************************************************************/
bool SerialCommandExecuter::_viewUsers() {
    String userPrints[USER_INFO_LENGTH] = {""};
      
    if (!_isLoggedIn) {
        Serial.println(ERROR_6_TEXT);
        return false;
    }
    
    Serial.println("|-USERNAME------|-PASSWORD------|-ROLE--|");
  
    for (uint8_t i = 0; i < _numberUsers; i += 3) {
        userPrints[0] = _users[i].c_str();                                  //Username
        if(atoi(_users[i+2].c_str()) == PERMISSION_LVL_USER) {
            userPrints[1] = _users[i+1].c_str();                            //Password
            userPrints[2] = "User";                                         //Permission level/role
        } else if (atoi(_users[i+2].c_str()) == PERMISSION_LVL_ADMIN) {
            userPrints[1] = "******";                                       //Password, not printed
            userPrints[2] = "Admin";                                        //Permission level/role
        }
        Serial.printf("| %s\t| %s\t| %s\t|\n", userPrints[0].c_str(), userPrints[1].c_str(), userPrints[2].c_str());
    }
    return true;
}

/**************************************************************************/
/*!
  @brief    Hostname options.
  @param    params        Array of the parameters
  @return   bool          true == success, false == error
  TODO:     add -i as option to obtain ip addr
*/
/**************************************************************************/
bool SerialCommandExecuter::_hostname(String* params) { 
    uint8_t numParams = params->length();
    if (numParams == 0) {                                                   //If empty: show hostname
      Serial.print("Hostname is: ");
      Serial.println(String(getHostname()));
      return true;
    }
    
    if (params[0] == "--set" && params[1] != "") {                        //If parameter == "--set" check if next value is not empty
        char newHostname[MAX_HOSTNAME_LENGTH];
        params[1].toCharArray(newHostname, MAX_HOSTNAME_LENGTH);
        writeHostname(newHostname);
    } else if (params[0] == "--default"){
        writeHostname(DEFAULT_HOSTNAME);
    } else {
        Serial.println(ERROR_3_TEXT);                                       //If it can't find suitable params: give error
        return false;
    }
    return true;
}

/**************************************************************************/
/*!
  @brief    Checks if the -h or --help is used in a command and sends to
            printHelp for the command.
  @param    param         Parameter that needs to be checked
  @param    command       Command that help might be needed for
  @return   bool          true == help menu, false == no help menu
*/
/**************************************************************************/
bool SerialCommandExecuter::_checkHelp(String param, String command) {
    if (param == "-h" || param == "--help") {
        _printHelp(command);
        return true;
    }
    return false;
}
