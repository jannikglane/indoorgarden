int inputTemperature = A0;
int led1 = 2;
float minTemp = 20;
const float BETA = 3950;

void setup() {
  pinMode(inputTemperature, INPUT);
  pinMode(led1, OUTPUT);
}
void processSensorValues(int inputvalue, int output)
{
  if (inputvalue < minTemp) 
  {
    digitalWrite(output, HIGH);
  } else
  {
    digitalWrite(output, LOW);
  }
}
float GetTemperature(int input)
{
  float sensorValue = analogRead(input);
  float temperature = 1 / (log(1 / (1023. / sensorValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  return temperature;
}
void loop() { 
    float temperature = GetTemperature(inputTemperature);
    processSensorValues(temperature, led1);
    //TODO: Luftfeuchtigkeit verarbeiten
    //TODO: Bodenfeuchtigkeit verarbeiten
    //TODO: Licht verarbeiten
}
