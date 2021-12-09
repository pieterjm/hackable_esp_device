/*
 * File:      HostnameWrite.cpp
 * Author:    Twenne Elffers
 * Class:     HostnameWrite
 * Version:   0.1
 * 
 * Parses and executes serial terminal commands.
 */
#include "HostnameWrite.h"


/**************************************************************************/
/*!
  @brief    Gets the hostname from the EEPROM.
  @return   hostname       contains the hostname from eeprom (true == enabled)
*/
/**************************************************************************/
String getHostname(){
    char hostname[32]; 
    int lenHost = sizeof(hostname);
    EEPROM.begin(32);
    String hostnameString;
    for (int i = 0; i< 32; i++){
        EEPROM.get(HOSTNAME_ADRESS+i,hostname[i]);
        //Serial.println(hostname[i]);
    }
    EEPROM.end();
    //debug("Stored hostname:");
    //debugln(hostname);
    hostnameString = String(hostname);
    return hostnameString;
};

/**************************************************************************/
/*!
  @brief    Writes the new hostname to the EEPROM.
  @param    hostname[32]    char that contains the hostname to be writen
*/
/**************************************************************************/
void writeHostname(char hostname[32]){
    EEPROM.begin(32);
    // for (int i = 0; i< 32; i++){
    //     EEPROM.write(HOSTNAME_ADRESS+i, hostname[i]);
    //     Serial.print(EEPROM.read(HOSTNAME_ADRESS+i));
    //     delay(100);
    // }
    for (int i = 0; i< 32; i++){
        EEPROM.write(HOSTNAME_ADRESS+i, hostname[i]);
        yield();
    }
    checkEepromCommit();

    // Serial.println(" Done Writing");
    // debug("Hostname writen: ");
    // debugln(String(hostname));
    // if (EEPROM.commit()){
    //   Serial.print("I can still read this: ");
    //   EEPROM.begin(32);
    //   for (int i = 0; i< 32; i++){
    //       Serial.print(EEPROM.read(HOSTNAME_ADRESS+i));
    //       //Serial.println(hostname[i]);
    //   }
    // }
    // Serial.print("\n");
    EEPROM.end();
  };


/**************************************************************************/
/*!
  @brief    Resets the EEPROM at the startAdress.
  @param    writeLenth      int of total lenth to be writen
  @param    startAdress     int of start adress
*/
/**************************************************************************/
void setEEPROMToNULL(int writeLenth, int startAdress){ 
    EEPROM.begin(writeLenth);
    for (int i = 0; i< writeLenth; i++){
        EEPROM.write(startAdress+i, 0);
        yield();
    }
    checkEepromCommit();
    debug("Reset Value at: ");
    debug(String(startAdress));
    debug(" till ");
    debugln(String(startAdress+writeLenth));
    EEPROM.end();
    getHostname();
};

/**************************************************************************/
/*!
  @brief    Checks if the eeprom was actually committed.
*/
/**************************************************************************/
void checkEepromCommit() {
  if (EEPROM.commit()) {
    debugln("EEPROM successfully committed!");
  } else {
    debugln("ERROR! EEPROM commit failed!");
  }
}