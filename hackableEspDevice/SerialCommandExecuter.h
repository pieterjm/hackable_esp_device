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
#include "config.h"                                                         //For the configuration
#include "Debugger.h"                                                       //For handling debug messages
#include "HostnameWrite.h"
#include "BufferOverflow.h"

#define MAX_NUMBER_PARAMS  2

#define COMMAND_HELP            "help"
#define COMMAND_DEBUG           "debug"
#define COMMAND_SU              "su"
#define COMMAND_KEYS            "privatekeys"
#define COMMAND_RESTART         "reboot"
#define COMMAND_USERS           "users"
#define COMMAND_HOSTNAME        "hostname"
#define COMMAND_WHOAMI          "whoami"

/* Used for buffer overflow */
#define COMMAND_LS              "ls"
#define COMMAND_VI              "vi"
#define COMMAND_RUN             "./"
#define COMMAND_OBJDUMP         "objdump"

#define MESS_SUPER_USER         "You are now super user."

#define ERROR_TOO_MANY_ARGS     "Too many arguments. Typ 'help' for help."
#define ERROR_CMD_NOT_FOUND     "Bash: command not found. Typ 'help' for help."
#define ERROR_PERM_DENIED       "Bash: Permission denied"
#define ERROR_WRONG_ARGS        "Wrong argument(s). Add '-h' or '--help' to the command for help."
#define ERROR_TOO_FEW_ARGS      "Too few arguments. Typ 'help' for help."
#define ERROR_WRONG_PWD         "Wrong password."
#define ERROR_NO_PERMISSION     "You are no super user. Use 'su {password}' to log in."
#define ERROR_NO_FILE           "No such file."
#define ERROR_NO_FILE_DIR       "No such file or directory."

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
