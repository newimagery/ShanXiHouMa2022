//char sendBuf[20],sendBufoff[22];

String comdata = "";
char buf[20];
String topic;
bool bErr = true;
// ============== Subscription callback ========================================
void processMessage(MqttClient::MessageData& md) {
  const MqttClient::Message& msg = md.message;
  char payload[msg.payloadLen + 1];
  char topic[md.topicName.lenstring.len + 1];
  memcpy(payload, msg.payload, msg.payloadLen);
  memcpy(topic, md.topicName.lenstring.data, md.topicName.lenstring.len);
  payload[msg.payloadLen] = '\0';
  topic[md.topicName.lenstring.len] = '\0';
  // LOG_PRINTFLN(
  // "Message arrived: qos %d, retained %d, dup %d, packetid %d, payload:[%s]",
  // msg.qos, msg.retained, msg.dup, msg.id, payload
  // );
  // memset (sendBuf, 0, sizeof(char)*20);

  const char* sendplay = "FJDPLAY";
  const char* sendpause = "FJDPAUSE";
  const char* sendstop = "FJDSTOP";
  const char* senddown = "FJDVDN";
  const char* sendup = "FJDVUP";
  const char* sendmtue = "FJDMUTE";

  //FJDMUTE 静音
  //FJDVDN 音量减
  //FJDVUP 音量加
  //FJDPLAY 播放
  //FJDPAUSE 暂停
  //FJDSTOP 停止并进入黑屏或者待机图片


  if (strcmp(topic, TOPIC_PLAY) == 0) {
    if (strcmp(payload, MSG_PLAY) == 0) {
      //        sendBuf[8] = 'N';
      //        Serial1.print(sendBuf);
      Serial1.print(sendplay);
      delay(1000);
      Serial.println(sendplay);
      //        LOG_PRINTFLN ("MSG_ON");
    } else if (strcmp(payload, MSG_PAUS) == 0) {
      //        sendBuf[8] = 'F';
      //        Serial1.print(sendBufoff);
      Serial1.print(sendpause);
      delay(1000);
      Serial.println(sendpause);
      LOG_PRINTFLN("MSG_sendpause");
    } else if (strcmp(payload, MSG_STOP) == 0) {
      //        sendBuf[8] = 'F';
      //        Serial1.print(sendBufoff);
      Serial1.print(sendstop);
      delay(1000);
      //        Serial.println(sendBufoff);
      Serial.println(sendstop);
      LOG_PRINTFLN("MSG_sendstop");
    }
  } else if (strcmp(topic, TOPIC_VOL) == 0) {
    if (strcmp(payload, MSG_UP) == 0) {
      //        sendBuf[8] = 'F';
      //        Serial1.print(sendBufoff);
      Serial1.print(sendup);
      delay(1000);
      //        Serial.println(sendBufoff);
      Serial.println(sendup);
      LOG_PRINTFLN("MSG_sendup");
    } else if (strcmp(payload, MSG_DOWN) == 0) {
      //        sendBuf[8] = 'F';
      //        Serial1.print(sendBufoff);
      Serial1.print(senddown);
      delay(1000);
      //        Serial.println(sendBufoff);
      Serial.println(senddown);
      LOG_PRINTFLN("MSG_senddown");
    } else if (strcmp(payload, MSG_MUTE) == 0) {
      //        sendBuf[8] = 'F';
      //        Serial1.print(sendBufoff);
      Serial1.print(sendmtue);
      delay(1000);
      //        Serial.println(sendBufoff);
      Serial.println(sendmtue);
      LOG_PRINTFLN("MSG_sendmtue");
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

  //  if(Serial.available())

  //  {
  //    Serial.println("==================================");
  //    Serial.println(Serial.read());

  //    }
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
      options.keepAliveInterval = 15;  // 15 seconds
      MqttClient::Error::type rc = mqtt->connect(options, connectResult);
      if (rc != MqttClient::Error::SUCCESS) {
        LOG_PRINTFLN("Connection error: %i", rc);
        return;
      }
    }
    {
      // Add subscribe here if required
      MqttClient::Error::type rc = mqtt->subscribe(
        TOPIC_PLAY, MqttClient::QOS2, processMessage);
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

      memset(buf, 0, sizeof(char) * 20);
      //      int i=0;
      while (Serial.available() > 0) {
        comdata += char(Serial.read());
        //        i++;
        delay(2);
      }
      Serial1.print(comdata);
      delay(100);
      Serial.println(comdata);
      if (comdata.length() > 0) {
        //        if (strcmp(comdata, "++end") == 0)
        if (comdata.compareTo("++end") == 0) {
          LOG_PRINTFLN("!!!!!");
          topic = TOPIC_PLAY;
          strcpy(buf, MSG_END);
          bErr = false;
        }
        comdata = "";
      }



      MqttClient::Message message;
      message.qos = MqttClient::QOS2;
      message.retained = false;
      message.dup = false;
      message.payload = (void*)buf;
      message.payloadLen = strlen(buf);
      if (!bErr) {
        mqtt->publish(topic.c_str(), message);
        bErr = true;
      } else {
        //bErr=false;
      }
    }
    // Idle for 3 miliseconds
    mqtt->yield(3L);
  }
}