/*
 * File:      CbcEncryptor.cpp
 * Author:    Luke de Munk
 * Class:     CbcEncryptor
 * Version:   0.1
 * 
 * Handles encryption and decryption of files. Uses AES encryption.
 */
#include "CbcEncryptor.h"

/**************************************************************************/
/*!
  @brief    Constructor
*/
/**************************************************************************/
CbcEncryptor::CbcEncryptor() {
    _aesInitVecInt = CBC_INIT_VECTOR;
    _aesKeyString = AES_KEY;
    
    for (uint8_t i = 0; i < AES_KEY_SIZE; i++) {
        _aesKey[i] = (byte) _aesKeyString[i];
    }
}

/**************************************************************************/
/*!
  @brief    brief
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
bool CbcEncryptor::encryptFile(String filename) {
    if (!SPIFFS.exists(filename)) {
        return false;
    }
    
    setKey(AES_KEY);                                                        //Reset key
    _aesKeyString = AES_KEY;

    File inputFile = SPIFFS.open(filename, "r");                            //Plaintext file
    File tmpFile = SPIFFS.open("tmp_" + filename, "w");                     //Temporary file to store encrypted part
    
    /* If already is plaintext, don't do anything */
    if (inputFile.readStringUntil('\n') == "format: encrypted") {
        return false;
    }
    
    tmpFile.println("format: encrypted");                                   //Save format
    String line = inputFile.readStringUntil('\n');
    
    _aes.iv_inc();                                                          //Unique initialization vector every encryption
    
    /* Encrypt the file line by line */
    while (line != "") {
        tmpFile.println(encryptLine(line));
        line = inputFile.readStringUntil('\n');                             //Read a line from the file
    }
    
    inputFile.close();
    tmpFile.close();
    
    SPIFFS.remove(filename);                                                //Remove plain text file
    SPIFFS.rename("tmp_" + filename, filename);                             //Rename tmp file
    return true;
}

/**************************************************************************/
/*!
  @brief    brief
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
bool CbcEncryptor::decryptFile(String filename) {
    if (!SPIFFS.exists(filename)) {
        return false;
    }
    
    setKey(AES_KEY);                                                        //Reset key
    _aesKeyString = AES_KEY;
    
    File inputFile = SPIFFS.open(filename, "r");
    File tmpFile = SPIFFS.open("tmp_" + filename, "w");
    
    /* If already is plaintext, don't do anything */
    if (inputFile.readStringUntil('\n') == "format: plaintext") {
        return false;
    }

    tmpFile.println("format: plaintext");                                   //Save format
    String line = inputFile.readStringUntil('\n');
    
    /* Encrypt the file line by line */
    while (line != "") {
        tmpFile.println(decryptLine(line));
        line = inputFile.readStringUntil('\n');                             //Read a line from the file
    }
    
    inputFile.close();
    tmpFile.close();
    
    SPIFFS.remove(filename);                                                //Remove plain text file
    SPIFFS.rename("tmp_" + filename, filename);                             //Rename tmp file
    return true;
}

/**************************************************************************/
/*!
  @brief    brief
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
bool CbcEncryptor::setKey(String key) {
    _aesKeyString = key;
    
    if (key.length() != AES_KEY_SIZE) {
        return false;
    }
    
    for (uint8_t i = 0; i < AES_KEY_SIZE; i++) {
        _aesKey[i] = (byte) key[i];
    }
    return true;
}

/**************************************************************************/
/*!
  @brief    brief
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
String CbcEncryptor::encryptLine(String line) {
    uint16_t len = line.length();
    byte plain[len];
    uint8_t paddedLength = len + N_BLOCK - len % N_BLOCK;
    byte encrypted[paddedLength];
    char encryptedString[paddedLength*2+1];                                   //To store return value
  
    /* Convert string to bytes */
    for (uint8_t i = 0; i < len; i++) {
        plain[i] = (byte) line[i];
    }
  
    _aes.set_IV(_aesInitVecInt);
    _aes.get_IV(_aesInitVector);
    
    _aes.do_aes_encrypt(plain, len, encrypted, _aesKey, 128, _aesInitVector);
  
    /* Convert bytes to string */
    for (uint8_t i = 0; i < paddedLength; i++) {
        sprintf(encryptedString+2*i,"%02x",*(encrypted+i));
    }

    return encryptedString;
}

/**************************************************************************/
/*!
  @brief    brief
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
String CbcEncryptor::decryptLine(String line) {
    uint16_t len = line.length()/2;
    byte encrypted[len];
    byte decrypted[len];
    uint8_t outputLen = 0;
    char decryptedString[len];                                              //To store return value

    /* Convert hex string to bytes */
    for (uint8_t i = 0; i < len; i++) {
        encrypted[i] = _hexCharToByte(&line[i*2])<<4 | _hexCharToByte(&line[i*2+1]);
    }
    
    _aes.set_IV(_aesInitVecInt);
    _aes.get_IV(_aesInitVector);
    _aes.do_aes_decrypt(encrypted, len, decrypted, _aesKey, 128, _aesInitVector);

    for (uint8_t i = 0; i < len; i++) {
        /* If decrypted is printable character, save */
        if (decrypted[i] < 127 && decrypted[i] > 32) {
            decryptedString[i] = (char) decrypted[i];
        } else {
            outputLen = i;
            break;
        }
    }
    
    if (_aesKeyString != AES_KEY) {
        return String(decryptedString);                                     //If another key is used, just throw the whole string back
    }
    return String(decryptedString).substring(0, outputLen);                 //Convert to string to trim the string with substring()
}

/**************************************************************************/
/*!
  @brief    brief
  @param    var           desc
  @return   var           desc
*/
/**************************************************************************/
byte CbcEncryptor::_hexCharToByte(char *in) {
    uint8_t c = in[0];

    if (c <= '9' && c >= '0') {
        c -= '0';
    } else if (c <= 'f' && c >= 'a') {
        c -= ('a' - 0x0a);
    } else if (c <= 'F' && c >= 'A') {
        c -= ('A' - 0x0a);
    } else {
        return(1);
    }
    
   return (c);
}
