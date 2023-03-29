/*
 * 在头文件中增加了clientID的编辑
 * 20210415
 * 薛超
 * ===========
   需要把setup里的clientID 用一个函数 放到主程序中 方便以后更改ID
   20201015
   薛涛
   ===
   金属键盘波特率必须是9600
   20201015
   薛涛
   ====
   使用pubsubclient库版本是2.7
   使用其他版本的库，有可能会导致编译错误
   比如"invalid conversion from 'const char*' to 'const uint8_t*"
   20201014
   xuetao
   ======
   整理代码格式
   薛涛
   20201012
   ======
   更改了代码格式
   薛涛
   20201010
   =======================
   更改了retained
   retained 设置为0
   20201008
   顾清辉
   20201008
   ========================
   面板控制器
   文件：KeyboardCtrl：程序的自定义内容
        LOOP：程序的主要工作
        SETUP：程序的初始化
   topic:  Computer  Projector  Playing  Volume
   初始化：给所有topic写0
   输入：无
   输出：根据按键输出topic及msg：
          Computer      Projector      Playing            Volume
         click  off      on  off     play  pause stop       + -
           2     8        1   9        3     4     5        6 7

   创建人：薛超
   创建日期：2020.8.23
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "zhongkong";
const char* password = "1234567890";
const char* mqtt_server = "192.168.1.201";

const char* TOPIC_COMP = "ComputerY";
const char* TOPIC_PROJ = "ProjectorY";
const char* TOPIC_PLAY = "PlayingY";
const char* TOPIC_VOL  = "VolumeY";
const char* CLIENT_ID = "ESP8266Client-KeyboardY";

const char* MSG_CLIK   = "click";
const char* MSG_OFF    = "off";
const char* MSG_ON     = "on";
const char* MSG_PLAY   = "play";
const char* MSG_PAUS   = "pause";
const char* MSG_STOP   = "stop";
const char* MSG_UP     = "+";
const char* MSG_DOWN   = "-";

#define KEY0     0X30
#define KEY1     0X31
#define KEY2     0X32
#define KEY3     0X33
#define KEY4     0X34
#define KEY5     0X35
#define KEY6     0X36
#define KEY7     0X37
#define KEY8     0X38
#define KEY9     0X39

WiFiClient espClient;
PubSubClient client(espClient);
