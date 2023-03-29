/*
   整理了格式
   注释掉了8号键
   F9 F10 改为 点击两次
   20201016
   薛涛
   =====
   颠倒了6、7的up、down
   整理代码格式
   薛涛
   20201010
   ==========
   热键控制器Leonardo端
   文件：HotKeyLeoCtrl：程序的自定义内容
        LOOP：程序的主要工作
        SETUP：程序的初始化
   topic:  Computer  Projector  Playing  Volume
   初始化：给所有topic写0
   输入：根据按键输出topic及msg：
          Computer      Projector      Playing            Volume
         click  off      on  off     play  pause stop       + -
           2     8        1   9        3     4     5        6 7
   输出：根据收到的信息模拟按键
   创建人：薛超
   创建日期：2020.8.23
   =========================================
*/
#include <Keyboard.h>

//#define KEY_COMP_OFF     '8'

#define KEY_PLAY_PLAY    '3'
#define KEY_PLAY_PAUSE   '4'
#define KEY_PLAY_STOP    '5'
#define KEY_VOL_UP       '6'
#define KEY_VOL_DOWN     '7'
