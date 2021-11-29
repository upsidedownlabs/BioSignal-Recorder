# BioSignal-Recorder
A utility for visualizing and recording Biopotential Signals using Upside Down Labs BioAmp.

## Demonstration 
![image info](./assets/randomADC.png)

## Setup instructions

1. Open BioSignal-Recorder.ino with Arduino IDE, you can change the input ADC pin if required.
2. Find and change SSID to your WiFi SSID and PASSWORD to your WiFi password.
3. In your Arduino IDE under Tools|Board make sure you choose "ESP32 Dev Module" or a board allowing partition schemes.
4. Compile sketch and run it on your ESP32.
5. In your Arduino IDE under Tools, click ESP32 Sketch Data Upload, BioSignal-Recorder uses spiffs file system.
6. Go to the serial monitor, you can find IP for the webpage, you might have to hard reset your esp32 at this stage if the IP is not displayed.


## Resources
The frontend of this project inspired from [BojunJurca's Esp32_oscilloscope](https://github.com/BojanJurca/Esp32_oscilloscope) project.
