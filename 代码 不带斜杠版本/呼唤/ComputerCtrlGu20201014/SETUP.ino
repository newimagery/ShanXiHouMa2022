void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // String clientId = "ESP8266Client-";
    // clientId += String(random(0xffff), HEX);
    String clientId = CLIENT_ID;
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      //... and resubscribe
      client.subscribe(TOPIC_COMP);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ComputerRelay, OUTPUT);
  // Initialize the BUILTIN_LED pin as an output
  // digitalWrite (ComputerRelay, LOW);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  /*
    client.publish(TOPIC_COMP, "0");
    client.publish(TOPIC_PROJ, "0");
    client.publish(TOPIC_PLAY, "0");
    client.publish(TOPIC_VOL, "0");
  */
}
