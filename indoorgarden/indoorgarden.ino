// include libraries
#include <DHT.h>
#include <DHT_U.h>

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
const int input = A0;

// define values for soil moisture
const float AirValue = 620;
const float WaterValue = 310;
float soilMoistureValue = 0;
float soilmoisturepercent = 0;

void setup()
{
  // start dht library for temperature sensor
  dht.begin();
  
  // set pin mode
  pinMode(A5, INPUT);
  pinMode(ventilator, OUTPUT);
  pinMode(ledmatrix, OUTPUT);
  pinMode(input, INPUT);
}

void processTemperatureAndHumidity()
{
  //read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // start ventilator when exceeding limit values
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
    int rawValue = analogRead(A5); //Read light level
    Serial.println("Reading: " + String(rawValue));

    double volts = rawValue * 5.0 / 1024.0;
    long microvolts = volts * 1000000.0;
    Serial.println("Voltage: " + String(volts) + " V (" + String(microvolts) + " µV)");

    long microamps = microvolts / 10000;
    Serial.println("Current: " + String(microamps) + " µA");

    long lux = microamps * 2;
    Serial.println("Illuminance: " + String(lux) + " lx\n");

    delay(2000);
}
}

void loop()
{
  processTemperatureAndHumidity();
  processsoilMoisture();
  processLight();
}
