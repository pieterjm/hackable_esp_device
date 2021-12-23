/*
 * File:      BufferOverflow.h
 * Author:    Luke de Munk
 * Class:     BufferOverflow
 * Version:   0.1
 * 
 * Buffer flow simulator. All elements of the bufferflow are in this class.
 */
#ifndef BUFFER_OVERFLOW_H
#define BUFFER_OVERFLOW_H
#include <stdint.h>
#include "Arduino.h"

#define OVERFLOW_BEGIN    16
#define OVERFLOW_LENGTH   20
#define ADDRESS_LENGTH    4
#define RETURN_ADDRESS    "00010488"                //0x00010488 == address of login function.
#define MAX_NUM_CHARS     256

class BufferOverflow
{
    public:
        BufferOverflow();
        void ls();
        void vi();
        void objectDump();
        bool runCProgram(String arg);
    
    private:
        bool _checkBufferOverflow();
        void _printOverflowError();
        void _formatInput(String input);
        String _getOverflowPortion(bool print = false);
        void _clearInput();
        String _generateRandomBytes(uint8_t numberOfBytes);
                 
        String _formattedInput[256];
        uint8_t _numChars;
};
#endif
