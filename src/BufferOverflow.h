/*
 * File:      BufferOverflow.h
 * Author:    Luke de Munk
 * Class:     BufferOverflow
 * Version:   1.0
 * 
 * Buffer overflow simulator based on a Linux cli.
 * All elements of the bufferflow are in this class.
 */
#ifndef BUFFER_OVERFLOW_H
#define BUFFER_OVERFLOW_H
#include <stdint.h>                                                         //For defining bits per integer
#include "Arduino.h"

#define OVERFLOW_BEGIN    16                                                //Because array is in byte resolution, 10 becomes 16. Then the return address pointer starts
#define ADDRESS_LENGTH    4                                                 //Address is 32 bits long, so 4 bytes
#define OVERFLOW_LENGTH   20                                                //OVERFLOW_BEGIN + ADDRESS_LENGTH
#define RETURN_ADDRESS    "00010488"                                        //0x00010488 == address of login function.
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
