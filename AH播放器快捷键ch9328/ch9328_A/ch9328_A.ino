/*
 * CH9328Keyboard Library example(Hello World).
 * CH9328键盘库范例程序
 * ===============English Ver.=================== 
 * Connect your CH9328's RST to Arduino/ESP8266/ESP32's any GPIO and 
 * connect your CH9328's RxD to Arduino/ESP8266/ESP32's TxD.
 * Maybe you can use a software serial, but I haven't tested it.
 * Tested on ESP8266.
 * ==================中文原版===================== 
 * CH9328的复位连接到Arduino的任意一引脚，RxD连到TxD
 * 可能可以用软件串口，但是我还没试过。
 * 在ESP8266上测试通过。
 * 如果你想改变默认的9600波特率，在Library/CH9328Keyboard/extra下有配置工具，同时也有数据手册，可以参考。
====================== License ========================
Copyright (c) Lxy <1452206376@qq.com>. All right reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
=======================================================
 */
#include <CH9328Keyboard.h>
#define PINRST 10
#define BAUDRATE 9600  //Default is 9600.



#include <Arduino.h>
#include <ESP8266WiFi.h>
// Enable MqttClient logs
#define MQTT_LOG_ENABLED 1
// Include library
#include <MqttClient.h>
#define ComputerRelay 4  //D2


#define HW_UART_SPEED 115200L
#define MQTT_ID "HOTKEYESP_A_"
//const char* ssid = "NewImagery";
//const char* password = "*#01234567890";
const char* ssid = "zhongkong";
const char* password = "1234567890";
const char* mqtt_server = "192.168.1.201";

const char* TOPIC_PLAY = "PlayingA";
const char* TOPIC_VOL = "VolumeA";

const char* MSG_MUTE = "mute";
const char* MSG_OFF = "off";
const char* MSG_ON = "on";
const char* MSG_PLAY = "play";
const char* MSG_PAUS = "pause";
const char* MSG_STOP = "stop";
const char* MSG_UP = "+";
const char* MSG_DOWN = "-";

//#define KEY_COMP_OFF     8
#define KEY_PLAY_PLAY 3
#define KEY_PLAY_PAUS 4
#define KEY_PLAY_STOP 5
#define KEY_VOL_DOWN 6
#define KEY_VOL_UP 7


bool bPause = false;
bool bStart = false;


//============================================

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


void setup() {
  Serial.begin(HW_UART_SPEED);
  Serial1.end();
  Keyboard.begin(&Serial1, PINRST, BAUDRATE);
  //  delay(1000);
  //  Keyboard.releaseAll();
  //  Keyboard.press(KEY_LEFT_GUI);
  //  Keyboard.press('r');
  //  delay(50);
  //  Keyboard.releaseAll();
  //  delay(500);
  //  Keyboard.println("notepad");
  //  delay(1000);
  //  Keyboard.print("Hello world From CH9328Keyboard Library\n");
  //  Keyboard.write('\n');
  //  delay(10);
  //  Keyboard.press(KEY_LEFT_GUI);
  //  delay(10);
  //  Keyboard.release(KEY_LEFT_GUI);

  while (!Serial)
    ;

  // Setup WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.hostname("ESP_" MQTT_ID);
  WiFi.begin(ssid, password);
  LOG_PRINTFLN("\n");
  LOG_PRINTFLN("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    LOG_PRINTFLN(".");
  }
  LOG_PRINTFLN("Connected to WiFi");
  LOG_PRINTFLN("IP: %s", WiFi.localIP().toString().c_str());

  // Setup MqttClient
  MqttClient::System* mqttSystem = new System;
  MqttClient::Logger* mqttLogger = new MqttClient::LoggerImpl<HardwareSerial>(Serial);
  MqttClient::Network* mqttNetwork = new MqttClient::NetworkClientImpl<WiFiClient>(network, *mqttSystem);
  //// Make 128 bytes send buffer
  MqttClient::Buffer* mqttSendBuffer = new MqttClient::ArrayBuffer<128>();
  //// Make 128 bytes receive buffer
  MqttClient::Buffer* mqttRecvBuffer = new MqttClient::ArrayBuffer<128>();
  //// Allow up to 2 subscriptions simultaneously
  MqttClient::MessageHandlers* mqttMessageHandlers = new MqttClient::MessageHandlersImpl<2>();
  //// Configure client options
  MqttClient::Options mqttOptions;
  ////// Set command timeout to 10 seconds
  mqttOptions.commandTimeoutMs = 10000;
  //// Make client object
  mqtt = new MqttClient(
    mqttOptions, *mqttLogger, *mqttSystem, *mqttNetwork, *mqttSendBuffer,
    *mqttRecvBuffer, *mqttMessageHandlers);
}


// ============== Subscription callback ========================================
void processMessage(MqttClient::MessageData& md) {
  const MqttClient::Message& msg = md.message;
  char payload[msg.payloadLen + 1];
  char topic[md.topicName.lenstring.len + 1];
  memcpy(payload, msg.payload, msg.payloadLen);
  memcpy(topic, md.topicName.lenstring.data, md.topicName.lenstring.len);
  payload[msg.payloadLen] = '\0';
  topic[md.topicName.lenstring.len] = '\0';
  //  LOG_PRINTFLN(
  //    "Message arrived: qos %d, retained %d, dup %d, packetid %d, payload:[%s]",
  //    msg.qos, msg.retained, msg.dup, msg.id, payload
  //  );


  if (strcmp(topic, TOPIC_PLAY) == 0) {
    if (strcmp(payload, MSG_PLAY) == 0) {
      if (bPause) {
        Keyboard.releaseAll();
        //  Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('P');
        /////////////////////////////修改快捷键///////////////////////////////////


        ///////////////////////////////////////////////////////////////////////////
        delay(50);
        Keyboard.releaseAll();
        //          Serial1.println(KEY_PLAY_PAUS);
        //          Serial.println(KEY_PLAY_PAUS);

        //  delay(1000);
        //  Keyboard.println("notepad");
        //  delay(1000);
        //  Keyboard.write('\n');
        //  delay(1000);
        //  Keyboard.print("Hello world From CH9328Keyboard Library\n");
        //  delay(1000);
        //  Keyboard.write('\n');
      } else {
        Keyboard.releaseAll();
        //  Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('P');
        delay(50);
        Keyboard.releaseAll();
        //          Serial1.println(KEY_PLAY_PLAY);
        //          Serial.println(KEY_PLAY_PLAY);
        //        Keyboard.releaseAll();
        ////          delay(100);
        ////          Keyboard.press('s');
        ////          delay(100);
        //        Keyboard.releaseAll();
      }
      bPause = false;
      LOG_PRINTFLN("MSG_PLAY");
    } else if (strcmp(payload, MSG_PAUS) == 0) {
      if (!bPause) {
        Keyboard.releaseAll();
        //  Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('Z');
        delay(50);
        Keyboard.releaseAll();
        //            Serial1.println(KEY_PLAY_PAUS);
        //            Serial.println(KEY_PLAY_PAUS);
        //        Keyboard.releaseAll();
        ////          delay(100);
        //Keyboard.press('r');
        ////          delay(100);
        //        Keyboard.releaseAll();
        bPause = true;
        LOG_PRINTFLN("MSG_PUSE");
      }

    } else if (strcmp(payload, MSG_STOP) == 0) {
      Keyboard.releaseAll();
      //  Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('S');
      delay(50);
      Keyboard.releaseAll();
      //        Serial1.println(KEY_PLAY_STOP);
      //        Serial.println(KEY_PLAY_STOP);


      //        Keyboard.releaseAll();
      ////          delay(100);
      //Keyboard.press('r');
      ////          delay(100);
      //        Keyboard.releaseAll();
      bPause = false;
      LOG_PRINTFLN("MSG_STOP");
    }

  } else if (strcmp(topic, TOPIC_VOL) == 0) {

    if (strcmp(payload, MSG_UP) == 0) {
      Keyboard.releaseAll();
      //  Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('+');
      delay(50);
      Keyboard.releaseAll();
      //        Serial1.println(KEY_VOL_UP);
      //        Serial.println(KEY_VOL_UP);
      //        Keyboard.releaseAll();
      ////          delay(100);
      ////          Keyboard.press('+');
      ////          delay(100);
      //        Keyboard.releaseAll();
      LOG_PRINTFLN("VOL UP");



    } else if (strcmp(payload, MSG_DOWN) == 0) {
      Keyboard.releaseAll();
      //  Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('-');
      delay(50);
      Keyboard.releaseAll();
      //        Serial1.println(KEY_VOL_DOWN);
      //        Serial.println(KEY_VOL_DOWN);
      //        Keyboard.releaseAll();
      ////          delay(100);
      ////          Keyboard.press('-');
      ////          delay(100);
      //        Keyboard.releaseAll();
      LOG_PRINTFLN("VOL DOWN");



    } else if (strcmp(payload, MSG_MUTE) == 0) {
      Keyboard.releaseAll();
      //  Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('0');
      delay(50);
      Keyboard.releaseAll();
      //        Serial1.println(KEY_VOL_DOWN);
      //        Serial.println(KEY_VOL_DOWN);
      //        Keyboard.releaseAll();
      ////          delay(100);
      ////          Keyboard.press('-');
      ////          delay(100);
      //        Keyboard.releaseAll();
      LOG_PRINTFLN("VOL MUTED");
    }
  }
  /*  const char* buf = "Hello";
  MqttClient::Message message;
  message.qos = MqttClient::QOS2;
  message.retained = false;
  message.dup = false;
  message.payload = (void*) buf;
  message.payloadLen = strlen(buf);
  mqtt->publish(MQTT_TOPIC_PUB, message);
  */
}
// ============== Main loop ====================================================
void loop() {
  // Check connection status
  if (!mqtt->isConnected()) {
    // Close connection if exists
    network.stop();
    // Re-establish TCP connection with MQTT broker
    LOG_PRINTFLN("Connecting");
    network.connect(mqtt_server, 1883);
    if (!network.connected()) {
      LOG_PRINTFLN("Can't establish the TCP connection");
      delay(5000);
      ESP.reset();
    }
    // Start new MQTT connection
    MqttClient::ConnectResult connectResult;
    // Connect
    {
      MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
      options.MQTTVersion = 4;
      String clientId = MQTT_ID + WiFi.macAddress();
      options.clientID.cstring = (char*)clientId.c_str();
      options.cleansession = false;
      options.keepAliveInterval = 15;  // 15 seconds
      MqttClient::Error::type rc = mqtt->connect(options, connectResult);
      if (rc != MqttClient::Error::SUCCESS) {
        LOG_PRINTFLN("Connection error: %i", rc);
        return;
      }
    }
    {
      // Add subscribe here if required
      MqttClient::Error::type rc = mqtt->subscribe(
        TOPIC_PLAY, MqttClient::QOS2, processMessage);
      if (rc != MqttClient::Error::SUCCESS) {
        LOG_PRINTFLN("Subscribe error: %i", rc);
        LOG_PRINTFLN("Drop connection");
        mqtt->disconnect();
        return;
      }
      rc = mqtt->subscribe(
        TOPIC_VOL, MqttClient::QOS2, processMessage);
      if (rc != MqttClient::Error::SUCCESS) {
        LOG_PRINTFLN("Subscribe error: %i", rc);
        LOG_PRINTFLN("Drop connection");
        mqtt->disconnect();
        return;
      }
    }
  } else {
    {
      // Add publish here if required
    }
    // Idle for 3 miliseconds
    mqtt->yield(3L);
  }
}