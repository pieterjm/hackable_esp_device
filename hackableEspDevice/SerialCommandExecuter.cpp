/*
 * File:      SerialCommandExecuter.cpp
 * Author:    Luke de Munk
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
    String* trimmedLessCmdLine =_trimLessCommand(commandString);
    String command = trimmedCmdLine[0].c_str();
    String params[MAX_NUMBER_PARAMS] = {""};
    uint8_t numParams = 0;
    
    while (numParams < MAX_NUMBER_PARAMS) {
      if (trimmedCmdLine[numParams+1] == "") {                              //+1, because the command is in the first cell
        break;
      }
      numParams++;
    }

    for (uint8_t i = 1; i-1 < numParams; i++){                              //+1, because the command is in the first cell
        params[i-1] = trimmedCmdLine[i].c_str();
    }

    /* Check which command is given */
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
    } else if (command == COMMAND_7) {
        if (!_hostname(trimmedLessCmdLine)) {
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
  @brief    Divides the commandstring to command and parameters.
  @param    commandString String with command and parameters
  @return   String array  Array with first element the command and then params
*/
/**************************************************************************/
String* SerialCommandExecuter::_trimCommand(String commandString) {
    static String commandItems[1+MAX_NUMBER_PARAMS] = {""};                   //To save command and parameters, each in own cell
    String item = "";                                                       //Can be a command or parameter
    uint8_t numParams = 1;                                                  //1, because the command counts as well
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
      numParams = MAX_NUMBER_PARAMS;
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
        while (commandString[c] != ' ' && commandString[c] != '-' && commandString[c] != '\n') {
          item += commandString[c];
          c++;
        }
        commandItems[paramCounter] = item;                                  //Save param to items list
        item = "";                                                          //Reset param
        paramCounter++;
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
  @brief    Prints help page to serial terminal.
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
  @brief    Executes 'debugEnable -x' command, enables or disables debug info.
  @param    enable        If the debug is enabled
  @return   bool          true == success, false == error
*/
/**************************************************************************/
bool SerialCommandExecuter::_enableDebug(String enable) {
    if (enable == "1") {
        setDebugEnabled(true);
        Serial.println("debug = true");
    } else if (enable == "0") {
        setDebugEnabled(false);
    } else {
        Serial.println(ERROR_3_TEXT);
        return false;
    }
    return true;
}

/**************************************************************************/
/*!
  @brief    Executes 'su -password' command, checks authentication.
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
  @param    var           desc
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
  @brief    Executes 'viewUsers' command, prints userlist of webpage.
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
  @brief    Does something with host name
  @param    trimmedCmdLine        contains the parameters from the command
  @param    trimmedCmdLine         hostname -set <hostname>     set hostname to <hostname>
  @param    trimmedCmdLine         hostname -i                  Return ip addr
  @return   bool          true == success, false == error
*/
/**************************************************************************/
bool SerialCommandExecuter::_hostname(String* trimmedCmdLine) {
    String command = trimmedCmdLine[0].c_str();
    String params[MAX_NUMBER_PARAMS] = {""};
    uint8_t numParams = 0;
    while (numParams < MAX_NUMBER_PARAMS) {
      if (trimmedCmdLine[numParams+1] == "") {                              //+1, because the command is in the first cell
        break;
      }
      numParams++;
    }

    for (uint8_t i = 1; i-1 < numParams; i++){                              //+1, because the command is in the first cell
        params[i-1] = trimmedCmdLine[i].c_str();
    }
    
    if (numParams == 0) { // if empty show hostname
      String hostname = getHostname();
      Serial.print("Hostname is: ");
      Serial.println(String(hostname));
      return true;
    } else {
        // for (int i =0; i < MAX_NUMBER_PARAMS;i++){ //loops through all params and prints
        //   //Serial.println(params[i]);
        //   String param = params[i];

        //   }
        if (params[0] == "--set"&& params[1] != "") { //if parameter = "--set-hostname" check if next value is not empty
            
            char newhostname[32];
            params[1].toCharArray(newhostname, 32);
            writeHostname(newhostname);
            debug(trimmedCmdLine[2]);
          
        }
        else if (params[0] == "--null"){ // if parameter = -null set hostname field to null
          setEEPROMToNULL(32, HOSTNAME_ADRESS);
        }
        else if (params[0]=="-h" | params[0]=="--help"){ // if -h or --help give help
              _giveHelp("hostname");
        }
        else{ //if it can't find give error 
          Serial.println(ERROR_7_TEXT);
        }

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
String* SerialCommandExecuter::_trimLessCommand(String commandString) {
    static String commandItems[1+MAX_NUMBER_PARAMS] = {""};                   //To save command and parameters, each in own cell
    String item = "";                                                       //Can be a command or parameter
    uint8_t numParams = 1;                                                  //1, because the command counts as well
    uint8_t paramCounter = 0;
    uint16_t c = 0;
    
    /* Reset static array */
    for (uint16_t x = 0; x < 1+MAX_NUMBER_PARAMS; x++) {
      commandItems[x] = "";
    }
    
    /* Count number of parameters by adding to temp variable if not a whitespace or end of line*/
    for (uint16_t c = 0; c < commandString.length(); c++) {
      if (commandString[c] == ' ') {// check for  whitespace
        if (item != ""){  // if item is empty does not add so whitepace is not added to item
          commandItems[paramCounter] = item;                                  //Save param to items list
          item = "";
          paramCounter++;
       }
      } else if (commandString[c] == '\n') { // check if end of line
          if (item != ""){ // if item is empty does not add so whitepace is not added to item
            commandItems[paramCounter] = item;                                  //Save param to items list
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
  @brief    Gives help based on command put in
  @param    command       contains the command that help is given about
*/
/**************************************************************************/
void SerialCommandExecuter::_giveHelp(String command) {
    const int commandCount = 6;
    String commandsList[commandCount] = {"help","enableDebug","su", "viewKey", "hostname", "restart" }; 
    if (command == ""){
          Serial.println("|---------------------------HELP---------------------------|");
          Serial.println("This is a commandline interface that allows access to the smartlight config");
          Serial.println("Available commands:");
          for (int i=0; i<commandCount;i++){ // loops through commands
            Serial.println(commandsList[i]);
          }
    }
    if (command = "hostname"){
        Serial.println("Usage: hostname [--set] {newhostname}   set new hostname for next boot");
        Serial.println("       hostname [--null]                reset hostname to null");
        Serial.println("       hostname                         gives the current hostname");
        Serial.println("       hostname [-i]                    gives the current ipaddr");
    }
}
