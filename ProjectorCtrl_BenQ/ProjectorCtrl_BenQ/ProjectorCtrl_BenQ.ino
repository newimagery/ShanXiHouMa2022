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
#define ComputerRelay  4 //D2


#define HW_UART_SPEED                 115200L
#define MQTT_ID                     "PROJECTOR_A_"

//const char* ssid = "NewImagery";
//const char* password = "*#01234567890";
const char* ssid = "zhongkong";
const char* password = "1234567890";
const char* mqtt_server = "192.168.1.201";

const char* TOPIC_PROJ = "ProjectorA";
const char* MSG_OFF    = "off";
const char* MSG_ON     = "on";


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
