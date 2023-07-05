

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi和服务器信息
const char* ssid = "NewImagery";
const char* password = "*#01234567890";
const char* mqtt_server = "iServer";
const int mqtt_port = 1883;

const char* TopicIn = "shanxijiandangin";
const char* TopicOut = "shanxijiandangout";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}



// 连接WiFi网络
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial1.println();
  Serial1.print("Connecting to ");
  Serial1.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial1.print(".");
  }

  randomSeed(micros());

  Serial1.println("");
  Serial1.println("WiFi connected");
  Serial1.println("IP address: ");
  Serial1.println(WiFi.localIP());
}

// 连接MQTT服务器
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial1.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial1.println("connected");
      // Once connected, publish an announcement...
      client.publish(TopicOut, "hello world");
      // ... and resubscribe
      client.subscribe(TopicIn);
    } else {
      Serial1.print("failed, rc=");
      Serial1.print(client.state());
      Serial1.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  /*
  client.loop();
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial1.print("Publish message: ");
    Serial1.println(msg);
    client.publish(TopicOut, msg);
  }
  */
}

// 处理MQTT消息
void callback(char* topic, byte* payload, unsigned int length) {
  Serial1.print("Message received: ");
  for (int i = 0; i < length; i++) {
    Serial1.print((char)payload[i]);
  }
  Serial1.println();


  if (strcmp(topic, TopicIn) == 0) {
    if (strncmp((char*)payload, "play", length) == 0) {
      Serial.write("FJDPLAY");  //播放板手册
    } else if (strncmp((char*)payload, "stop", length) == 0) {
      Serial.write("FJDSTOP");  //播放板手册
    } else if (strncmp((char*)payload, "pause", length) == 0) {
      Serial.write("FJDPAUSE");  //播放板手册
    } else if (strncmp((char*)payload, "+", length) == 0) {
      Serial.write("FJDVUP");
    } else if (strncmp((char*)payload, "-", length) == 0) {
      Serial.write("FJDVDN");
    } else if (strncmp((char*)payload, "mute", length) == 0) {
      Serial.write("FJDMUTE");  //播放板手册
    }
    if (Serial.find("+end")) {
      client.publish(TopicOut, "end");
    }
  }
}
