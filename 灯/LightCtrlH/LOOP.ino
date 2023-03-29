unsigned long curTime = 0;
bool bPlaying = false;
bool bPaused = false;

unsigned long startTime=0;
unsigned long pausedTime=0;
unsigned long pauseStartTime=0;

void StartPlay()
{
  if (!bPlaying)
  {
    startTime = millis();
    digitalWrite (LigntRelay, HIGH);
    bPlaying = true;
    LOG_PRINTFLN("StartPlay");
  }
  else if (bPaused)
  {
    pausedTime += (millis()-pauseStartTime);
    bPaused = false;
    LOG_PRINTFLN("Restart play");
  }
  else
  {}
}
void PausePlay()
{
  if (bPlaying&&(!bPaused))
  {
    bPaused = true;
    pauseStartTime = millis();
    LOG_PRINTFLN("Pause play");
  }
}
void StopPlay()
{
  bPlaying = false;

  delay(500);
  digitalWrite (LigntRelay, LOW);
  LOG_PRINTFLN("StopPlay");

  startTime=0;
  pauseStartTime=0;
  pausedTime=0;
}

// ============== Subscription callback ========================================
void processMessage(MqttClient::MessageData& md) {
  const MqttClient::Message& msg = md.message;
  char payload[msg.payloadLen + 1];
  char topic[md.topicName.lenstring.len + 1];
  memcpy(payload, msg.payload, msg.payloadLen);
  memcpy(topic, md.topicName.lenstring.data, md.topicName.lenstring.len);
  payload[msg.payloadLen] = '\0';
  topic[md.topicName.lenstring.len] = '\0';
//  LOG_PRINTFLN(
//    "Message arrived: qos %d, retained %d, dup %d, packetid %d, payload:[%s]",
//    msg.qos, msg.retained, msg.dup, msg.id, payload
//  );
  
  if (strcmp(topic, TOPIC_PLAY) == 0)
  {
    if (strcmp(payload, MSG_PLAY) == 0)
    {
      StartPlay ();
    }
    else if (strcmp(payload, MSG_PAUS) == 0)
    {
      PausePlay();
    }
    else if (strcmp(payload, MSG_STOP) == 0)
    {
      StopPlay();
    }
  }
}
// ============== Main loop ====================================================
void loop() {
  if (bPlaying && (!bPaused))
  {
    curTime = millis();

    Serial.print("pausedTime:");
    Serial.println(pausedTime);
    Serial.print("curTime -startTime");
    Serial.println(curTime - startTime);
    if (curTime - startTime - pausedTime >= playTime)
    {
      StopPlay();
    }
  }
  // Check connection status
  if (!mqtt->isConnected()) {
    // Close connection if exists
    network.stop();
    // Re-establish TCP connection with MQTT broker
    LOG_PRINTFLN("Connecting");
    network.connect(mqtt_server, 1883);
    if (!network.connected()) {
      LOG_PRINTFLN("Can't establish the TCP connection");
      delay(5000);
      ESP.reset();
    }
    // Start new MQTT connection
    MqttClient::ConnectResult connectResult;
    // Connect
    {
      MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
      options.MQTTVersion = 4;
      String clientId = MQTT_ID + WiFi.macAddress();
      options.clientID.cstring = (char*)clientId.c_str();
      options.cleansession = false;
      options.keepAliveInterval = 15; // 15 seconds
      MqttClient::Error::type rc = mqtt->connect(options, connectResult);
      if (rc != MqttClient::Error::SUCCESS) {
        LOG_PRINTFLN("Connection error: %i", rc);
        return;
      }
    }
    {
      // Add subscribe here if required
      MqttClient::Error::type rc = mqtt->subscribe(
        TOPIC_PLAY, MqttClient::QOS2, processMessage
      );
      if (rc != MqttClient::Error::SUCCESS) {
        LOG_PRINTFLN("Subscribe error: %i", rc);
        LOG_PRINTFLN("Drop connection");
        mqtt->disconnect();
        return;
      }
    }
  } else {
    {
      // Add publish here if required
    }
    // Idle for 3 miliseconds
    mqtt->yield(3L);
  }
}
