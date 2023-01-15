#include <LiquidCrystal.h>
#include <DHT.h>
#define PIN 3
#define PIN2 4
#define TYPE DHT22
DHT dht(PIN, TYPE);
DHT dht2(PIN2, TYPE);

////////////////////////////////////////////////////////////////////////////
// configuring lcd display
// LCD RS pin to digital pin 12
// LCD Enable pin to digital pin 11
// LCD D4 pin to digital pin 5
// LCD D5 pin to digital pin 4
// CD D6 pin to digital pin 3
// LCD D7 pin to digital pin 2
////////////////////////////////////////////////////////////////////////////
const int rs = 2, e = 4, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

int led1 = 12;
float maxTemperature = 20;
float maxHumidity = 30;

////////////////////////////////////////////////////////////////////////////
// Starting Initializing data of soilmoisture
const int AirValue = 620;   // you need to replace this value with Value_1
const int WaterValue = 310; // you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent = 0;
// Ending Initializing data of soilmoisture
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Starting Initializing data of light
int light = 0;
// Ending Initializing data of light
////////////////////////////////////////////////////////////////////////////

void setup()
{
  pinMode(led1, OUTPUT);
  dht.begin();
  ////////////////////////////////////////////////////////////////////////////
  // pinMode(led2, OUTPUT);
  // dht2.begin();
  ////////////////////////////////////////////////////////////////////////////
  // initial lcd setup
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("IndoorGarden");
}
void processTemperatureAndHumidity(float temperature, float humidity, int output, float maxTemperature, float maxHumidity)
{
  if ((temperature > maxTemperature) || (humidity > maxHumidity))
  {
    digitalWrite(output, HIGH);
  }
  else
  {
    digitalWrite(output, LOW);
  }
}

void processsoilMoisture()
{
  soilMoistureValue = analogRead(A0); // put Sensor insert into soil
  Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  if (soilmoisturepercent >= 100)
  {
    Serial.println("100 %");
    lcd.setCursor(0, 0);
    lcd.print("Soil Moisture");
    lcd.setCursor(0, 1);
    lcd.print("100 %");
    delay(250);
    lcd.clear();
  }
  else if (soilmoisturepercent <= 0)
  {
    Serial.println("0 %");
    lcd.setCursor(0, 0);
    lcd.print("Soil Moisture");
    lcd.setCursor(0, 1);
    lcd.print("0 %");
    delay(250);
    lcd.clear();
  }
  else if (soilmoisturepercent > 0 && soilmoisturepercent < 100)
  {
    Serial.print(soilmoisturepercent);
    Serial.println("%");
    lcd.setCursor(0, 0);
    lcd.print("Soil Moisture");
    lcd.setCursor(0, 1);
    lcd.print(soilmoisturepercent);
    lcd.print(" %");
    delay(250);
    lcd.clear();
  }
}

void processLight()
{
  int raw_light = analogRead(light_sensor);    // read the raw value from light_sensor pin (A3)
  int light = map(raw_light, 0, 1023, 0, 100); // map the value from 0, 1023 to 0, 100

  Serial.print("Light level: ");
  Serial.println(light); // print the light value in Serial Monitor
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  processTemperatureAndHumidity(temperature, humidity, led1, maxTemperature, maxHumidity);

  // float soilMoistureValue = dht2.readHumidity();
  // processsoilMoisture(soilMoistureValue,);
  // TODO: Bodenfeuchtigkeit verarbeiten
  processsoilMoisture();
  // TODO: Licht verarbeiten
  processLight();
}
