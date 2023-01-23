// include libraries
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <BH1750.h>

// initialize BH1750 object
BH1750 GY302;

//initialize DHT object
#define PIN 2
#define TYPE DHT11
DHT dht(PIN, TYPE);

// define min and max values
float maxTemperature = 18;
float maxHumidity = 50;
float minSoilMoisturepercent = 40;
float minLightLevel = 40;

// define pins
const int ventilator = 3;
const int ledmatrix = 4;
const int waterpump = 6;
const int soilmoistureSensor = A0;

// define values for soil moisture
const float AirValue = 620;
const float WaterValue = 310;
float soilMoistureValue = 0;
float soilmoisturepercent = 0;

void setup()
{
  // start dht library for temperature sensor
  dht.begin();

  // start gy302 library for light sensor
  GY302.begin();

  // set pin mode
  pinMode(ventilator, OUTPUT);
  pinMode(ledmatrix, OUTPUT);
  pinMode(waterpump, OUTPUT);
  pinMode(soilmoistureSensor, INPUT);
}

void processTemperatureAndHumidity()
{
  //read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // start ventilator if temperature or humidity is higher than max values
  if ((temperature > maxTemperature) || (humidity > maxHumidity))
  {
    digitalWrite(ventilator, LOW);
  }
  else
  {
    //stop ventilator
    digitalWrite(ventilator, HIGH);
  }
}

void processsoilMoisture()
{
  // get sensor value 
  soilMoistureValue = analogRead(soilmoistureSensor); 
  // get soil moisture percent
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

  // start water pump if soilmoisturepercent is higher than minSoilMoisturepercent
  if (soilmoisturepercent < minSoilMoisturepercent)
  {
    digitalWrite(waterpump, LOW);
  }
  else
  {
    //stop water pump
    digitalWrite(waterpump, HIGH);
  }
}

void processLightLevel()
{
  // get reading from gy302 light sensor
  uint16_t lux = GY302.readLightLevel();
  
  //start ledmatrix if lightlevel is higher than minLightLevel
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
  processLightLevel();
}