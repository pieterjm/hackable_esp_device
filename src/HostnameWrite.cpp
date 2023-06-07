/*
 * File:      HostnameWrite.cpp
 * Author:    Twenne Elffers
 * Version:   1.0
 * 
 * Handles hostname saving and reading to EEPROM.
 */
#include "HostnameWrite.h"

/**************************************************************************/
/*!
  @brief    Gets the hostname from the EEPROM.
  @return   String          Current hostname
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
  @param    hostname        String that contains the hostname to be written
*/
/**************************************************************************/
void writeHostname(String hostname) {
    EEPROM.begin(MAX_HOSTNAME_LENGTH);
    
    for (uint8_t i = 0; i< MAX_HOSTNAME_LENGTH; i++) {
        if ( i < hostname.length() ) {
            EEPROM.write(HOSTNAME_ADRESS+i, hostname[i]);
        } else {
            EEPROM.write(HOSTNAME_ADRESS+i,0);
        }
        yield();
    }
    
    checkEepromCommit();
    EEPROM.end();
}

/**************************************************************************/
/*!
  @brief    Resets the given EEPROM adresses.
  @param    writeLength     Length to be erased
  @param    startAdress     Start adress
*/
/**************************************************************************/
void setEepromToNull(uint8_t writeLength, uint8_t startAdress) {
    EEPROM.begin(writeLength);
    
    for (uint8_t i = 0; i < writeLength; i++){
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
