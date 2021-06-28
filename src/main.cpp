#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

Servo servo;

const char* user = "";
const char* pass = "";
const char* broker = "192.168.1.8";

const char* outTopic = "control/out";
const char* inTopic = "control/in";

WiFiClient espClient;
PubSubClient client(espClient);

long currentTime, lastTime;
int count = 0;
char message[50];

void setupWifi() {
  Serial.println("Connection \n");
  WiFi.mode(WIFI_STA);
  WiFi.begin("HAKIM", "ELSHIRAZI");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while(!client.connected()){
    Serial.print("\nConnecting to");
    Serial.println(broker);
    if (client.connect("esp8266", user, pass))
    {
      Serial.print("\nConnected to ");
      Serial.println(broker);
      client.subscribe(inTopic);
    }else{
      Serial.println("\nTrying connect again..");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received messages: ");
  Serial.println(topic);
  for (int i = 0; i < length; i++)
  {
    Serial.print((char) payload[i]);
  }
  Serial.println();  
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(5);
  servo.write(0);

  setupWifi();
  client.setServer(broker, 1883);
  client.setCallback(callback);
}

void loop()
{
  // put your main code here, to run repeatedly:

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  
  // currentTime = millis();
  // if (currentTime - lastTime > 2000)
  // {
  //   count++;
  //   snprintf(message, 75, "Count: %ld", count);
  //   Serial.print("Sending message: ");
  //   Serial.println(message);
  //   client.publish(outTopic, message);
  //   lastTime = millis();
  // }
  

  // servo.write(90);
  // delay(1000);
  // servo.write(10);
  // delay(5000);
}