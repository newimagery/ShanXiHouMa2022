void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  String topicStr = topic;
  char code[10];
  if (topicStr == TOPIC_PLAY)
  {
    if (length < 9)
    {
      memcpy (code, payload, length);
      if (strstr(code, MSG_PLAY) != NULL)
      {
        Serial1.println(KEY_PLAY_PLAY);
        Serial.println(KEY_PLAY_PLAY);
        //client.publish(TOPIC_PLAY, "0");
        Serial.println("MSG_PLAY");
      }
      else if (strstr(code, MSG_PAUS) != NULL)
      {
        Serial1.println(KEY_PLAY_PAUS);
        Serial.println(KEY_PLAY_PAUS);
        //client.publish(TOPIC_PLAY, "0");
        Serial.println("MSG_PUSE");
      }
      else if (strstr(code, MSG_STOP) != NULL)
      {
        Serial1.println(KEY_PLAY_STOP);
        Serial.println(KEY_PLAY_STOP);
        //client.publish(TOPIC_PLAY, "0");
        Serial.println("MSG_STOP");
      }
    }
  }
  
  /*
  else if (topicStr == TOPIC_COMP)
  {
    if (length < 9)
    {
      memcpy (code, payload, length);
      if (strstr(code, MSG_OFF) != NULL)
      {
        Serial1.println(KEY_COMP_OFF);
        Serial.println(KEY_COMP_OFF);
        //client.publish(TOPIC_COMP, "0");
      }
    }
  }
  */
  
  else if (topicStr == TOPIC_VOL)
  {
    if (length < 9)
    {
      memset(code, 0, 10);
      memcpy (code, payload, length);
      if (strstr(code, MSG_UP) != NULL)
      {
        Serial1.println(KEY_VOL_UP);
        Serial.println(KEY_VOL_UP);
        //client.publish(TOPIC_VOL, "0");
      }
      else if (strstr(code, MSG_DOWN) != NULL)
      {
        Serial1.println(KEY_VOL_DOWN);
        Serial.println(KEY_VOL_DOWN);
        //client.publish(TOPIC_VOL, "0");
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
