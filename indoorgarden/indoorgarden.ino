int inputTemperature = A0;
int led1 = 2;
float minTemp = 20;
float inputvalue;

void setup() {
  pinMode(inputTemperature, INPUT);
  pinMode(led1, OUTPUT);
  Serial.begin(9600);
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
float GetTemperature()
{
}
void loop() {
    inputvalue := GetTemperature();
    processSensorValues(inputvalue, led1);


}
