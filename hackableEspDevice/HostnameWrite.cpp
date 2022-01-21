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
    char hostname[MAX_HOSTNAME_LENGTH]; 
    EEPROM.begin(MAX_HOSTNAME_LENGTH);

    for (uint8_t i = 0; i < MAX_HOSTNAME_LENGTH; i++) {
        EEPROM.get(HOSTNAME_ADRESS+i, hostname[i]);
        if (hostname[i] == 0xFF) {
            break;                                                          //Skips the unreadable chars
		    }
	  }
    
    EEPROM.end();
    return String(hostname);
}

/**************************************************************************/
/*!
  @brief    Writes the new hostname to the EEPROM.
  @param    hostname    char[32] that contains the hostname to be written
*/
/**************************************************************************/
void writeHostname(char hostname[MAX_HOSTNAME_LENGTH]) {
    EEPROM.begin(MAX_HOSTNAME_LENGTH);
    
    for (int i = 0; i< MAX_HOSTNAME_LENGTH; i++){
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
        Serial.println("Data written!");
    } else {
        Serial.println("ERROR! Data not written!");
    }
}
