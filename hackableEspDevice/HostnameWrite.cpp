/*
 * File:      HostnameWrite.cpp
 * Author:    Twenne Elffers
 * Class:     HostnameWrite
 * Version:   0.1
 * 
 * Writes hostname to the EEPROM.
 */
#include "HostnameWrite.h"

/**************************************************************************/
/*!
  @brief    Gets the hostname from the EEPROM.
  @return   hostname       contains the hostname from eeprom (true == enabled)
*/
/**************************************************************************/
String getHostname() {
    char hostname[32]; 
    EEPROM.begin(32);

    for (uint8_t i = 0; i < 32; i++){
        EEPROM.get(HOSTNAME_ADRESS+i, hostname[i]);
    }
    
    EEPROM.end();
    return String(hostname);
}

/**************************************************************************/
/*!
  @brief    Writes the new hostname to the EEPROM.
  @param    hostname[32]    char that contains the hostname to be written
*/
/**************************************************************************/
void writeHostname(char hostname[32]) {
    EEPROM.begin(32);
    
    for (int i = 0; i< 32; i++){
        EEPROM.write(HOSTNAME_ADRESS+i, hostname[i]);
        yield();
    }
    
    checkEepromCommit();
    EEPROM.end();
}

/**************************************************************************/
/*!
  @brief    Resets the EEPROM at the startAdress.
  @param    writeLength      int of total lenth to be writen
  @param    startAdress     int of start adress
*/
/**************************************************************************/
void setEEPROMToNULL(int writeLength, int startAdress){ 
    EEPROM.begin(writeLength);
    
    for (int i = 0; i< writeLength; i++){
        EEPROM.write(startAdress+i, 0);
        yield();
    }
    
    checkEepromCommit();
    
    debug("Reset Value at: ");
    debug(String(startAdress));
    debug(" till ");
    debugln(String(startAdress+writeLength));
    
    EEPROM.end();
}

/**************************************************************************/
/*!
  @brief    Checks if the eeprom was actually committed.
*/
/**************************************************************************/
void checkEepromCommit() {
    if (EEPROM.commit()) {
        debugln("Data written!");
    } else {
        debugln("ERROR! Data not written!");
    }
}
