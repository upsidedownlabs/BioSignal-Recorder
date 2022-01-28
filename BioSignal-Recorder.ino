// EXG-Visualizer
// https://github.com/upsidedownlabs/BioSignal-Recorder

// Copyright (c) 2021 Moteen Shah moteenshah.02@gmail.com


// Upside Down Labs invests time and resources providing this open source code,
// please support Upside Down Labs and open-source hardware by purchasing
// products from Upside Down Labs!
// Copyright (c) 2021 Upside Down Labs - contact@upsidedownlabs.tech

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.


// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <WebSocketsServer.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#define ADC_PIN 36

WebSocketsServer webSocket = WebSocketsServer(81);
const char *SSID = "";
const char *PASSWORD = "";

String myString[3] = {"0", "-1", "25"}; //1st index used for ADC data, 2nd as dummy value per sample, 3rd for timing

String JSONtxt;
AsyncWebServer server(80);

String readADC(){
  float t = analogRead(ADC_PIN);
  return String(t);
}

void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html"); });

  server.begin();
  webSocket.begin();
}

void loop(){
  webSocket.loop();
  String ADCVal = String(readADC().c_str());

  myString[0] = ADCVal;
  JSONtxt = "{\"ADC1\":\"" + myString[0] + "\",";
  JSONtxt += "\"ADC2\":\"" + myString[1] + "\",";
  JSONtxt += "\"ADC3\":\"" + myString[2] + "\"}";

  webSocket.broadcastTXT(JSONtxt);
}
