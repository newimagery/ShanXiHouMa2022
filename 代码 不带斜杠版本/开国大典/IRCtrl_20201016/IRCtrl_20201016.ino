/*
 * 在头文件中增加了clientID的编辑
 * 20210415
 * 薛超
 * ===========
   注释掉回写“0”
   20201017
   薛涛
   ====
   另存了文件名
   整理了代码格式
   薛涛2020.10.10
   =========
   修改日志
   clientID改成了“ESP8266Client-Projector-IR”
   薛涛 2020.10.08
   =========
   红外控制器
   文件：IRCtrl：程序的自定义内容
        LOOP：程序的主要工作
        SETUP：程序的初始化
   topic:  Computer  Projector  Playing  Volume
   初始化：给所有topic写0
   输入：topic:Projector msg:on   do: 红外控制开机
        topic:Projector msg:off  do: 红外控制关机
   输出：无
   创建人：薛超
   创建日期：2020.8.23
   =========================================
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.
uint16_t rawData[115] = {3586, 1708,  488, 388,  488, 1266,  486, 392,  490, 394,
                         488, 398,  486, 398,  488, 400,  488, 404,  488, 388,
                         488, 390,  486, 392,  490, 394,  490, 396,  488, 1272,
                         488, 400,  490, 402,  488, 388,  488, 392,  490, 390,
                         460, 422,  488, 400,  488, 398,  488, 400,  486, 1280,
                         488, 388,  488, 392,  488, 392,  488, 1268,  488, 398,
                         488, 396,  490, 1274,  486, 404,  490, 388,  490, 390,
                         488, 392,  488, 396,  488, 398,  488, 398,  488, 402,
                         488, 404,  488, 1264,  488, 390,  490, 1266,  488, 1270,
                         488, 1272,  490, 1272,  488, 400,  490, 402,  490, 1262,
                         488, 392,  492, 1260,  494, 390,  490, 1272,  490, 1270,
                         488, 1276,  490, 1278,  486
                        };  // UNKNOWN 2899206F

const char* ssid = "zhongkong";
const char* password = "1234567890";
const char* mqtt_server = "192.168.1.201";

const char* TOPIC_COMP = "ComputerK";
const char* TOPIC_PROJ = "ProjectorK";
const char* TOPIC_PLAY = "PlayingK";
const char* TOPIC_VOL  = "VolumeK";
const char* CLIENT_ID = "ESP8266Client-Projector-IRK";

const char* MSG_OFF    = "off";
const char* MSG_ON     = "on";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = CLIENT_ID;
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(TOPIC_PROJ);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void sendCode() {
  irsend.sendRaw(rawData, 115, 38);  // Send a raw data capture at 38kHz.
  Serial.print("Sent a raw data capture at 38kHz");
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  String topicStr = topic;
  char code[10];
  if (topicStr == TOPIC_PROJ)
  {
    if (length < 9)
    {
      memcpy (code, payload, length);
      if (strstr(code, MSG_ON) != NULL)
      {
        sendCode();
        //client.publish(TOPIC_PROJ, "0");
      }
      else if (strstr(code, MSG_OFF) != NULL)
      {
        sendCode();
        delay(2000);
        sendCode();
        //client.publish(TOPIC_PROJ, "0");
      }
    }
  }
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
