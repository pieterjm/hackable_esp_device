/*
 * File:      Debugger.h
 * Author:    Luke de Munk
 * Version:   1.0
 * 
 * Class for handling the debug prints.
 */
#include "Debugger.h"

/**************************************************************************/
/*!
  @brief    Prints text if debug is enabled.
  @param    text            String of text that needs to be printed
*/
/**************************************************************************/
void debug(String text) {
    EEPROM.begin(1);
    bool isEnabled = EEPROM.read(ENABLE_DEBUG_FLAG_ADDRESS);
    
    if(isEnabled) {
        Serial.print(text);
    }
    EEPROM.end();
}

/**************************************************************************/
/*!
  @brief    Prints text (+'\n') if debug is enabled.
  @param    text            String of text that needs to be printed
*/
/**************************************************************************/
void debugln(String text) {
    EEPROM.begin(1);
    bool isEnabled = EEPROM.read(ENABLE_DEBUG_FLAG_ADDRESS);
    if(isEnabled) {
        Serial.println(text);
    }
    EEPROM.end();
}

/**************************************************************************/
/*!
  @brief    Gets if debug is enabled.
  @return   bool            True if debug is enabled
*/
/**************************************************************************/
bool getDebugEnabled() {
    EEPROM.begin(1);
    bool isEnabled = EEPROM.read(ENABLE_DEBUG_FLAG_ADDRESS);
    EEPROM.end();
    return isEnabled;
}

/**************************************************************************/
/*!
  @brief    Sets if debug is enabled.
  @param    isEnabled       If debug is enabled (true == enabled)
*/
/**************************************************************************/
void setDebugEnabled(bool isEnabled) {
    EEPROM.begin(1);
    EEPROM.write(ENABLE_DEBUG_FLAG_ADDRESS, (uint8_t) isEnabled);           //Set the debug flag
    EEPROM.commit();                                                        //Write to EEPROM
    EEPROM.end();
}
