void setup_wifi() {
  delay(10);
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
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = CLIENT_ID;
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      //client.subscribe(TOPIC_COMP);
      client.subscribe(TOPIC_PLAY);
      client.subscribe(TOPIC_VOL);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);//8266板子自带串口，监视窗可以看到信息
  Serial1.begin(9600);//通往leonardo的 D4针脚
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}
