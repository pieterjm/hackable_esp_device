/*
 * File:      HostnameWrite.h
 * Author:    Twenne Elffers
 * Class:     HostnameWrite
 * Version:   0.1
 * 
 * Parses and executes serial terminal commands.
 */
#ifndef HostnameWrite
#define HostnameWrite
#include <Arduino.h>
#include "Arduino.h"
#include <EEPROM.h>                                                       //For reading from and writing to flash memory, used for resetting wifi
#include "Debugger.h"

/* 
 * 1 byte to store the enable debug flag.
 * Is done in EEPROM, because the
 * flag is then non-volatile and can
 * be used by multiple classes. Also 
 * is saved during restart.
 */
#define HOSTNAME_ADRESS   1

String getHostname();
void writeHostname(char hostname[32]);
void setEEPROMToNULL(int writeLenth, int startAdress);
void checkEepromCommit();


#endif