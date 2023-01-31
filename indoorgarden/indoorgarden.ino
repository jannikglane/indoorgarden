/*
Title: Indoor Garden
Authors: Fu Yunfei, Jannik Glane, Matteo Kuschel
Date: 24.01.2023
*/

// include libraries
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <BH1750.h>
#include "menu.h"

// configuring lcd display
// const int rs = 13, e = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// initialize BH1750 object
BH1750 lightMeter;

//initialize DHT object
#define PIN 4
#define TYPE DHT11
DHT dht(PIN, TYPE);

// reference default values
const float maxTemperatureDefault = 24;
const float maxHumidityDefault = 70;
const float minSoilMoisturepercentDefault = 30;
const float minLightLevelDefault = 40;

// define min and max values
float maxTemperature = 24;
float maxHumidity = 70;
float minSoilMoisturepercent = 30;
float minLightLevel = 40;

// define pins
const int waterpump = 3;
const int ventilator = 2;
const int ledmatrix = 6;
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

  // start libraries for light sensor
  Wire.begin();
  lightMeter.begin();

  // set pin mode
  pinMode(ventilator, OUTPUT);
  pinMode(ledmatrix, OUTPUT);
  pinMode(waterpump, OUTPUT);
  pinMode(soilmoistureSensor, INPUT);

  pinMode(navButton, INPUT);
  pinMode(okButton, INPUT);

  // thresholds
  maxTemperature = maxTemperatureDefault;
  maxHumidity = maxHumidityDefault;
  minSoilMoisturepercent =minSoilMoisturepercentDefault;
  minLightLevel = minLightLevelDefault;

  // menu setup
  setupMenu(&lcd);

  // debug
  Serial.begin(9600);
  Serial.println("Initialized");
}

void processTemperatureAndHumidity()
{
  //read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // start ventilator if temperature or humidity is higher than max values
  if ((temperature > maxTemperature) || (humidity > maxHumidity))
  {
    digitalWrite(ventilator, HIGH);
  }
  else
  {
    //stop ventilator
    digitalWrite(ventilator, LOW);
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
  float lux = lightMeter.readLightLevel();
  
  //start ledmatrix if lightlevel is higher than minLightLevel
  if (lux < minLightLevel)
  {
    digitalWrite(ledmatrix, HIGH);
  }
  else
  {
    // stop LED Matrix
    digitalWrite(ledmatrix, LOW);
  }
}

void readInput()
{
  navState = digitalRead(navButton);
  okState = digitalRead(okButton);

  Serial.print(navState);
}

// update max values for the sensors 
void updateSensorValues()
{
  minLightLevel = (minLightLevel / 100) * (menu.nodes[0].settings[0].setting.threshold * 100);
  minSoilMoisturepercent = (minSoilMoisturepercentDefault / 100) * (menu.nodes[1].settings[0].setting.threshold * 100);
  maxTemperature = (maxTemperatureDefault / 100) * (menu.nodes[2].settings[0].setting.threshold * 100);
}

void loop()
{
  processTemperatureAndHumidity();
  processsoilMoisture();
  processLightLevel();

  // menu
  readInput();
  navigateMenu();
  processOutput(&lcd);
  updateSensorValues();

  // prevents multiple reads of a button press
  delay(150);
}
