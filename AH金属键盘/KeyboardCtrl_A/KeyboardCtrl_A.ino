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
//这个代码用在山西侯马彭真馆的『山西建党』子项目中
//2023-05-09

#include <Arduino.h>
#include <ESP8266WiFi.h>
// Enable MqttClient logs
#define MQTT_LOG_ENABLED 1
// Include library
#include <MqttClient.h>
#define ComputerRelay 4  //D2


#define HW_UART_SPEED 9600L
#define MQTT_ID "KEYBOARD_AA_"//之前用的是A但是另一个应该是H 也被写成了A 所以这个地方就改成了AA 不然会引起ID冲突

//测试环境中的WIFI
//const char* ssid = "NewImagery";
//const char* password = "*#01234567890";
//const char* mqtt_server = "iServer";

//使用环境中的WIFI
const char* ssid = "zhongkong";
const char* password = "1234567890";
const char* mqtt_server = "192.168.1.201";

const char* TOPIC_COMP = "ComputerA";
const char* TOPIC_PROJ = "ProjectorA";
const char* TOPIC_PLAY = "PlayingA";
const char* TOPIC_VOL = "VolumeA";

const char* MSG_CLIK = "click";
const char* MSG_OFF = "off";
const char* MSG_ON = "on";
const char* MSG_PLAY = "play";
const char* MSG_PAUS = "pause";
const char* MSG_STOP = "stop";
const char* MSG_UP = "+";
const char* MSG_DOWN = "-";
const char* MSG_MUTE = "mute";

#define KEY0 0X30
#define KEY1 0X31
#define KEY2 0X32
#define KEY3 0X33
#define KEY4 0X34
#define KEY5 0X35
#define KEY6 0X36
#define KEY7 0X37
#define KEY8 0X38
#define KEY9 0X39
#define KEYa 0X1B
#define KEYb 0X0D


static MqttClient* mqtt = NULL;
static WiFiClient network;

#define LOG_PRINTFLN(fmt, ...) logfln(fmt, ##__VA_ARGS__)
#define LOG_SIZE_MAX 128
void logfln(const char* fmt, ...) {
  char buf[LOG_SIZE_MAX];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, LOG_SIZE_MAX, fmt, ap);
  va_end(ap);
  Serial.println(buf);
}

// ============== Object to supply system functions ============================
class System : public MqttClient::System {
public:

  unsigned long millis() const {
    return ::millis();
  }

  void yield(void) {
    ::yield();
  }
};