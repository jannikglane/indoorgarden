//include library
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <BH1750.h>

// initialize BH1750 object
BH1750 GY302;

//configuring dht sensor
#define PIN 2
#define TYPE DHT11
DHT dht(PIN, TYPE);

//define min and max values
float maxTemperature = 18;
float maxHumidity = 50;
float minSoilMoisturepercent = 40;
float minLightLevel = 40;

//define pins
const int ventilator = 3;
const int ledmatrix = 4;
const int input = A0;

//define values for soil moisture
const float AirValue = 620;   
const float WaterValue = 310;
float soilMoistureValue = 0;
float soilmoisturepercent = 0;

int light = 0;

void setup()
{
  //start dht library for temperature sensor
  dht.begin();
  // start gy302 library for light sensor
  GY302.begin();

  //set pin mode
  pinMode(ventilator, OUTPUT);
  pinMode(ledmatrix, OUTPUT);
  pinMode(input, INPUT);
  
  Serial.begin(9600);
}
void processTemperatureAndHumidity()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("temperature");
  Serial.println(temperature);
  Serial.print("humidity");
  Serial.println(humidity);

  if ((temperature > maxTemperature) || (humidity > maxHumidity))
  {
    digitalWrite(ventilator, LOW);
  }
  else
  {
    digitalWrite(ventilator, HIGH);
  }
}

 

void processsoilMoisture()
{
  soilMoistureValue = analogRead(input); // put Sensor insert into soil
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

  if (soilmoisturepercent < minSoilMoisturepercent)
  {
    digitalWrite(ledmatrix, HIGH);
  }
  else
  {
    digitalWrite(ledmatrix, LOW);
  }
}

void processLight()
{
  // get reading from gy302 light sensor
  uint16_t lux = GY302.readLightLevel();

  if (lux < minLightLevel)
  {
    digitalWrite(ledmatrix, HIGH);
  }
  else
  {
    digitalWrite(ledmatrix, LOW);
  }
}

void loop()
{
  processTemperatureAndHumidity();
  processsoilMoisture();
  processLight();
  }
