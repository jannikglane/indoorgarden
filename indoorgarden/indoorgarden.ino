#include <LiquidCrystal.h>
#include <DHT.h>
#define PIN 3
#define TYPE DHT22
DHT dht(PIN, TYPE);

// configuring lcd display
const int rs = 2, e = 4, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

int led1 = 12;
float maxTemperature = 20;
float maxHumidity = 30;

void setup() {
   pinMode(led1, OUTPUT);
   dht.begin();
  // initial lcd setup
    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    lcd.print("IndoorGarden");
}
void processTemperatureAndHumidity(float temperature, float humidity, int output, float maxTemperature, float maxHumidity)
{
  if ((temperature > maxTemperature) || (humidity > maxHumidity))
  {
    digitalWrite(output, HIGH);
  } else
  {
    digitalWrite(output, LOW);
  }
}
void loop() { 
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    processTemperatureAndHumidity(temperature, humidity, led1, maxTemperature, maxHumidity);

    //TODO: Bodenfeuchtigkeit verarbeiten
    //TODO: Licht verarbeiten
}
