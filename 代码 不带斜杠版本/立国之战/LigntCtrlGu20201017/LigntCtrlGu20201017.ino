/*
 * 在头文件中增加了clientID的编辑
 * 20210415
 * 薛超
 * ===========   
   修正了播放器间重按播放键会重新计时的问题
   20201017
   顾清辉
   ========
   Serial.begin 改回 115200
   去掉取用代码
   20201015
   薛涛
   =======
   Serial.begin 改为 9600
   pubsubclient 库版本为2.7
   薛涛
   20201014
   =======
   整理了代码格式
   薛涛
   20201010
   =======
   照明灯控制器
   文件：LigntCtrl：程序的自定义内容
        LOOP：程序的主要工作
        SETUP：程序的初始化
   topic:  Computer  Projector  Playing  Volume
   初始化：给所有topic写0
   输入：topic:Playing msg:play   do: 继电器断开，开始计时，直到计时结束
        topic:Playing msg:pause  do: 暂停计时
        topic:Playing msg:stop   do: 计时结束，继电器吸合
   输出：无
   创建人：薛超
   创建日期：2020.8.23
   =========================================
   修改人：
   修改日期：
   修改内容：
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LigntRelay  4

const char* ssid = "zhongkong";
const char* password = "1234567890";
const char* mqtt_server = "192.168.1.201";
const char* TOPIC_PLAY = "PlayingL";
const char* CLIENT_ID = "ESP8266Client-LightL";

const char* MSG_PLAY   = "play";
const char* MSG_PAUS   = "pause";
const char* MSG_STOP   = "stop";


unsigned long playTime = 227000;//ms

WiFiClient espClient;
PubSubClient client(espClient);
