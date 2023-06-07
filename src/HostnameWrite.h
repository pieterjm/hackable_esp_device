/*
 * File:      HostnameWrite.h
 * Author:    Twenne Elffers
 * Version:   1.0
 * 
 * Handles hostname saving and reading to EEPROM.
 */
#ifndef HOSTNAME_WRITE_H
#define HOSTNAME_WRITE_H
#include <stdint.h>                                                         //For defining bits per integer
#include <EEPROM.h>                                                         //For reading from and writing to EEPROM
#include "Debugger.h"                                                       //For handling debug messages
#include "config.h"                                                         //For the configuration

String getHostname();
void writeHostname(String hostname);
void setEepromToNull(uint8_t writeLength, uint8_t startAdress);
void checkEepromCommit();
#endif
