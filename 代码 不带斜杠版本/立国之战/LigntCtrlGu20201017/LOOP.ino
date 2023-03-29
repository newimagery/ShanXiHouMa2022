unsigned long playedTime = 0;
unsigned long lastTime = 0;
unsigned long curTime = 0;
bool bPlaying = false;
bool bPaused = false;

void StartPlay()
{
  
  if (!bPlaying)
  {
    playedTime = 0;
    lastTime = millis();
    digitalWrite (LigntRelay, HIGH);
    bPlaying = true;
    Serial.println("StartPlay");
  }
  else
  {
    bPaused = false;
  }
}
void PausePlay()
{
  if (bPlaying)
  {
    bPaused = true;
    delay (100);

    playedTime = curTime - lastTime;
    Serial.println("PausePlay");
  }
}
void StopPlay()
{
  bPlaying = false;

  delay(500);
  digitalWrite (LigntRelay, LOW);
  Serial.println("StopPlay");

  playedTime = 0;
  lastTime = 0;
}

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
        StartPlay();
      }

      else if (strstr(code, MSG_PAUS) != NULL)
      {
        PausePlay();
      }

      else if (strstr(code, MSG_STOP) != NULL)
      {
        StopPlay();
      }
    }
  }
}

void loop()
{
  if (bPlaying && !bPaused)
  {
    curTime = millis();

    Serial.print("playedTime:");
    Serial.println(playedTime);
    Serial.print("curTime -lastTime");
    Serial.println(curTime - lastTime);
    if (playedTime +  curTime - lastTime >= playTime)
    {
      StopPlay();
    }
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
