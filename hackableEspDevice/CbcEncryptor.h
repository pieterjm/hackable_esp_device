/*
 * File:      CbcEncryptor.h
 * Author:    Luke de Munk
 * Class:     CbcEncryptor
 * Version:   1.0
 * 
 * Handles encryption and decryption of files. Uses AES encryption.
 */
#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H
#include <FS.h>                                                             //For SPIFFS
#include "AES.h"                                                            //For the encryption
#include "config.h"                                                         //For the configuration

class CbcEncryptor
{
    public:
        CbcEncryptor();
        bool encryptFile(String filename);
        bool decryptFile(String filename);
        String encryptLine(String line);
        String decryptLine(String line);
        bool setKey(String key);
    
    private:
        byte _hexCharToByte(char hexChar);

        String _aesKeyString;
        byte _aesKey[AES_KEY_SIZE];
        byte _aesInitVector[N_BLOCK];
        unsigned long long int _aesInitVecInt;
        AES _aes;
};
#endif
