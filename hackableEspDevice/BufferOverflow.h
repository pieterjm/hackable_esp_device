/*
 * File:      BufferOverflow.h
 * Author:    Luke de Munk
 * Class:     BufferOverflow
 * Version:   0.1
 * 
 * Buffer flow emulator. All elements of the bufferflow are in this class.
 */
#ifndef BUFFER_OVERFLOW_H
#define BUFFER_OVERFLOW_H
#include <stdint.h>
#include "Arduino.h"

#define OVERFLOW_BEGIN    16
#define OVERFLOW_LENGTH   20
#define ADDRESS_LENGTH    4
#define RETURN_ADDRESS    "\\x70\\x04\\x01\\x00"//"0x00010470"//address of login. needs to be changed?????

void ls();
void vi();
void objectDump();
bool runCProgram(String arg);
bool checkBufferOverflow(String input);
void printOverflowError(String input);
String formatInput(String input, uint8_t *numberOfHex);
String generateRandomBytes(uint8_t numberOfBytes);
int char2int(char input);
#endif
