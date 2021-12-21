/*
 * File:      BufferOverflow.h
 * Author:    Luke de Munk
 * Version:   0.1
 * 
 * Buffer flow emulator. All elements of the bufferflow are in this class.
 */
#include "BufferOverflow.h"

/**************************************************************************/
/*!
  @brief    Prints the fake list of files.
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
void ls() {
    Serial.println(F("testprogram.c"));
    Serial.println(F("testprogram"));
}

/**************************************************************************/
/*!
  @brief    Prints the vulnerable testprogram.
*/
/**************************************************************************/
void vi() {
    Serial.println(F("|-----------FILENAME-----------|-----------TYPE-----------|-----------AUTOR-----------|"));
    Serial.println(F("|---------testprogram.c--------|---------READONLY---------|-----------admin-----------|"));
    Serial.println(F("|-------------------------------------------------------------------------------------|"));
    Serial.println(F("1     /*                                                                              |"));
    Serial.println(F("2      * File:   testprogram.c                                                        |"));
    Serial.println(F("3      * Author: admin                                                                |"));
    Serial.println(F("4      *                                                                              |"));
    Serial.println(F("5      * To test superuser login. DELETE WHEN FINISHING DEVELOPMENT!!!                |"));
    Serial.println(F("6      */                                                                             |"));
    Serial.println(F("7     #include <stdio.h>                                                              |"));
    Serial.println(F("8     #include <string.h>                                                             |"));
    Serial.println(F("9                                                                                     |"));
    Serial.println(F("10                                                                                    |"));
    Serial.println(F("11    /**************************************************************************/    |"));
    Serial.println(F("12    /*!                                                                             |"));
    Serial.println(F("13      @brief    Logs given user name in as superuser and logs out again.            |"));
    Serial.println(F("14    */                                                                              |"));
    Serial.println(F("15    /**************************************************************************/    |"));
    Serial.println(F("16    int main(int argc, char** argv) {                                               |"));
    Serial.println(F("17        char username[10];                                                          |"));
    Serial.println(F("18        strcpy(username, argv[1]);                                                  |"));
    Serial.println(F("19        login(*username);                                                           |"));
    Serial.println(F("20        logout();                                                                   |"));
    Serial.println(F("21                                                                                    |"));
    Serial.println(F("22        return 0;                                                                   |"));
    Serial.println(F("23    }                                                                               |"));
    Serial.println(F("|-------------------------------------------------------------------------------------|"));
}

/**************************************************************************/
/*!
  @brief    Prints the disassembled code of the vulnerable testprogram.
*/
/**************************************************************************/
void objectDump() {
    Serial.println(F("00010438 <main>:"));
    Serial.println(F("   10438: e92d4800  push  {fp, lr}"));
    Serial.println(F("   1043c: e28db004  add fp, sp, #4"));
    Serial.println(F("   10440: e24dd018  sub sp, sp, #24"));
    Serial.println(F("   10444: e50b0018  str r0, [fp, #-24]  ; 0xffffffe8"));
    Serial.println(F("   10448: e50b101c  str r1, [fp, #-28]  ; 0xffffffe4"));
    Serial.println(F("   1044c: e51b301c  ldr r3, [fp, #-28]  ; 0xffffffe4"));
    Serial.println(F("   10450: e2833004  add r3, r3, #4"));
    Serial.println(F("   10454: e5932000  ldr r2, [r3]"));
    Serial.println(F("   10458: e24b3010  sub r3, fp, #16"));
    Serial.println(F("   1045c: e1a01002  mov r1, r2"));
    Serial.println(F("   10460: e1a00003  mov r0, r3"));
    Serial.println(F("   10464: ebffffab  bl  10318 <strcpy@plt>"));
    Serial.println(F("   10468: e55b3010  ldrb  r3, [fp, #-16]"));
    Serial.println(F("   1046c: e1a00003  mov r0, r3"));
    Serial.println(F("   10470: eb000004  bl  10488 <login>"));
    Serial.println(F("   10474: eb00000d  bl  104b0 <logout>"));
    Serial.println(F("   10478: e3a03000  mov r3, #0"));
    Serial.println(F("   1047c: e1a00003  mov r0, r3"));
    Serial.println(F("   10480: e24bd004  sub sp, fp, #4"));
    Serial.println(F("   10484: e8bd8800  pop {fp, pc}"));

}

/**************************************************************************/
/*!
  @brief    Runs the vulnerable testprogram.
  @return   bool      True if the buffer overflow attack is done correctly
*/
/**************************************************************************/
bool runCProgram(String arg) {
  /* If the length is OK, run program
   * Else, check if buffer overflow attack
   * is correctly done.
   */
    if (arg.length() < OVERFLOW_BEGIN) {
        Serial.println("You are now super user.");
        Serial.print("Hello ");
        Serial.println(arg);
        Serial.println("You are not longer super user.");
    } else {
        if (checkBufferOverflow(arg)) {
          return true;
        }
    }
    return false;
}

bool checkBufferOverflow(String input) {
  if(input.substring(OVERFLOW_BEGIN) == RETURN_ADDRESS) { //???
    return true;
  }
  printOverflowError(input);
  return false;
}

void printOverflowError(String input) {
  String overflowPortion = "";
  uint8_t numberOfHex = 0;
  
  Serial.println("Program received signal SIGSEGV, Segmentation fault.");
  Serial.print("0x");
  
  String formattedInput = formatInput(input, &numberOfHex);
  Serial.println(formattedInput.length());
  
  Serial.println(formattedInput[formattedInput.length()-1], HEX);
  if(formattedInput.length() < OVERFLOW_LENGTH) {
      uint8_t numMissingBytes = OVERFLOW_LENGTH - formattedInput.length();
      String randomBytes = generateRandomBytes(numMissingBytes);
      
      overflowPortion = formattedInput.substring(numMissingBytes, numMissingBytes-ADDRESS_LENGTH);
      Serial.println(randomBytes);
      Serial.println(overflowPortion);
      for (uint8_t i = 0; i < ADDRESS_LENGTH-numMissingBytes; i++) {
          Serial.print(overflowPortion[i], HEX);
      }
  } else {
      //overflowPortion = input.substring(OVERFLOW_BEGIN, OVERFLOW_LENGTH);
      for (uint8_t i = OVERFLOW_BEGIN; i < ADDRESS_LENGTH; i++) {
          Serial.print(formattedInput[i], HEX);
      }
  }
  Serial.println(" in ?? ()");
}

String formatInput(String input, uint8_t *numberOfHex) {
    char tmp;
    uint8_t lastIndex = 0;
    uint8_t counter = 0;
    String kanker;
    while(input.indexOf("\\x", lastIndex) != -1) {
      lastIndex = input.indexOf("\\x", lastIndex);
      input[lastIndex] = ' ';
      input[lastIndex+1] = ' ';
      input[lastIndex+2] = ' ';
      input[lastIndex+3] = char(char2int(input[lastIndex+3])*16 + char2int(input[lastIndex+2]));
      kanker = char(char2int(input[lastIndex+3])*16 + char2int(input[lastIndex+2]));
      Serial.println(kanker[0], HEX);
      lastIndex += 4;
      counter++;
    }
    *numberOfHex = counter;

    String formattedInput = "                                ";//Fill with something, otherwise setCharAt() is not working...
    uint8_t j = 0;
    
    for (uint8_t i = 0; i < input.length(); i++) {
      if (input[i] != ' ') {
        formattedInput.setCharAt(j, input.charAt(i));
        j++;
      }
    }
    
    formattedInput = formattedInput.substring(0, formattedInput.indexOf(" "));
  /*
    for (uint8_t i = 0; i < formattedInput.length()/2-1; i++) {
      tmp = formattedInput[i];
      formattedInput[i] = formattedInput[formattedInput.length()-i-1];
      formattedInput[formattedInput.length()-i-1] = tmp;
    }*/
    return formattedInput;
}
int char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  return -1;
}

String generateRandomBytes(uint8_t numberOfBytes) {
    String bytes = "";
    randomSeed(numberOfBytes);
    
    for (uint8_t i = 0; i < numberOfBytes; i++) {
      bytes += String(random(127), HEX);
    }
    
    return bytes;
}
