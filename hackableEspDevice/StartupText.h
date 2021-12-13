/*
 * File:      startupText.h
 * Author:    Luke de Munk
 * Version:   0.1
 * 
 * Static text in bytes that is printed when debug is on, to show
 * vulnerable information packed in it.
 */
#ifndef STARTUP_TEXT_H
#define STARTUP_TEXT_H

#define LENGTH    20                                                        //Number of bytes per line

/* Declare functions, because it is not a class */
bool printStartupText(String hiddenMess);
bool printStringInBytes(String str);

/**************************************************************************/
/*!
  @brief    Prints bytes of information with a message wrapped in it.
  @param    hiddenMess      String of text that needs to be printed
  @return   bool            If conversion is successfull
*/
/**************************************************************************/
bool printStartupText(String hiddenMess) {
    /* Serial.println(F(x));, because then the strings are stored in FLASH */
    Serial.println(F("Bootlog file print: "));
    Serial.println(F("53 74 61 72 74 75 70 20 62 75 73 79 2E 2E 2E 0A 65 74 73"));
    Serial.println(F("20 4A 61 6E 20 20 38 20 32 30 31 33 2C 72 73 74 20 63 61"));
    Serial.println(F("75 73 65 3A 32 2C 20 62 6F 6F 74 20 6D 6F 64 65 3A 28 33"));
    Serial.println(F("2C 36 29 0A 6C 6F 61 64 20 30 78 34 30 31 30 66 30 30 30"));
    Serial.println(F("2C 20 6C 65 6E 20 33 35 38 34 2C 20 72 6F 6F 6D 20 31 36"));
    Serial.println(F("20 0A 74 61 69 6C 20 30 0A 63 68 6B 73 75 6D 20 30 78 62"));
    Serial.println(F("30 0A 63 73 75 6D 20 30 78 62 30 0A 76 32 38 34 33 61 35"));
    Serial.println(F("61 63 0A 7E 6C 64 0A 45 78 65 63 75 74 61 62 6C 65 20 73"));
    Serial.println(F("65 67 6D 65 6E 74 20 73 69 7A 65 73 3A 0A 49 52 4F 4D 20"));
    Serial.println(F("20 20 3A 20 33 30 38 31 35 36 20 20 20 20 20 20 20 20 20"));
    Serial.println(F("20 2D 20 63 6F 64 65 20 69 6E 20 66 6C 61 73 68 20 20 20"));
    Serial.println(F("20 20 20 20 20 20 28 64 65 66 61 75 6C 74 20 6F 72 20 49"));
    Serial.println(F("43 41 43 48 45 5F 46 4C 41 53 48 5F 41 54 54 52 29 20 0A"));
    Serial.println(F("49 52 41 4D 20 20 20 3A 20 32 37 32 39 32 20 20 20 2F 20"));
    Serial.println(F("33 32 37 36 38 20 2D 20 63 6F 64 65 20 69 6E 20 49 52 41"));
    Serial.println(F("4D 20 20 20 20 20 20 20 20 20 20 28 49 43 41 43 48 45 5F"));
    Serial.println(F("52 41 4D 5F 41 54 54 52 2C 20 49 53 52 73 2E 2E 2E 29 20"));
    Serial.println(F("0A 44 41 54 41 20 20 20 3A 20 31 32 35 32 20 20 29 20 20"));
    Serial.println(F("20 20 20 20 20 20 20 2D 20 69 6E 69 74 69 61 6C 69 7A 65"));
    Serial.println(F("64 20 76 61 72 69 61 62 6C 65 73 20 28 67 6C 6F 62 61 6C"));
    Serial.println(F("2C 20 73 74 61 74 69 63 29 20 69 6E 20 52 41 4D 2F 48 45"));
    Serial.println(F("41 50 20 0A 52 4F 44 41 54 41 20 3A 20 33 30 35 36 20 20"));
    Serial.println(F("29 20 2F 20 38 31 39 32 30 20 2D 20 63 6F 6E 73 74 61 6E"));
    Serial.println(F("74 73 20 20 20 20 20 20 20 20 20 20 20 20 20 28 67 6C 6F"));
    Serial.println(F("62 61 6C 2C 20 73 74 61 74 69 63 29 20 69 6E 20 52 41 4D"));
    Serial.println(F("2F 48 45 41 50 20 0A 42 53 53 20 20 20 20 3A 20 32 36 33"));
    Serial.println(F("36 38 20 29 20 20 20 20 20 20 20 20 20 2D 20 7A 65 72 6F"));
    Serial.println(F("65 64 20 76 61 72 69 61 62 6C 65 73 20 20 20 20 20 20 28"));
    Serial.println(F("67 6C 6F 62 61 6C 2C 20 73 74 61 74 69 63 29 20 69 6E 20"));
    Serial.println(F("52 41 4D 2F 48 45 41 50 20 0A 42 6F 61 72 64 20 20 3A 20"));
    Serial.println(F("22 57 65 4D 6F 73 20 44 31 20 4D 69 6E 69 22 0A 44 65 62"));
    Serial.println(F("75 67 20 20 3A 20 54 72 75 65 0A 43 50 55 20 66 72 65 71"));
    Serial.println(F("75 65 6E 63 79 20 3A 20 38 30 4D 48 7A 0A 56 75 6C 6E 65"));
    Serial.println(F("72 61 62 69 6C 69 74 79 20 41 73 73 65 73 73 6D 65 6E 74"));
    Serial.println(F("20 53 63 61 6E 20 53 74 61 74 75 73 0A 53 69 6E 67 6C 65"));
    Serial.println(F("20 6D 61 74 63 68 69 6E 67 20 61 63 63 6F 75 6E 74 20 66"));
    Serial.println(F("6F 75 6E 64 20 69 6E 20 64 6F 6D 61 69 6E 0A 55 73 65 72"));
    Serial.println(F("20 61 75 74 68 65 6E 74 69 63 61 74 69 6F 6E 20 61 67 61"));
    Serial.println(F("69 6E 73 74 20 41 63 74 69 76 65 20 44 69 72 65 63 74 6F"));
    Serial.println(F("72 79 20 66 61 69 6C 65 64 20 73 69 6E 63 65 20 75 73 65"));
    Serial.println(F("72 20 69 73 20 63 6F 6E 73 69 64 65 72 65 64 20 74 6F 20"));
    Serial.println(F("62 65 20 69 6E 20 72 65 73 74 72 69 63 74 65 64 20 6C 6F"));
    Serial.println(F("67 6F 6E 20 68 6F 75 72 73 0A 54 72 75 73 74 73 65 63 20"));
    Serial.println(F("65 67 72 65 73 73 20 70 6F 6C 69 63 79 20 77 61 73 20 73"));
    Serial.println(F("75 63 63 65 73 73 66 75 6C 6C 79 20 64 6F 77 6E 6C 6F 61"));
    Serial.println(F("64 65 64 0A 52 41 44 49 55 53 20 44 54 4C 53 3A 20 72 65"));
    Serial.println(F("63 65 69 76 65 64 20 63 6C 69 65 6E 74 20 68 65 6C 6C 6F"));
    Serial.println(F("20 76 65 72 69 66 79 20 72 65 71 75 65 73 74 0A 54 68 65"));
    Serial.println(F("20 75 73 65 72 27 73 20 6F 72 20 68 6F 73 74 27 73 20 61"));
    Serial.println(F("63 63 6F 75 6E 74 20 69 73 20 69 6E 20 72 65 73 74 72 69"));
    Serial.println(F("63 74 65 64 20 6C 6F 67 6F 6E 20 68 6F 75 72 73 3B 20 73"));
    Serial.println(F("65 74 74 69 6E 67 20 74 68 65 20 49 64 65 6E 74 69 74 79"));
    Serial.println(F("41 63 63 65 73 73 52 65 73 74 72 69 63 74 65 64 20 66 6C"));
    Serial.println(F("61 67 20 74 6F 20 74 72 75 65 2E 20 74 72 75 65 0A 53 65"));
    Serial.println(F("6E 74 20 54 45 41 50 20 52 65 73 75 6C 74 20 54 4C 56 20"));
    Serial.println(F("69 6E 64 69 63 61 74 69 6E 67 20 73 75 63 63 65 73 73 0A"));
    Serial.println(F("47 75 65 73 74 20 73 65 73 73 69 6F 6E 20 6C 69 6D 69 74"));
    Serial.println(F("20 69 73 20 61 63 74 69 76 65 3B 20 72 65 6D 6F 76 69 6E"));
    Serial.println(F("67 20 6F 6C 64 65 72 20 67 75 65 73 74 20 73 65 73 73 69"));
    Serial.println(F("6F 6E 73 0A 53 65 76 65 72 61 6C 20 63 65 72 74 69 66 69"));
    Serial.println(F("63 61 74 65 73 20 61 72 65 20 63 6F 6E 66 69 67 75 72 65"));
    Serial.println(F("64 20 6F 6E 20 49 64 50 2C 68 6F 77 65 76 65 72 20 63 61"));
    Serial.println(F("6E 20 6E 6F 74 20 64 65 74 65 72 6D 69 6E 65 20 63 65 72"));
    Serial.println(F("74 69 66 69 63 61 74 65 20 66 6F 72 20 73 69 67 6E 61 74"));
    Serial.println(F("75 72 65 0A 53 75 73 70 65 6E 64 20 6C 6F 67 20 63 6F 6C"));
    Serial.println(F("6C 65 63 74 6F 72 0A 46 61 69 6C 65 64 20 74 6F 20 6A 6F"));
    Serial.println(F("69 6E 20 74 6F 20 41 44 0A 49 67 6E 6F 72 65 20 4D 61 63"));
    Serial.println(F("68 69 6E 65 20 41 75 74 68 6F 72 69 7A 61 74 69 6F 6E 20"));
    Serial.println(F("50 41 43 20 72 65 71 75 65 73 74 20 62 65 63 61 75 73 65"));
    Serial.println(F("20 6F 66 20 63 75 72 72 65 6E 74 20 50 41 43 20 6F 66 20"));
    Serial.println(F("74 68 65 20 73 61 6D 65 20 74 79 70 65 20 77 61 73 20 75"));
    Serial.println(F("73 65 64 20 74 6F 20 73 6B 69 70 20 69 6E 6E 65 72 20 6D"));
    Serial.println(F("65 74 68 6F 64 0A 4E 54 50 20 53 65 72 76 65 72 20 73 65"));
    Serial.println(F("74 0A 43 68 69 70 20 69 73 20 45 53 50 38 32 36 36 45 58"));
    Serial.println(F("0A 46 65 61 74 75 72 65 73 3A 20 57 69 46 69 0A 43 72 79"));
    Serial.println(F("73 74 61 6C 20 69 73 20 32 36 4D 48 7A 0A 4D 41 43 3A 20"));
    Serial.println(F("38 63 3A 61 61 3A 62 35 3A 37 62 3A 65 30 3A 61 38 0A 43"));
    Serial.println(F("6F 6D 70 72 65 73 73 65 64 20 33 34 34 36 30 38 20 62 79"));
    Serial.println(F("74 65 73 20 74 6F 20 32 34 38 38 32 36 2E 2E 2E 0A 48 61"));
    Serial.println(F("73 68 20 6F 66 20 64 61 74 61 20 76 65 72 69 66 69 65 64"));
    Serial.println(F("2E 0A 43 6C 69 65 6E 74 20 63 65 72 74 69 66 69 63 61 74"));
    Serial.println(F("65 20 77 61 73 20 72 65 71 75 65 73 74 65 64 20 62 75 74"));
    Serial.println(F("20 6E 6F 74 20 72 65 63 65 69 76 65 64 20 69 6E 73 69 64"));
    Serial.println(F("65 20 74 68 65 20 74 75 6E 6E 65 6C 2E 20 57 69 6C 6C 20"));
    Serial.println(F("63 6F 6E 74 69 6E 75 65 20 77 69 74 68 20 69 6E 6E 65 72"));
    Serial.println(F("20 6D 65 74 68 6F 64 2E 0A 54 65 6C 65 6D 65 74 72 79 20"));
    Serial.println(F("6D 65 73 73 61 67 65 73 20 77 65 72 65 20 73 65 6E 74 20"));
    Serial.println(F("73 75 63 63 65 73 73 66 75 6C 6C 79 0A 44 65 6C 65 74 65"));
    Serial.println(F("20 6E 6F 64 65 20 66 61 69 6C 65 64 0A 50 72 6F 66 69 6C"));
    Serial.println(F("65 72 20 45 6E 64 50 6F 69 6E 74 20 63 6F 6C 6C 65 63 74"));
    Serial.println(F("69 6F 6E 20 65 76 65 6E 74 20 6F 63 63 75 72 72 65 64 0A"));
    Serial.println(F("52 41 44 49 55 53 20 44 54 4C 53 20 43 6F 41 20 68 61 6E"));
    Serial.println(F("64 73 68 61 6B 65 20 73 74 61 72 74 65 64 0A 52 75 6E 6E"));
    Serial.println(F("69 6E 67 20 73 74 75 62 2E 2E 2E 0A 53 74 75 62 20 72 75"));
    Serial.println(F("6E 6E 69 6E 67 2E 2E 2E 0A 53 74 6F 70 70 65 64 20 54 41"));
    Serial.println(F("43 41 43 53 2B 20 6C 69 73 74 65 6E 65 72 0A 53 65 6C 65"));
    Serial.println(F("63 74 65 64 20 41 63 63 65 73 73 20 53 65 72 76 69 63 65"));
    Serial.println(F("20 74 79 70 65 20 69 73 20 6E 6F 74 20 44 65 76 69 63 65"));
    Serial.println(F("20 41 64 6D 69 6E 69 73 74 72 61 74 69 6F 6E 0A 4C 6F 63"));
    Serial.println(F("61 6C 20 6D 6F 64 65 0A 55 73 65 72 20 61 75 74 68 65 6E"));
    Serial.println(F("74 69 63 61 74 69 6F 6E 20 61 67 61 69 6E 73 74 20 41 63"));
    Serial.println(F("74 69 76 65 20 44 69 72 65 63 74 6F 72 79 20 66 61 69 6C"));
    Serial.println(F("65 64 20 73 69 6E 63 65 20 75 73 65 72 20 68 61 73 20 69"));
    Serial.println(F("6E 76 61 6C 69 64 20 63 72 65 64 65 6E 74 69 61 6C 73 0A"));
    Serial.println(F("43 41 20 73 65 72 76 69 63 65 20 64 69 73 61 62 6C 65 64"));
    Serial.println(F("0A 43 68 61 6E 67 69 6E 67 20 62 61 75 64 20 72 61 74 65"));
    Serial.println(F("20 74 6F 20 34 36 30 38 30 30 0A 43 6F 6E 66 69 67 75 72"));
    Serial.println(F("69 6E 67 20 66 6C 61 73 68 20 73 69 7A 65 2E 2E 2E 0A 41"));
    Serial.println(F("75 74 6F 2D 64 65 74 65 63 74 65 64 20 46 6C 61 73 68 20"));
    Serial.println(F("73 69 7A 65 3A 20 34 4D 42 0A 49 6E 76 61 6C 69 64 20 6E"));
    Serial.println(F("65 77 20 70 61 73 73 77 6F 72 64 2E 20 43 6F 6E 74 61 69"));
    Serial.println(F("6E 73 20 72 65 73 65 72 76 65 64 20 77 6F 72 64 0A 52 53"));
    Serial.println(F("41 20 61 67 65 6E 74 20 63 6F 6E 66 69 67 75 72 61 74 69"));
    Serial.println(F("6F 6E 20 75 70 64 61 74 65 64 2C 20 52 53 41 20 61 67 65"));
    Serial.println(F("6E 74 20 72 65 73 74 61 72 74 65 64 0A 4C 6F 6F 6B 75 70"));
    Serial.println(F("20 53 49 44 20 42 79 20 4E 61 6D 65 20 72 65 71 75 65 73"));
    Serial.println(F("74 20 66 61 69 6C 65 64 0A 53 74 61 72 74 20 6C 69 73 74"));
    Serial.println(F("65 6E 69 6E 67 20 74 6F 20 74 63 70 20 70 6F 72 74 0A 49"));
    Serial.println(F("67 6E 6F 72 65 20 4D 61 63 68 69 6E 65 20 41 75 74 68 6F"));
    Serial.println(F("72 69 7A 61 74 69 6F 6E 20 50 41 43 20 72 65 71 75 65 73"));
    Serial.println(F("74 20 62 65 63 61 75 73 65 20 6F 66 20 63 75 72 72 65 6E"));
    Serial.println(F("74 20 50 41 43 20 6F 66 20 74 68 65 20 73 61 6D 65 20 74"));
    Serial.println(F("79 70 65 20 77 61 73 20 75 73 65 64 20 74 6F 20 73 6B 69"));
    Serial.println(F("70 20 69 6E 6E 65 72 20 6D 65 74 68 6F 64 0A 49 53 45 20"));
    Serial.println(F("75 70 67 72 61 64 65 20 2D 20 4D 6E 54 0A 49 53 45 20 42"));
    Serial.println(F("61 63 6B 75 70 20 68 61 73 20 73 74 61 72 74 65 64 0A 54"));
    Serial.println(F("72 75 73 74 73 65 63 20 65 67 72 65 73 73 20 70 6F 6C 69"));
    Serial.println(F("63 79 20 77 61 73 20 73 75 63 63 65 73 73 66 75 6C 6C 79"));
    Serial.println(F("20 64 6F 77 6E 6C 6F 61 64 65 64 0A 52 41 44 49 55 53 20"));
    Serial.println(F("44 54 4C 53 3A 20 72 65 63 65 69 76 65 64 20 63 6C 69 65"));
    Serial.println(F("6E 74 20 68 65 6C 6C 6F 20 76 65 72 69 66 79 20 72 65 0A"));
    /* Print the message, return false if is not successfull */
    if (!printStringInBytes(hiddenMess)) {
      return false;
    }
    Serial.println(F("75 65 73 74 0A 47 75 65 73 74 20 73 65 73 73 69 6F 6E 20"));
    Serial.println(F("6C 69 6D 69 74 20 69 73 20 61 63 74 69 76 65 3B 20 72 65"));
    Serial.println(F("6D 6F 76 69 6E 67 20 6F 6C 64 65 72 20 67 75 65 73 74 20"));
    Serial.println(F("73 65 73 73 69 6F 6E 73 0A 49 67 6E 6F 72 65 20 4D 61 63"));
    Serial.println(F("68 69 6E 65 20 41 75 74 68 6F 72 69 7A 61 74 69 6F 6E 20"));
    Serial.println(F("50 41 43 20 72 65 71 75 65 73 74 20 62 65 63 61 75 73 65"));
    Serial.println(F("20 6F 66 20 63 75 72 72 65 6E 74 20 50 41 43 20 6F 66 20"));
    Serial.println(F("74 68 65 20 73 61 6D 65 20 74 79 70 65 20 77 61 73 20 75"));
    Serial.println(F("73 65 64 20 74 6F 20 73 6B 69 70 20 69 6E 6E 65 72 20 6D"));
    Serial.println(F("65 74 68 6F 64 0A 43 6C 69 65 6E 74 20 63 65 72 74 69 66"));
    Serial.println(F("69 63 61 74 65 20 77 61 73 20 72 65 71 75 65 73 74 65 64"));
    Serial.println(F("20 62 75 74 20 6E 6F 74 20 72 65 63 65 69 76 65 64 20 69"));
    Serial.println(F("6E 73 69 64 65 20 74 68 65 20 74 75 6E 6E 65 6C 2E 20 57"));
    Serial.println(F("69 6C 6C 20 63 6F 6E 74 69 6E 75 65 20 77 69 74 68 20 69"));
    Serial.println(F("6E 6E 65 72 20 6D 65 74 68 6F 64 2E 0A 54 65 6C 65 6D 65"));
    Serial.println(F("74 72 79 20 6D 65 73 73 61 67 65 73 20 77 65 72 65 20 73"));
    Serial.println(F("65 6E 74 20 73 75 63 63 65 73 73 66 75 6C 6C 79 0A 50 72"));
    Serial.println(F("6F 66 69 6C 65 72 20 45 6E 64 50 6F 69 6E 74 20 63 6F 6C"));
    Serial.println(F("6C 65 63 74 69 6F 6E 20 65 76 65 6E 74 20 6F 63 63 75 72"));
    Serial.println(F("72 65 64 0A 52 41 44 49 55 53 20 44 54 4C 53 20 43 6F 41"));
    Serial.println(F("20 68 61 6E 64 73 68 61 6B 65 20 73 74 61 72 74 65 64 0A"));
    Serial.println(F("53 74 6F 70 70 65 64 20 54 41 43 41 43 53 2B 20 6C 69 73"));
    Serial.println(F("74 65 6E 65 72 0A 53 65 6C 65 63 74 65 64 20 41 63 63 65"));
    Serial.println(F("73 73 20 53 65 72 76 69 63 65 20 74 79 70 65 20 69 73 20"));
    Serial.println(F("6E 6F 74 20 44 65 76 69 63 65 20 41 64 6D 69 6E 69 73 74"));
    Serial.println(F("72 61 74 69 6F 6E 0A 43 41 20 73 65 72 76 69 63 65 20 64"));
    Serial.println(F("69 73 61 62 6C 65 64 0A 52 53 41 20 61 67 65 6E 74 20 63"));
    Serial.println(F("6F 6E 66 69 67 75 72 61 74 69 6F 6E 20 75 70 64 61 74 65"));
    Serial.println(F("64 2C 20 52 53 41 20 61 67 65 6E 74 20 72 65 73 74 61 72"));
    Serial.println(F("74 65 64 0A 53 74 61 72 74 20 6C 69 73 74 65 6E 69 6E 67"));
    Serial.println(F("20 74 6F 20 74 63 70 20 70 6F 72 74 0A 49 67 6E 6F 72 65"));
    Serial.println(F("20 4D 61 63 68 69 6E 65 20 41 75 74 68 6F 72 69 7A 61 74"));
    Serial.println(F("69 6F 6E 20 50 41 43 20 72 65 71 75 65 73 74 20 62 65 63"));
    Serial.println(F("61 75 73 65 20 6F 66 20 63 75 72 72 65 6E 74 20 50 41 43"));
    Serial.println(F("20 6F 66 20 74 68 65 20 73 61 6D 65 20 74 79 70 65 20 77"));
    Serial.println(F("61 73 20 75 73 65 64 20 74 6F 20 73 6B 69 70 20 69 6E 6E"));
    Serial.println(F("65 72 20 6D 65 74 68 6F 64 0A 49 53 45 20 42 61 63 6B 75"));
    Serial.println(F("70 20 68 61 73 20 73 74 61 72 74 65 64 0A 53 6D 61 72 74"));
    Serial.println(F("20 4C 69 63 65 6E 73 69 6E 67 20 61 75 74 68 6F 72 69 7A"));
    Serial.println(F("61 74 69 6F 6E 20 72 65 6E 65 77 61 6C 20 73 75 63 63 65"));
    Serial.println(F("73 73 0A 52 65 6D 69 6E 64 65 72 3A 20 41 73 73 69 67 6E"));
    Serial.println(F("20 4E 41 44 20 50 72 6F 66 69 6C 65 73 2E 0A 52 41 44 49"));
    Serial.println(F("55 53 20 44 54 4C 53 3A 20 73 65 6E 74 20 66 69 6E 69 73"));
    Serial.println(F("68 65 64 20 6D 65 73 73 61 67 65 0A 50 72 65 70 61 72 65"));
    Serial.println(F("64 20 54 4C 53 20 53 65 72 76 65 72 4B 65 79 45 78 63 68"));
    Serial.println(F("61 6E 67 65 20 6D 65 73 73 61 67 65 0A 54 68 65 20 73 65"));
    Serial.println(F("63 75 72 69 64 20 66 69 6C 65 20 68 61 73 20 62 65 65 6E"));
    Serial.println(F("20 72 65 6D 6F 76 65 64 0A 55 70 64 61 74 65 64 20 45 41"));
    Serial.println(F("50 2D 54 4C 53 20 4D 61 73 74 65 72 20 4B 65 79 20 47 65"));
    Serial.println(F("6E 65 72 61 74 69 6F 6E 20 70 65 72 69 6F 64 0A 50 65 72"));
    Serial.println(F("66 6F 72 6D 65 64 20 66 61 6C 6C 62 61 63 6B 20 74 6F 20"));
    Serial.println(F("73 65 63 6F 6E 64 61 72 79 20 4F 43 53 50 20 73 65 72 76"));
    Serial.println(F("65 72 0A 49 53 45 20 68 61 73 20 72 65 66 72 65 73 68 65"));
    Serial.println(F("64 20 61 75 74 68 65 6E 74 69 63 61 74 69 6F 6E 20 61 67"));
    Serial.println(F("61 69 6E 73 74 20 41 50 49 43 20 73 75 63 63 65 73 73 66"));
    Serial.println(F("75 6C 6C 79 0A 52 41 44 49 55 53 20 44 54 4C 53 3A 20 53"));
    Serial.println(F("65 6E 74 20 61 6E 20 4F 43 53 50 20 72 65 71 75 65 73 74"));
    Serial.println(F("20 74 6F 20 74 68 65 20 70 72 69 6D 61 72 79 20 4F 43 53"));
    Serial.println(F("50 20 73 65 72 76 65 72 20 66 6F 72 20 74 68 65 20 43 41"));
    Serial.println(F("0A 55 73 65 72 20 6F 72 20 68 6F 73 74 20 64 69 73 61 62"));
    Serial.println(F("6C 65 64 20 69 6E 20 63 75 72 72 65 6E 74 20 49 44 53 74"));
    Serial.println(F("6F 72 65 20 69 6E 20 61 74 74 72 69 62 75 74 65 20 72 65"));
    Serial.println(F("74 72 69 65 76 61 6C 20 6D 6F 64 65 0A 53 6B 69 70 70 69"));
    Serial.println(F("6E 67 20 75 6E 75 73 61 62 6C 65 20 64 6F 6D 61 69 6E 0A"));
    Serial.println(F("50 72 65 70 61 72 65 64 20 45 41 50 2D 52 65 71 75 65 73"));
    Serial.println(F("74 20 77 69 74 68 20 61 6E 6F 74 68 65 72 20 45 41 50 2D"));
    Serial.println(F("4D 53 43 48 41 50 20 63 68 61 6C 6C 65 6E 67 65 0A 49 64"));
    Serial.println(F("65 6E 74 69 74 79 20 70 6F 6C 69 63 79 20 72 65 73 75 6C"));
    Serial.println(F("74 20 69 73 20 63 6F 6E 66 69 67 75 72 65 64 20 66 6F 72"));
    Serial.println(F("20 70 61 73 73 77 6F 72 64 20 62 61 73 65 64 20 61 75 74"));
    Serial.println(F("68 65 6E 74 69 63 61 74 69 6F 6E 20 6D 65 74 68 6F 64 73"));
    Serial.println(F("20 62 75 74 20 72 65 63 65 69 76 65 64 20 63 65 72 74 69"));
    Serial.println(F("66 69 63 61 74 65 20 62 61 73 65 64 20 61 75 74 68 65 6E"));
    Serial.println(F("74 69 63 61 74 69 6F 6E 20 72 65 71 75 65 73 74 0A 46 61"));
    Serial.println(F("69 6C 65 64 20 74 6F 20 66 6F 72 77 61 72 64 20 72 65 71"));
    Serial.println(F("75 65 73 74 20 74 6F 20 63 75 72 72 65 6E 74 20 72 65 6D"));
    Serial.println(F("6F 74 65 20 52 41 44 49 55 53 20 73 65 72 76 65 72 3B 20"));
    Serial.println(F("61 6E 20 69 6E 76 61 6C 69 64 20 72 65 73 70 6F 6E 73 65"));
    Serial.println(F("20 77 61 73 20 72 65 63 65 69 76 65 64 0A 55 73 65 72 20"));
    Serial.println(F("6C 6F 67 69 6E 20 74 6F 20 49 53 45 20 63 6F 6E 66 69 67"));
    Serial.println(F("75 72 61 74 69 6F 6E 20 6D 6F 64 65 20 66 61 69 6C 65 64"));
    Serial.println(F("0A 55 6E 61 62 6C 65 20 74 6F 20 66 69 6E 64 20 27 75 73"));
    Serial.println(F("65 72 6E 61 6D 65 27 20 61 74 74 72 69 62 75 74 65 20 61"));
    Serial.println(F("73 73 65 72 74 69 6F 6E 0A 56 61 6C 69 64 20 69 6E 63 6F"));
    Serial.println(F("6D 69 6E 67 20 61 63 63 6F 75 6E 74 69 6E 67 20 72 65 71"));
    Serial.println(F("75 65 73 74 0A 41 75 74 68 65 6E 74 69 63 61 74 69 6F 6E"));
    Serial.println(F("20 66 61 69 6C 65 64 20 62 65 63 61 75 73 65 20 4E 54 4C"));
    Serial.println(F("4D 20 77 61 73 20 62 6C 6F 63 6B 65 64 0A 53 6B 69 70 70"));
    Serial.println(F("69 6E 67 20 75 6E 6A 6F 69 6E 65 64 20 64 6F 6D 61 69 6E"));
    Serial.println(F("0A 54 68 65 20 75 73 65 72 20 69 73 20 6E 6F 74 20 66 6F"));
    Serial.println(F("75 6E 64 20 69 6E 20 74 68 65 20 69 6E 74 65 72 6E 61 6C"));
    Serial.println(F("20 67 75 65 73 74 73 20 69 64 65 6E 74 69 74 79 20 73 74"));
    Serial.println(F("6F 72 65 0A 43 68 61 6E 67 65 20 70 61 73 73 77 6F 72 64"));
    Serial.println(F("20 61 67 61 69 6E 73 74 20 41 63 74 69 76 65 20 44 69 72"));
    Serial.println(F("65 63 74 6F 72 79 20 66 61 69 6C 65 64 20 73 69 6E 63 65"));
    Serial.println(F("20 75 73 65 72 20 68 61 73 20 61 20 6E 6F 6E 2D 63 6F 6D"));
    Serial.println(F("70 6C 69 61 6E 74 20 70 61 73 73 77 6F 72 64 0A 41 70 70"));
    Serial.println(F("61 72 65 6E 74 20 6D 69 73 63 6F 6E 66 69 67 75 72 61 74"));
    Serial.println(F("69 6F 6E 20 6F 66 20 45 78 74 65 72 6E 61 6C 20 50 6F 6C"));
    Serial.println(F("69 63 79 20 53 65 72 76 65 72 0A 41 75 74 68 6F 72 69 7A"));
    Serial.println(F("61 74 69 6F 6E 20 70 72 6F 66 69 6C 65 2F 73 20 73 70 65"));
    Serial.println(F("63 69 66 69 65 64 20 61 72 65 20 6E 6F 74 20 73 75 69 74"));
    Serial.println(F("65 64 20 66 6F 72 20 74 68 69 73 20 4E 65 74 77 6F 72 6B"));
    Serial.println(F("20 41 63 63 65 73 73 20 44 65 76 69 63 65 0A 52 65 63 65"));
    Serial.println(F("69 76 65 64 20 61 20 72 65 61 75 74 68 65 6E 74 69 63 61"));
    Serial.println(F("74 65 20 72 65 73 70 6F 6E 73 65 0A 4C 6F 67 67 69 6E 67"));
    Serial.println(F("20 63 6F 6D 70 6F 6E 65 6E 74 20 6E 6F 77 20 72 65 61 64"));
    Serial.println(F("79 20 74 6F 20 72 65 63 65 69 76 65 20 63 6F 6E 66 69 67"));
    Serial.println(F("75 72 61 74 69 6F 6E 20 63 68 61 6E 67 65 73 0A 52 65 74"));
    Serial.println(F("75 72 6E 65 64 20 54 41 43 41 43 53 2B 20 41 75 74 68 65"));
    Serial.println(F("6E 74 69 63 61 74 69 6F 6E 20 52 65 70 6C 79 0A 45 76 61"));
    Serial.println(F("6C 75 61 74 69 6E 67 20 47 72 6F 75 70 20 4D 61 70 70 69"));
    Serial.println(F("6E 67 20 50 6F 6C 69 63 79 0A 4C 44 41 50 20 66 65 74 63"));
    Serial.println(F("68 20 66 6F 75 6E 64 20 6E 6F 20 6D 61 74 63 68 69 6E 67"));
    Serial.println(F("20 61 63 63 6F 75 6E 74 20 69 6E 20 64 6F 6D 61 69 6E 0A"));
    Serial.println(F("4D 61 63 68 69 6E 65 20 61 75 74 68 65 6E 74 69 63 61 74"));
    Serial.println(F("69 6F 6E 20 61 67 61 69 6E 73 74 20 41 63 74 69 76 65 20"));
    Serial.println(F("44 69 72 65 63 74 6F 72 79 20 66 61 69 6C 65 64 20 73 69"));
    Serial.println(F("6E 63 65 20 6D 61 63 68 69 6E 65 20 69 73 20 63 6F 6E 73"));
    Serial.println(F("69 64 65 72 65 64 20 74 6F 20 62 65 20 69 6E 20 72 65 73"));
    Serial.println(F("74 72 69 63 74 65 64 20 6C 6F 67 6F 6E 20 68 6F 75 72 73"));
    Serial.println(F("0A 41 73 73 65 72 74 69 6F 6E 20 64 6F 65 73 20 6E 6F 74"));
    Serial.println(F("20 63 6F 6E 74 61 69 6E 20 73 75 62 6A 65 63 74 20 63 6F"));
    Serial.println(F("6E 66 69 72 6D 61 74 69 6F 6E 0A 55 73 65 72 20 72 65 63"));
    Serial.println(F("6F 72 64 20 77 61 73 20 63 61 63 68 65 64 20 69 6E 20 50"));
    Serial.println(F("61 73 73 63 6F 64 65 20 63 61 63 68 65 0A 49 64 65 6E 74"));
    Serial.println(F("69 74 79 20 72 65 73 6F 6C 75 74 69 6F 6E 20 62 79 20 63"));
    Serial.println(F("65 72 74 69 66 69 63 61 74 65 20 66 6F 75 6E 64 20 61 6D"));
    Serial.println(F("62 69 67 75 6F 75 73 20 61 63 63 6F 75 6E 74 73 0A 53 74"));
    Serial.println(F("61 72 74 75 70 20 43 6F 6D 70 6C 65 74 65 21 2E 2E 2E 2E"));
    return true;
}

/**************************************************************************/
/*!
  @brief    Converts message in bytes and prints it.
  @param    str             String of text that needs to be printed
  @return   bool            If conversion is successfull
*/
/**************************************************************************/
bool printStringInBytes(String str) {
    uint8_t messLength = str.length() + 1;
    /* Check if string is not too long */
    if (messLength > LENGTH) {
      return false;
    }
    unsigned char messBytes[messLength];
    str.getBytes(messBytes, messLength);
    uint8_t i;
    for (i = 0; i < messLength; i++) {
      if (messBytes[i] != 0) {
        Serial.print(messBytes[i], HEX);
        Serial.print(" ");
      }
    }
    Serial.print("0A ");
    i++;
    /* Print . untill end of line, to match random data */
    while (i < LENGTH-1) {
      Serial.print("2E ");
      i++;
    }
    Serial.println("2E");
    return true;
}
#endif
