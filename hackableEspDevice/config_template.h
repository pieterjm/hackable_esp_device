/*
 * File:      config.h
 * Author:    Luke de Munk
 * 
 * Configuration options can be configured here. This file
 * is in the .gitignore list, so this file will not be pushed.
 */
#ifndef CONFIG_H
#define CONFIG_H

/* Network credentials */
#define WIFI_SSID       "ssid"
#define WIFI_PASSWORD   "password"

/* Hostname */
#define HOSTNAME      "smartlight"

/* Login credentials */
#define HTTP_USERNAME   "admin"
#define HTTP_PASSWORD   "admin"

#define DEBUG 0                                                             //For defining serial debugging

/*
 * This precompiler functions are there for switching on and off the serial debugger
 */
#if DEBUG == 1
    #define debug(x) Serial.print(x)
    #define debugln(x) Serial.println(x)
#else
    #define debug(x)
    #define debugln(x)
#endif
