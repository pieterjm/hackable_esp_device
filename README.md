![Hackable IoT LED Controller](/images/Hackable-IoT-LED-Controller-showcase.png)

# Hackable ESP8266 device
Firmware for ESP8266 based device (D1 Mini board) with designed vulnerabilities to practice ethical hacking. The software is tested on the following boards:

* [D1 Mini](https://www.berrybase.de/en/dev.-boards/esp8266-esp32-d1-mini/)

## Getting Started

These instructions will get you a copy of the project up and running on your D1 Mini (or other ESP8266 based boards) for development or hacking purposes.

### Prerequisites

The software is written, compiled and uploaded using the [Arduino IDE](https://www.arduino.cc/en/software). Platform.io and Visual Studio Code can be used as well. Use the script to convert the project to a Platform.io.

### Dependencies

- ESP for Arduino IDE
- ESP Async WebServer V1.2.3
- Wifimanager V0.16.0

### Installing

<b>General Install</b>
1. Install the [driver](https://github.com/HobbyComponents/CH340-Drivers) for the esp8266.
2. Clone the repository.

There are multiple ways to upload the program files to the board. The two ways listed here are using Arduino IDE and Platformio on Visual Studio Code.

#### <b>Option 1: Arduino IDE</b>

1. Install the Arduino IDE, this can be found in the [Microsoft store](https://www.microsoft.com/en-us/p/arduino-ide/9nblggh4rsd8) or on their [website](https://www.arduino.cc/en/software).
2. [Add the esp8266 libraries to Arduino IDE](https://www.nonscio.com/blog/installing-esp8266-libraries-to-the-arduino-ide).

3. Go to Tools > Board: ”xxxx xxxx” and you will see a new option for ESP8266 Boards, select the `LOLIN(WEMOS) D1 R2 & mini`.

4. Download [SPIFF](https://github.com/esp8266/arduino-esp8266fs-plugin/releases/tag/0.5.0), unpack the zip file and place the `esp8266fs.jar` in `This PC > Documents > Arduino > Tools > ESP8266FS > tool > esp8266fs.jar`, you will need to create some of the folders, just make sure the arduino folder already existed.
5. Reboot Arduino IDE and now you can find the option ESP8266 Data Upload under the tools bar.
6. In Arduino IDE go to File > Open and in the hackable ESP folder from the github you go to hackable_esp_device-main > hackableEspDevice and open the `hackableEspDevice.ino` file.
7. In Arduino IDE go to Tools > Port and select the COM port that the ESP is connected with. You can find this in Device Manager under Ports (COM & LPT).
8. Now first upload the data by clicking on Tools > `ESP8266 Sketch Data upload`.
9. Now you can upload the code by clicking the arrow on the top left corner.
9. The Esp has now been flashed with the needed code. Use a device with a Wi-Fi adapter to connect with the new access point. 
10. A website will open up where you can connect to an existing Wi-Fi network. Connect with one of the networks and check which IP the ESP has gotten. And now you can start hacking!

#### <b>Option 2: Visual Studio Code + Platformio</b>
1. Install the [Platformio](https://platformio.org/install/ide?install=vscode) plugin.
2. Prepare files for platformio.
   - Run the `toPlatformio.ps1` script and select the copy or symbolic option.
        - Symbolic changes the original ideal for editing the files.
        - Copy simply copies the files to a new location for platformio files.
   - Run the `toPlatformio.ps1` script and select fix.
   - Or prepare the files manually see manual prep platformio.
3. Open visual studio code in the `HackableEspDevicePlatformio` directory.
4. In visual studio code open the project in the platformio addon. `(Platoformio > Projects > open HackableEspDevicePlatformio)`.
5. Upload the program `(project tasks > General> Upload)`.
6. Upload the filesystem Image `(Project tasks > Platform > Upload filesystem Image)`.
7. Done. The device should now be ready for use.

### Manual Platformio Prep
1. Create the correct hierarchy.

```
    |HackableEspDevicePlatformio\
    |--- platformio.ini
    |--- src\
    |--- src\main.cpp\
    |--- data\
```

1. The src dir needs to contain all the files from the `hackableEspDevice` directory except the data directory.
2. Rename the `hackableEspDevice.ino` to `main.cpp`.
3. In `main.cpp` add a reference to all functions in main e.g.
```
void setup();
void setup();
void initializeHostname();
void connectWifi();
void initializeServer();
void loop();
String getContentType(String filename);
void handleFileRequest(String path, uint8_t permissionLevel);
void handleFileUpload();
void handleFileDownload();
```
4. Move the `platformio.ini` file from the root dir to the `hackableEspDevicePlatformio` dir.
5. Copy all files from `hackableEspDevice\data` to `hackableEspDevicePlatformio\data`.

### <b>Running</b>

#### <b>Wifi Manager First Boot</b>
1. Start up the device.
2. Connect to the `Configure Smartlight Wifi` via a mobile device.
3. Go to the IP address listed in the serial monitor. Most of the time this is [http://192.168.4.1](http://192.168.4.1).
4. Follow the steps on the website to configure a wifi connection.
5. The device should now restart, connect to the selected wifi network and be ready for use.

### Customization of Hackable ESP (Contains spoilers) (Look in raw version of readme.md)
<!--
Default user and passwd
It is possible to change the default username and password for the users on the webpage.
The file for this is /hackableEspDevice/data/config.conf. 
Here the username and password is stored and can be changed to increase or decrease the challenge. the format is “<Username>:<Password>:<authorization level (1|2)>” 
The file is encrypted using the ecnryption command in the serial interface of the device. `gpg --encrypt "key" "string"`
They key is found with the `privatekeys` command
Level 1 is a normal user, level 2 is an admin user.


config.h
/hackableEspDevice/config.h also contains a few interesting possible settings.
The settings that can be changed are the:
* The default hostname for the device (DEFAULT_HOSTNAME)
* Access Point name for the configuration phase (WIFI_CONF_AP_NAME)
* The root password for the Serial interface (ROOT_PASSWORD)
-->

## Hardware

* 1x D1 Mini Board
* 1x USB to USB-mini cable
* 1x ESP8266 casing

## Questions or Feedback?

There is technical documentation available if you want to contribute to this project. There is a user manual as well, contact us for information. You can open an issue if you have questions or feedback for this repository.

## Authors

* **Luke de Munk** - *Head author* - [LinkedIn](https://www.linkedin.com/in/luke-de-munk/)
* **Thijs Takken** - *Head author* - [LinkedIn](https://www.linkedin.com/in/thijstakken)
* **Christina Kostine** - *Head author* - [LinkedIn](https://www.linkedin.com/in/christine-kostine)
* **Twenne Elffers** - *Head author* - [LinkedIn](https://www.linkedin.com/in/twenne-elffers/)

<!-- ## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details -->