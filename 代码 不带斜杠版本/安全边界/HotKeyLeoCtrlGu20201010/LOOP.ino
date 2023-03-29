int key = 0;
void loop()
{
  if (Serial1.available() > 0)
  {
    key = Serial1.read();
    Serial.println(key);
    switch (key)
    {
      /*
        case KEY_COMP_OFF:
        // Keyboard.press(KEY_F1);
        Keyboard.press('8');
        delay(50);
        Keyboard.releaseAll();
        break;
      */
      case KEY_PLAY_PLAY:
        Keyboard.press(' ');
        delay(50);
        Keyboard.releaseAll();
        break;
      case KEY_PLAY_PAUSE:
        Keyboard.press(KEY_LEFT_CTRL);
        delay(50);
        Keyboard.press('p');    //向电脑发送向暂停的指令
        delay(50);
        Keyboard.releaseAll();
        break;
      case KEY_PLAY_STOP:
        Keyboard.press(KEY_LEFT_CTRL);
        delay(50);
        Keyboard.press('s');    //向电脑发送向停止的指令
        delay(50);
        Keyboard.releaseAll();
        break;
      case KEY_VOL_UP:
        Keyboard.press(KEY_F9);
        delay(50);
        Keyboard.releaseAll();
        Keyboard.press(KEY_F9);
        delay(50);
        Keyboard.releaseAll();
        break;
      case KEY_VOL_DOWN:
        Keyboard.press(KEY_F10);
        delay(50);
        Keyboard.releaseAll();
        Keyboard.press(KEY_F10);
        delay(50);
        Keyboard.releaseAll();
        break;
      default:
        break;
    }
    key = 0;
  }
}
