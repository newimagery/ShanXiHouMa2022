

/*  const char* buf = "Hello";
  MqttClient::Message message;
  message.qos = MqttClient::QOS2;
  message.retained = false;
  message.dup = false;
  message.payload = (void*) buf;
  message.payloadLen = strlen(buf);
  mqtt->publish(MQTT_TOPIC_PUB, message);
  */
char buf[20];// = "Hello";
String topic;
bool bErr=true;
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
    }
  } else {
    {
      // Add publish here if required
      
      memset (buf, 0, sizeof(char)*20);
      if (Serial.available() > 0)
      {
        LOG_PRINTFLN("!!!!!");
        int  key = Serial.read();
        Serial.println(key);
        switch (key)
        {
          case KEY1:   topic = TOPIC_PROJ; strcpy(buf, MSG_ON);    bErr = false;  break;  
          case KEY4:   topic = TOPIC_COMP; strcpy(buf, MSG_ON);    bErr = false;  break;
          case KEY6:   topic = TOPIC_COMP; strcpy(buf, MSG_OFF);  bErr = false;  break;
          case KEY3:   topic = TOPIC_PROJ; strcpy(buf, MSG_OFF);    bErr = false;  break;
          case KEY7:   topic = TOPIC_PLAY; strcpy(buf, MSG_PLAY);  bErr = false;  break;
          case KEY8:   topic = TOPIC_PLAY; strcpy(buf, MSG_PAUS);  bErr = false;  break;
          case KEY9:   topic = TOPIC_PLAY; strcpy(buf, MSG_STOP);  bErr = false;  break;
          case KEY0:   topic = TOPIC_VOL; strcpy(buf, MSG_DOWN);   bErr = false;  break;
          case KEYb:   topic = TOPIC_VOL; strcpy(buf, MSG_UP);     bErr = false;  break;
          case KEYa:   topic = TOPIC_VOL; strcpy(buf, MSG_MUTE);    bErr = false;  break; 

          default:     bErr = true; break;
        }
      }
      
      MqttClient::Message message;
      message.qos = MqttClient::QOS2;
      message.retained = false;
      message.dup = false;
      message.payload = (void*) buf;
      message.payloadLen = strlen(buf);
      if (!bErr)
      {
        mqtt->publish(topic.c_str(), message);
        bErr = true;
      }
      else 
      {
        //bErr=false;
      }   
    }
    // Idle for 3 miliseconds
    mqtt->yield(3L);
  }
}
