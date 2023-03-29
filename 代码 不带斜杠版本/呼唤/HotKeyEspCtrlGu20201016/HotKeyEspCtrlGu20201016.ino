/*
 * 在头文件中增加了clientID的编辑
 * 20210415
 * 薛超
 * ===========
  颠倒了up、down
  注释掉了8号按键
  20201016
  薛涛
  ===
  去掉了回写0
  20201015
  薛涛
  ====
  整理代码格式
  薛涛
  20201010
  ========
  热键控制器Esp8266端
  文件：HotKeyEspCtrl：程序的自定义内容
       LOOP：程序的主要工作
       SETUP：程序的初始化
  topic:  Computer  Projector  Playing  Volume
  初始化：给所有topic写0
  输入：根据按键输出topic及msg：
          Computer      Projector      Playing            Volume
         click  off      on  off     play  pause stop       + -
           2     8        1   9        3     4     5        6 7
  输出：根据收到的信息通过串口发给Leonardo板,再给topic写0
  创建人：薛超
  创建日期：2020.8.23
  =========================================
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "zhongkong";
const char* password = "1234567890";
const char* mqtt_server = "192.168.1.201";

const char* TOPIC_COMP = "ComputerH";
const char* TOPIC_PROJ = "ProjectorH";
const char* TOPIC_PLAY = "PlayingH";
const char* TOPIC_VOL  = "VolumeH";
const char* CLIENT_ID = "ESP8266Client-HotKeyH";

const char* MSG_CLIK   = "click";
const char* MSG_OFF    = "off";
const char* MSG_ON     = "on";
const char* MSG_PLAY   = "play";
const char* MSG_PAUS   = "pause";
const char* MSG_STOP   = "stop";
const char* MSG_UP     = "+";
const char* MSG_DOWN   = "-";

 //#define KEY_COMP_OFF     8
#define KEY_PLAY_PLAY    3
#define KEY_PLAY_PAUS    4
#define KEY_PLAY_STOP    5
#define KEY_VOL_DOWN     6
#define KEY_VOL_UP       7

WiFiClient espClient;
PubSubClient client(espClient);
