
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
  
  if (strcmp(topic, TOPIC_COMP) == 0)
  {
    if (strcmp(payload, MSG_ON) == 0)
    {
      digitalWrite (ComputerRelay, HIGH);
//      delay(500);
//      digitalWrite (ComputerRelay, LOW);
      LOG_PRINTFLN("Computer on");
    }
    else  if (strcmp(payload, MSG_OFF) == 0)
    {
//      digitalWrite (ComputerRelay, HIGH);
//      delay(500);
      digitalWrite (ComputerRelay, LOW);
      LOG_PRINTFLN("Computer off");
    }
  }
/*  const char* buf = "Hello";
  MqttClient::Message message;
  message.qos = MqttClient::QOS2;
  message.retained = false;
  message.dup = false;
  message.payload = (void*) buf;
  message.payloadLen = strlen(buf);
  mqtt->publish(MQTT_TOPIC_PUB, message);
  */
}
// ============== Main loop ====================================================
void loop() {
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
        TOPIC_COMP, MqttClient::QOS2, processMessage
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
