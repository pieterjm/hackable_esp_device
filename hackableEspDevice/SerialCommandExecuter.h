/*
 * File:      SerialCommandExecuter.h
 * Author:    Luke de Munk
 * Class:     SerialCommandExecuter
 * Version:   0.1
 * 
 * Description
 */
#ifndef SERIAL_COMMAND_EXECUTER_H
#define SERIAL_COMMAND_EXECUTER_H
#include <Arduino.h>
#include "config.h"                                                         //For the configuration.

#define MAX_NUMBER_PARAMS  2

#define COMMAND_1       "help"
#define COMMAND_2       "enableDebug"
#define ERROR_1_TEXT    "Too many arguments. Typ 'help' for help"
#define ERROR_2_TEXT    "Wrong command. Typ 'help' for help"
#define ERROR_3_TEXT    "Wrong parameter(s), needs to be 0 or 1. Typ 'help' for help"

class SerialCommandExecuter
{
    public:
        SerialCommandExecuter();
        void executeCommand();
    
    private:
        bool _parseCommand(String command);
        String* _trimCommand(String commandString);
        void _printHelp();
        void _enableDebug(String enable);
};
#endif
