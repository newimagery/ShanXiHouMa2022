/*
 * 在头文件中增加了clientID的编辑
 * 20210415
 * 薛超
 * ===========
  修正了莫名其妙的串口打印一大堆 和 重新连接wifi的问题
  去掉了回写0
  注释了D2
  20201014
  薛涛 顾清辉
  =======
   电脑开关机控制
   文件：ComputerCtrl：程序的自定义内容
        LOOP：程序的主要工作
        SETUP：程序的初始化
   topic:  Computer  Projector  Playing  Volume
   初始化：给所有topic写0
   输入：topic:Computer msg:click   do: 继电器断触0.5秒
        topic:Computer msg:pause   do: 暂停计时

   输出：topic:Computer msg:0 when:断触执行完后回写0
   创建人：薛超
   创建日期：2020.8.23
   =========================================
   修改人：
   修改日期：
   修改内容：
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define ComputerRelay  4 //D2
const char* ssid = "zhongkong";
const char* password = "1234567890";
const char* mqtt_server = "192.168.1.201";

const char* TOPIC_COMP = "ComputerA";
const char* TOPIC_PROJ = "ProjectorA";
const char* TOPIC_PLAY = "PlayingA";
const char* TOPIC_VOL  = "VolumeA";
const char* CLIENT_ID = "ESP8266Client-ComputerA";

const char* MSG_CLIK   = "click";
const char* MSG_OFF    = "off";



WiFiClient espClient;
PubSubClient client(espClient);
