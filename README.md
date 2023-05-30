# BioSignal-Recorder
A utility for visualizing and recording Biopotential Signals using Upside Down Labs BioAmp.

## Demonstration 
<img src="./assets/ADC1.png" alt="Single Chanel ADC"  width="600" height="300">
<img src="./assets/ADC2.png" alt="Multi Chanel ADC"  width="600" height="300">

## Prerequisites
1. [ESP32 Devkitc V1](https://www.espressif.com/en/products/devkits/esp32-devkitc)
2. [Arduino IDE](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE)
3. [ESP32 Filesystem Uploader in Arduino](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/)
4. [Upside Down Labs BioAmp](https://store.upsidedownlabs.tech/shop/)

## Connections
<img src="./assets/connections.png" alt="Electrical Connection"  width="600" height="300">

## Setup Instructions
1. Open BioSignal-Recorder.ino with Arduino IDE.
2. Find and change SSID to your WiFi SSID and PASSWORD to your WiFi password.
3. In your Arduino IDE under Tools|Board make sure you choose "ESP32 Dev Module" or a board allowing partition schemes.
4. Compile sketch and run it on your ESP32.
5. In your Arduino IDE under Tools, click ESP32 Sketch Data Upload, BioSignal-Recorder uses spiffs file system.
6. Go to the serial monitor, you can find IP for the webpage, you might have to hard reset your esp32 at this stage if the IP is not displayed.

## Features
| Feature                   | Description                                           |
| :------------------------:| :----------------------------------------------------:|
| Multichannel              | Upto 4 GPIO pins can be used for taking analog reading|
| Variable sampling rate    | Separate sampling rate can be set for all every pin   |
|Save as CSV                | Analog data can be saved in CSV file                  |

Note: ESP32 has 2 ADC units. ADC1 and ADC2. ADC2 is shared with WIFI module hence we can only use pins in ADC1 channel.
     ADC1 has 8 pins, GPIO 32-39.

## Resources
The frontend of this project inspired from [BojunJurca's Esp32_oscilloscope](https://github.com/BojanJurca/Esp32_oscilloscope) project.
