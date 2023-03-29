void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
}
void loop()
{
  if (Serial.available() > 0)
  {
    int  key = Serial.read();
    Serial.println(key);
    switch (key)
    {
      case KEY1:   client.publish(TOPIC_PROJ, MSG_ON, 0);      break;
      case KEY2:   client.publish(TOPIC_COMP, MSG_CLIK, 0);    break;
      case KEY4:   client.publish(TOPIC_PLAY, MSG_PLAY, 0);    break;
      case KEY5:   client.publish(TOPIC_PLAY, MSG_PAUS, 0);    break;
      case KEY6:   client.publish(TOPIC_PLAY, MSG_STOP, 0);    break;
      case KEY7:   client.publish(TOPIC_VOL, MSG_DOWN, 0);     break;
      case KEY8:   client.publish(TOPIC_VOL, MSG_UP, 0);     break;
      default:    break;
    }
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
