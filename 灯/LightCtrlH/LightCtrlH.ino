/*
 *******************************************************************************
 *
 * Purpose: Example of using the Arduino MqttClient with Esp8266WiFiClient.
 * Project URL: https://github.com/monstrenyatko/ArduinoMqtt
 *
 *******************************************************************************
 * Copyright Oleg Kovalenko 2017.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
// Enable MqttClient logs
#define MQTT_LOG_ENABLED 1
// Include library
#include <MqttClient.h>
#define LigntRelay  12

#define HW_UART_SPEED                 115200L
#define MQTT_ID                     "LIGHT_H_"

const char* ssid = "zhongkong";
const char* password = "1234567890";
const char* mqtt_server = "192.168.1.201";

const char* TOPIC_PLAY = "PlayingH";

const char* MSG_PLAY   = "play";
const char* MSG_PAUS   = "pause";
const char* MSG_STOP   = "stop";


//unsigned long playTime = 213000;//ms
unsigned long playTime = 260000;//ms

static MqttClient *mqtt = NULL;
static WiFiClient network;

#define LOG_PRINTFLN(fmt, ...)  logfln(fmt, ##__VA_ARGS__)
#define LOG_SIZE_MAX 128
void logfln(const char *fmt, ...) {
  char buf[LOG_SIZE_MAX];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, LOG_SIZE_MAX, fmt, ap);
  va_end(ap);
  Serial.println(buf);
}

// ============== Object to supply system functions ============================
class System: public MqttClient::System {
public:

  unsigned long millis() const {
    return ::millis();
  }

  void yield(void) {
    ::yield();
  }
};
