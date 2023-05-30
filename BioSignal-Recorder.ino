// EXG-Visualizer
// https://github.com/upsidedownlabs/BioSignal-Recorder

// Copyright (c) 2023 Mahesh Tupe tupemahesh91@gmail.com


// Upside Down Labs invests time and resources providing this open source code,
// please support Upside Down Labs and open-source hardware by purchasing
// products from Upside Down Labs!
// Copyright (c) 2023 Upside Down Labs - contact@upsidedownlabs.tech

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
#include <driver/adc.h>
#include <math.h>

WebSocketsServer webSocket = WebSocketsServer(81);

// Add SSID and PASSWORD for network you are connected to
const char *SSID = "";
const char *PASSWORD = "";

AsyncWebServer server(80);

volatile int interruptCounter[4] = {0};

// Initialize timer interrupts
hw_timer_t * timer_1 = NULL;
hw_timer_t * timer_2 = NULL;
hw_timer_t * timer_3 = NULL;
hw_timer_t * timer_4 = NULL;

portMUX_TYPE timerMux_1 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux_2 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux_3 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux_4 = portMUX_INITIALIZER_UNLOCKED;

// counter for each timer
void IRAM_ATTR onTimer_1() {
  portENTER_CRITICAL_ISR(&timerMux_1);
  interruptCounter[0]++;
  portEXIT_CRITICAL_ISR(&timerMux_1);
}
void IRAM_ATTR onTimer_2() {
  portENTER_CRITICAL_ISR(&timerMux_2);
  interruptCounter[1]++;
  portEXIT_CRITICAL_ISR(&timerMux_2);
}
void IRAM_ATTR onTimer_3() {
  portENTER_CRITICAL_ISR(&timerMux_3);
  interruptCounter[2]++;
  portEXIT_CRITICAL_ISR(&timerMux_3);
}
void IRAM_ATTR onTimer_4() {
  portENTER_CRITICAL_ISR(&timerMux_4);
  interruptCounter[3]++;
  portEXIT_CRITICAL_ISR(&timerMux_4);
}


void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/index.html");
  });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(callback);

}

bool sample = false;
int sampling_rate = 100;


int adc[4];
int channel_count = 0;
int total_channel = 0;
void callback(byte num, WStype_t type, uint8_t * payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.println("Client Disconnected");
      sample = false;
      channel_count = 0;
      total_channel = 0;
      break;
    case WStype_CONNECTED:
      Serial.println("Client connected");
      sample = true;
      break;
    case WStype_TEXT:
      String rate;
      String gpio;
      total_channel++;
      gpio += (char)payload[length - 1];
      gpio += '\n';
      adc[channel_count] = gpio.toInt();
      Serial.print("Channel: ");
      Serial.println(gpio);

      for (int i = 0; i < length - 1; i++)
      {
        rate += (char)payload[i];
      }
      rate += '\n';
      sampling_rate = rate.toInt();
      Serial.print("Sampling rate: ");
      Serial.println(rate);

      send_samples(sampling_rate, adc[channel_count], channel_count);
      channel_count++;
  }
}

uint16_t **buffer_add = (uint16_t **)calloc(4, sizeof(uint16_t *));


void send_samples(int sampling_rate, int adc, int channel_count)
{

  int tick_count = 1000000 / sampling_rate;
  switch (channel_count)
  {
    case 0:
      timer_1 = timerBegin(0, 80, true);
      timerAttachInterrupt(timer_1, &onTimer_1, true);
      timerAlarmWrite(timer_1, tick_count, true);
      timerAlarmEnable(timer_1);
      buffer_add[0] = (uint16_t*)calloc(round((float)sampling_rate / 30.0) + 2, sizeof(uint16_t));
      break;

    case 1:
      timer_2 = timerBegin(1, 80, true);
      timerAttachInterrupt(timer_2, &onTimer_2, true);
      timerAlarmWrite(timer_2, tick_count, true);
      timerAlarmEnable(timer_2);
      buffer_add[1] = (uint16_t*)calloc(round((float)sampling_rate / 30.0) + 2, sizeof(uint16_t));
      break;

    case 2:
      timer_3 = timerBegin(2, 80, true);
      timerAttachInterrupt(timer_3, &onTimer_3, true);
      timerAlarmWrite(timer_3, tick_count, true);
      timerAlarmEnable(timer_3);
      buffer_add[2] = (uint16_t*)calloc(round((float)sampling_rate / 30.0) + 2, sizeof(uint16_t));
      break;

    case 3:
      timer_4 = timerBegin(3, 80, true);
      timerAttachInterrupt(timer_4, &onTimer_4, true);
      timerAlarmWrite(timer_4, tick_count, true);
      timerAlarmEnable(timer_4);
      buffer_add[3] = (uint16_t*)calloc(round((float)sampling_rate / 30.0) + 2, sizeof(uint16_t));
      break;

  }
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten((adc1_channel_t)(adc), ADC_ATTEN_DB_11);
}

static long num_counter = 0;
static long buffer_counter[4] = {0};
portMUX_TYPE * timer_mux = NULL;
void loop() {
  webSocket.loop();

  for (int i = 0; i < total_channel; i++)
  {
    switch (i)
    {
      case 0:
        timer_mux = &timerMux_1;
        break;
      case 1:
        timer_mux = &timerMux_2;
        break;
      case 2:
        timer_mux = &timerMux_3;
        break;
      case 3:
        timer_mux = &timerMux_4;
        break;
    }
    if (interruptCounter[i] > 0)
    {

      portENTER_CRITICAL(timer_mux);
      interruptCounter[i]--;
      portEXIT_CRITICAL(timer_mux);

      if (buffer_counter[i] < round((float)sampling_rate / 30.0))
      {
        buffer_add[i][buffer_counter[i]] = adc1_get_raw((adc1_channel_t)adc[i]) & 0x0FFF;
        buffer_counter[i]++;
      }
      else
      {
        if (num_counter < 100)
        {
          num_counter++;
        }
        else
        {
          num_counter = 0;
        }
        buffer_add[i][buffer_counter[i]] = num_counter & 0x0FFF;
        buffer_counter[i]++;
        buffer_add[i][buffer_counter[i]] = i & 0x0FFF;
        buffer_counter[i]++;
        webSocket.sendBIN(0, (uint8_t *)&buffer_add[i][0], buffer_counter[i]*sizeof(uint16_t));
        buffer_counter[i] = 0;
      }
    }

  }
}
