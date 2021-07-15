#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

Servo servo;

const char *user = "";
const char *pass = "";
const char *broker = "192.168.1.11";

const char *legA = "LEG/A";

WiFiClient espClient;
PubSubClient client(espClient);

long currentTime, lastTime;
int count = 0;
char message[50];

void setupWifi()
{
  Serial.println("Connection \n");
  WiFi.mode(WIFI_STA);
  WiFi.begin("Kreatifindo", "Kreatifindo666");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("\nConnecting to");
    Serial.println(broker);
    if (client.connect("esp8266", user, pass))
    {
      Serial.print("\nConnected to ");
      Serial.println(broker);
      client.subscribe(legA);
    }
    else
    {
      Serial.println("\nTrying connect again..");
      delay(5000);
    }
  }
}

String byteToString(byte data[], unsigned int len)
{
    int a = (int) len;
    char result[a];
    for(int i = 0; i < a;i++){
      result[i] = (char) data[i];
    }
    result[a] = '\0'; // make last element null

    return result;
}

byte received_payload[128];
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Received messages: ");
  Serial.println(topic);
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  String topicStr(topic);
  if (topicStr == legA)
  {
    Serial.println("move leg A ");
    if ((char)payload[0] == '1')
    {
      servo.write(180);
      delay(500);
      servo.write(0);
    }
  }

  memcpy(received_payload, payload, length);
  
  String cmd = byteToString(payload, length);
  Serial.println(cmd);

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
}