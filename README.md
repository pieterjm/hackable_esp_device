# Hackable ESP8266 device
Firmware for ESP8266 based device (D1 Mini board) with designed vunerabilities to practice ethical hacking. The software is tested on the following boards:

* [D1 Mini](https://www.berrybase.de/en/dev.-boards/esp8266-esp32-d1-mini/)
## Getting Started

These instructions will get you a copy of the project up and running on your D1 Mini (or other ESP8266 based boards) for development or hacking purposes.

### Prerequisites

The software is written, compiled and uploaded using the [Arduino IDE](https://www.arduino.cc/en/software). Platform.io and Visual Studio Code can be used as well. Use the script to convert the project to a Platform.io.

### Dependecies


#### Libaries

- ESP for Arduino IDE
- ESP Async WebServer V1.2.3
- ESPAsyncWebServer-esphome V2.1.0
- Neotimer V1.1.6

#### Files
- config.h

### Installing

<b>General install</b>
1. Install the [driver](https://github.com/HobbyComponents/CH340-Drivers) for the esp8266
2. Clone the repository

There are multiple ways to upload the program files to the board. The two ways listed here are using Arduino IDE and Platformio on visual studio code

<b>Arduino IDE</b>

1. Install the [Arduino IDE](https://www.arduino.cc/en/software)
2. [Add the esp8266 libaries to Arduino IDE](https://www.nonscio.com/blog/installing-esp8266-libraries-to-the-arduino-ide)
3. Follow [this](https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide) tutorial about the SPIFFS.
4. Navigate to the `hackableEspDevice` folder.
5. Open `hackableEspDevice.ino`.
6. Upload the files in the `data` folder (see the tutorial).
7. Upload the program to the device.
8. Connect to the `Configure Smartlight Wifi` AP to configure the wifi.

<b>Visual Studio Code + Platformio</b>
1. Install the [Platformio](https://platformio.org/install/ide?install=vscode) plugin.
2. Perpare files for platformio
   - Run the `toPlatformio.ps1` script and select the copy or sybolic option
   - Run the `toPlatformio.ps1` script and select fix
   - Or perpare the files manualy see manual prep platformio
3. Open visual studio code in the `HackableEspDevicePlatformio` directory
4. In visual studio code open the project in the platformio addon. `(Platoformio > Projects > open HackableEspDevicePlatformio)`
5. Upload the program `(project tasks > General> Upload)`
6. Upload the filesystem Image `(Project tasks > Platform > Upload filesystem Image)`
7.  Done the device should now be ready for use

### Manual platformio prep
1. create the correct hiarchy 

    |HackableEspDevicePlatformio\ \
    |--- platformio.ini \
    |--- src\ \
    |--- src\main.cpp\
    |--- data\ 

1. the src dir needs to contain all the files from the `hackableEspDevice` directory except the data directory
2. rename the `hackableEspDevice.ino` to `main.cpp`
3. in `main.cpp` add a reference to all functions in main eg.
void setup();\
void setup();\
void initializeHostname();\
void connectWifi();\
void initializeServer();\
void loop();\
String processor(const String& var);\
String getContentType(String filename);\
void handleFileRequest(String path, uint8_t permissionLevel);\
void handleFileUpload();\
void handleFileDownload();
4. Move the platoformio.ini file from the root dir to the `hackableEspDevicePlatoformio` dir
5. Copy all files from `hackableEspDevice\data` to `hackableEspDevicePlatformio\data`

## Hardware

* 1x D1 Mini Board
* 1x USB to USB-mini cable

## Questions or feedback?

There is technical documentation available if you want to contribute to this project. There is an user manual as well, contact us for information. You can open an issue if you have questions or feedback for this repository.

## Authors

* **Luke de Munk** - *Head author* - [LinkedIn](https://www.linkedin.com/in/luke-de-munk/)
* **Thijs Takken** - *Head author* - [LinkedIn](https://www.linkedin.com/in/)
* **Christina Kostine** - *Head author* - [LinkedIn](https://www.linkedin.com/in/)
* **Twenne Elffers** - *Head author* - [LinkedIn](https://www.linkedin.com/in/)

<!-- ## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details -->