/*
Title: Indoor Garden
Authors: Fu Yunfei, Jannik Glane, Matteo Kuschel
Date: 25.01.2023
*/

// include libraries
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <BH1750.h>
#include "menu.h"

// configuring lcd display
// const int rs = 2, e = 4, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

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

  // menu setup
  setupMenu(&lcd);

  pinMode(navButton, INPUT);
  pinMode(okButton, INPUT);

  // debug
  Serial.begin(9600);
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
    Delay(5000);
    digitalWrite(waterpump,HIGH);
    Delay(5000);
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
    // stop LED Matrix
    digitalWrite(ledmatrix, LOW);
  }
}

void readInput()
{
  navState = digitalRead(navButton);
  okState = digitalRead(okButton);
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

  // prevents multiple reads of a button press
  delay(150);
}