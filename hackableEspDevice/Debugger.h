/*
 * File:      Debugger.h
 * Author:    Luke de Munk
 * Version:   0.1
 * 
 * Class for handling the debug prints.
 */
#ifndef DEBUGGER_H
#define DEBUGGER_H
#include <stdint.h>                                                         //For defining bits per integer
#include "Arduino.h"
#include <EEPROM.h>                                                       //For reading from and writing to flash memory, used for resetting wifi

/* 
 * 1 byte to store the enable debug flag.
 * Is done in EEPROM, because the
 * flag is then non-volatile and can
 * be used by multiple classes. Also 
 * is saved during restart.
 */
#define ENABLE_DEBUG_FLAG_ADDRESS   0

void debug(String text);
void debugln(String text);
bool getDebugEnabled();
void setDebugEnabled(bool isEnabled);

#endif
