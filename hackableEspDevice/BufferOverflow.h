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

#define MAX_LEN    10

void ls();
void vi();
void objectDump();
bool runCProgram(String arg);
bool checkBufferOverflow(String input);
#endif
