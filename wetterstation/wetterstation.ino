#include <ArduinoJson.h>

#include <BH1750.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <DHT.h>

#include <string>
#include <iostream>
#include <sstream>
#include <WiFi.h>
#include <PubSubClient.h>

#define DHT_SENSOR_PIN 23
#define DHT_SENSOR_TYPE DHT11

#define SOUND_SENSOR 36

const char *ssid = "WLANrouterRPPT";
const char *password = "bbsbrinkstrasse";
const char *mqtt_server = "192.168.1.2";

const char *outHum = "jannik/wetterstation/humidity";
const char *outTemp = "jannik/wetterstation/temperature";
const char *outLight = "jannik/wetterstation/lightlevel";
const char *outSound = "jannik/wetterstation/volume";

BH1750 lightmeter;
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  // if ((char)payload[0] == '1') {
  //   digitalWrite(LED, HIGH);   // Turn the LED on (Note that LOW is the voltage level
  //   client.publish(outTopic, "LED ON");
  // } else {
  //   digitalWrite(LED, LOW);  // Turn the LED off by making the voltage HIGH
  //   client.publish(outTopic, "LED OFF");
  // }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(outMoisture, "connected");
      // ... and resubscribe
      //client.subscribe(inMoisture);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  dht_sensor.begin();

  Wire.begin();
  lightmeter.begin();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  // read humidity
  float humi  = dht_sensor.readHumidity();
  // read temperature in Celsius
  float tempC = dht_sensor.readTemperature();

  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.println("%");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println("°C");
  }

  // Read light level
  float lux = lightmeter.readLightLevel();
  Serial.print("Lightlevel: ");
  Serial.println(lux);

  // Read sound level
  int sound = analogRead(SOUND_SENSOR);
  Serial.print("Volume: ");
  Serial.println(sound);

  // DynamicJsonDocument doc(1024);
  // doc["temperature"] = tempC;
  // doc["humidity"] = humi;
  // doc["lux"] = lux;
  // doc["volume"] = sound;
  // String jsonString;
  // serializeJson(doc, jsonString);

  client.publish(outHum,  std::to_string(humi).c_str());
  client.publish(outTemp,  std::to_string(tempC).c_str());
  client.publish(outLight,  std::to_string(lux).c_str());
  client.publish(outSound,  std::to_string(sound).c_str());

  //wait 2 seconds between readings
  delay(2000);
}
