void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String topicStr = topic;
  char code[10];
  if (topicStr == TOPIC_COMP)
  {
    if (length < 9)
    {
      memcpy (code, payload, length);
      if (strcmp(code, MSG_CLIK) == 0)
      {
        digitalWrite (ComputerRelay, HIGH);
        delay(500);
        digitalWrite (ComputerRelay, LOW);
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
