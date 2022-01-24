/*
 * File:      config.h
 * Author:    Luke de Munk
 * 
 * Configuration options can be configured here.
 */
#ifndef CONFIG_H
#define CONFIG_H

/* Hostname */
#define DEFAULT_HOSTNAME        "smartlight"
#define HOSTNAME_ADRESS         1
#define MAX_HOSTNAME_LENGTH     32
#define WIFI_CONF_AP_NAME       "Configure Smartlight WiFi"

#define HTTP_CONFIG_LOCATION    "/config.conf"

/* Permission levels */
#define PERMISSION_LVL_ALL      0
#define PERMISSION_LVL_USER     1
#define PERMISSION_LVL_ADMIN    2

#define USER_INFO_LENGTH        3                                           //Username, password and permission level
#define MAX_NUMBER_USERS        10

#define ROOT_PASSWORD           "p@$Sw0rd"                                  //For serial debug

/* Encryption */
#define AES_KEY                 "ThisIsNotTheKey!"                          //Key to encrypt and decript
#define CBC_INIT_VECTOR         36753562                                    //For CBC (encryption) padding

#define AES_KEY_SIZE            16
#define AES_BIT_SIZE            128
#endif
