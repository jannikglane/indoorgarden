#include <LiquidCrystal.h>
#include "menu.h"

// configuring lcd display
const int rs = 2, e = 4, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

const int button_up = 5, button_down = 7, button_ok = 6; // button pins
enum Button {none = 0, up = 1, down = 2, ok = 3};

// MENU -- 16 chars per line
MenuBase menu;

int inputTemperature = A0;
int led1 = 2;
float minTemp = 20;
const float BETA = 3950;

void setup() {
  pinMode(inputTemperature, INPUT);
  pinMode(led1, OUTPUT);

  // initial lcd setup
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("IndoorGarden");

  menu = initializeMenu();
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
void loop() { 
  float temperature = GetTemperature(inputTemperature);
  processSensorValues(temperature, minTemp, led1);
  //TODO: Luftfeuchtigkeit verarbeiten
  //TODO: Bodenfeuchtigkeit verarbeiten
  //TODO: Licht verarbeiten

  enum Button input = getInput();
  processInput(input);
}

void processInput(enum Button input)
{
  switch(input)
  {
    case up:
      navigateMenu(&menu, menu.menuPos+1);
      displayMenu();
      break;
    case down:
      navigateMenu(&menu, menu.menuPos-1);
      displayMenu();
      break;
    case ok:
      // TODO: enter submenu
      break;
    case none:
      break;
  }
}

void displayMenu()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(menu.entries[menu.menuPos].title);
}

enum Button getInput()
{
  int up_state = 0, down_state = 0, ok_state = 0;

  up_state = digitalRead(button_up);
  down_state = digitalRead(button_down);
  ok_state = digitalRead(button_ok);

  if(up_state == HIGH)
  {
    return up;
  }
  if(down_state == HIGH)
  {
    return down;
  }
  if(ok_state == HIGH)
  {
    return ok;
  }
  return none;
}