/*
 * File:      SerialCommandExecuter.h
 * Author:    Luke de Munk & Twenne Elffers
 * Class:     SerialCommandExecuter
 * Version:   0.1
 * 
 * Parses and executes serial terminal commands.
 */
#ifndef SERIAL_COMMAND_EXECUTER_H
#define SERIAL_COMMAND_EXECUTER_H
#include "Arduino.h"
#include "config.h"                                                         //For the configuration.
#include "Debugger.h"                                                       //For handling debug messages
#include "HostnameWrite.h"
#include "BufferOverflow.h"

#define MAX_NUMBER_PARAMS  2

#define COMMAND_1       "help"
#define COMMAND_2       "enableDebug"
#define COMMAND_3       "su"
#define COMMAND_4       "sshkeys"
#define COMMAND_5       "reboot"
#define COMMAND_6       "users"
#define COMMAND_7       "hostname"

/* Used for buffer overflow */
#define COMMAND_8       "ls"
#define COMMAND_9       "vi"
#define COMMAND_10      "./testprogram"
#define COMMAND_11      "objdump"

#define ERROR_1_TEXT    "Too many arguments. Typ 'help' for help."
#define ERROR_2_TEXT    "Bash: command not found. Typ 'help' for help."
#define ERROR_3_TEXT    "Wrong argument(s). Add '-h' or '--help' to the command for help."
#define ERROR_4_TEXT    "Too few arguments. Typ 'help' for help."
#define ERROR_5_TEXT    "Wrong password."
#define ERROR_6_TEXT    "You are no super user. Use 'su {password}' to log in."
#define ERROR_7_TEXT    "Not able to open this file."
#define ERROR_8_TEXT    "Too many arguments. Typ 'help' for help."

class SerialCommandExecuter
{
    public:
        SerialCommandExecuter();
        void executeCommand();
        void setUsers(String* users, uint8_t numUsers);
    
    private:
        bool _parseCommand(String command);
        String* _trimCommand(String commandString);
        bool _checkParams(uint8_t numParams, uint8_t minNumberParams, uint8_t maxNumberParams);

        void _printHelp(String command);
        void _printCommands();
        bool _enableDebug(String enable);
        bool _superUserLogin(String password);
        bool _viewKey();
        void _restart();
        bool _viewUsers();
        bool _hostname(String* params);
        bool _checkHelp(String param, String command);
         
        bool _isLoggedIn;
        String _users[MAX_NUMBER_USERS*USER_INFO_LENGTH];
        uint8_t _numberUsers;
        BufferOverflow buffOverflow;
};
#endif
