/*
Title: Indoor Garden
Authors: Fu Yunfei, Jannik Glane, Matteo Kuschel
Date: 25.01.2023
*/
#include "menu.h"

// configuring lcd display
// const int rs = 2, e = 4, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

// configuring buttons
const int button_up = 5, button_down = 7, button_ok = 6; // button pins
enum Button {none = 0, next = 1, ok = 2};

// configuring menu struct

int inputTemperature = A0;
int led1 = 2;
float minTemp = 20;
const float BETA = 3950;

void setup() {
  pinMode(inputTemperature, INPUT);
  pinMode(led1, OUTPUT);

  // button setup
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_down, INPUT_PULLUP);
  pinMode(button_ok, INPUT_PULLUP);

  // menu setup
  setupMenu(&lcd);

  pinMode(navButton, INPUT);
  pinMode(okButton, INPUT);

  // debug
  Serial.begin(9600);

}

void processSensorValues(int inputValue, int minValue,int output)
{
  if (inputValue < minValue) 
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


void readInput()
{
  navState = digitalRead(navButton);
  okState = digitalRead(okButton);
}

void loop() { 
  float temperature = GetTemperature(inputTemperature);
  processSensorValues(temperature, minTemp, led1);
  //TODO: Luftfeuchtigkeit verarbeiten
  //TODO: Bodenfeuchtigkeit verarbeiten
  //TODO: Licht verarbeiten

  // menu
  readInput();
  navigateMenu();
  processOutput(&lcd);

  // prevents multiple reads of a button press
  delay(150);
}