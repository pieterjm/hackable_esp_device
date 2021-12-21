# Hackable ESP8266 device
Firmware for ESP8266 based device (D1 Mini board) with designed vunerabilities to practice ethical hacking. The software is tested on the following boards:

* D1 Mini

## Getting Started

These instructions will get you a copy of the project up and running on your D1 Mini (or other ESP8266 based boards) for development or hacking purposes.

### Prerequisites

The software is written, compiled and uploaded using the [Arduino IDE](https://www.arduino.cc/en/software). Platform.io and Visual Studio Code can be used as well. Use the script to convert the project to a Platform.io.

### Dependecies

No dependecies yet.

### Installing

There are multiple ways to upload the program files to the board. First instructions for the Arduino IDE are described. Then instructions for Visual Studio Code are described.

<b>Arduino IDE</b>
1. Clone the repository.
2. Follow [this](https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide) tutorial about the SPIFFS.
3. Navigate to the `hackableEspDevice` folder.
4. Open `hackableEspDevice.ino`.
5. Upload the files in the `data` folder (see the tutorial).
6. Upload the program to the device.
7. Connect to the `Configure Smartlight Wifi` AP to configure the wifi.

<b>Visual Studio Code</b>
1. Install the [Platformio](https://platformio.org/install/ide?install=vscode) plugin.
2. ??

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