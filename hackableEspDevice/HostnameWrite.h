/*
 * File:      HostnameWrite.h
 * Author:    Twenne Elffers
 * Class:     HostnameWrite
 * Version:   0.1
 * 
 * Writes hostname to the EEPROM.
 */
#ifndef HOSTNAME_WRITE_H
#define HOSTNAME_WRITE_H
#include <stdint.h>                                                         //For defining bits per integer
//#include "Arduino.h"
#include <EEPROM.h>                                                       //For reading from and writing to flash memory, used for resetting wifi
#include "Debugger.h"

#define HOSTNAME_ADRESS   1

String getHostname();
void writeHostname(char hostname[32]);
void setEEPROMToNULL(int writeLength, int startAdress);
void checkEepromCommit();
#endif
