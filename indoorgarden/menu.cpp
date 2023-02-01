#include "menu.h"
#include <Arduino.h>

extern uint8_t navState = 0x0;
extern uint8_t okState = 0x0;
extern String currentSelection = "";
extern String nextSelection = "";
extern int changeDetector = 1;
extern Menu menu = {0,
  {
    {"Lichtmenue",
    {
      {"Schwellwert", {threshold, false, 1.0, false, 0, 0, 0}},
      {"Zeit verwenden", {toggle, false, 1.0, false, 0, 0, 0}},
      {"Start-Zeit", {startTime, false, 1.0, false, 0, 0, 0}},
      {"End-Zeit", {endTime, false, 1.0, false, 0, 0, 0}}
    },
    false,
    false,
    false,
    0,
    false},
    {"Wassermenue",
    {
      {"Schwellwert", {threshold, false, 1.0, false, 0, 0, 0}},
      {"Zeit verwenden", {toggle, false, 1.0, false, 0, 0, 0}},
      {"Start-Zeit", {startTime, false, 1.0, false, 0, 0, 0}},
      {"End-Zeit", {endTime, false, 1.0, false, 0, 0, 0}}
    },
    false,
    false,
    false,
    0,
    false},
    {"Temp.menue",
    {
      {"Schwellwert", {threshold, false, 1.0, false, 0, 0, 0}},
      {"Zeit verwenden", {toggle, false, 1.0, false, 0, 0, 0}},
      {"Start-Zeit", {startTime, false, 1.0, false, 0, 0, 0}},
      {"End-Zeit", {endTime, false, 1.0, false, 0, 0, 0}}
    },
    false,
    false,
    false,
    0,
    false}
  }
};

void setupMenu(LiquidCrystal *lcd) {
  lcd->begin(16, 2);

  changeSelection();
}

int determineNextPos(int currentPos, int maxPos)
{  
  if((currentPos + 1) >= (maxPos))
  {
    return 0;
  }
  return currentPos + 1;
}

String determineNextNodeSelection(int currentPos, int maxPos)
{
  if((currentPos + 1) >= (maxPos))
  {
    return menu.nodes[0].name;
  }

  return menu.nodes[currentPos+1].name;
}

String determineNextSettingSelection(int currentPos, int maxPos)
{ 
  if((currentPos + 1) >= (maxPos))
  {
    return menu.nodes[menu.nodePos].settings[0].name;
  }

  return menu.nodes[menu.nodePos].settings[currentPos+1].name;
}

void printMenu(LiquidCrystal *lcd)
{
  lcd->clear();
  lcd->setCursor(0,0);
  lcd->print("> ");
  lcd->print(currentSelection);
  lcd->setCursor(0,1);
  lcd->print(nextSelection);
}

void changeSelection()
{
  MenuNode *currentNode = &menu.nodes[menu.nodePos];

  if (currentNode->onBackButton == false && currentNode->settingsPos == (settingNodes - 1) && currentNode->inSettingsNode == true)
  {
      currentSelection = currentNode->settings[currentNode->settingsPos].name;
      nextSelection = "Zureuck";
  }
  else if (currentNode->onBackButton == true && currentNode->settingsPos == (settingNodes - 1))
  {
    currentSelection = "Zurueck";
    nextSelection = currentNode->settings[0].name;
  }
  else if (currentNode->inEditSetting == true && currentNode->inSettingsNode == true)
  {
    String value = getSettingValueAsString(&currentNode->settings[currentNode->settingsPos].setting);
    String back = "Zurueck";

    if (currentNode->settings[currentNode->settingsPos].setting.onBackButton)
    {
      currentSelection = back;
      nextSelection = value;
    }
    else
    {
      currentSelection = value;
      nextSelection = back;
    }
  }
  else if (currentNode->inSettingsNode == false)
  {
    currentSelection = currentNode->name;
    nextSelection = determineNextNodeSelection(menu.nodePos, totalNodes);
  }
  else
  {
    currentSelection = menu.nodes[menu.nodePos].settings[menu.nodes[menu.nodePos].settingsPos].name;
    nextSelection = determineNextSettingSelection(menu.nodes[menu.nodePos].settingsPos, settingNodes);
  }

  changeDetector = 1;
}

void enterSettingsNodes(MenuNode *node)
{
  node->inSettingsNode = true;
  changeSelection();
}

String convertToTime(float time)
{
  int hour = floor(time);
  int minutes = floor(((time - hour) * 60));

  String output = "";
  output.concat(hour);
  output.concat(":");
  output.concat(minutes);
  return output;
}

String getSettingValueAsString(Setting *setting)
{
  String val = "";

    switch (setting->type) {
    case threshold:
      val = (String)(int)floor(setting->threshold * 100);
      val.concat(" %");
      break;
    case toggle:
      if (setting->toggle)
      {
        return "[X] AN [] AUS";
      }
      return "[] AN [X] AUS";
      break;
    case startTime:
      val = convertToTime(setting->startTime);
      break;
    case endTime:
      val = convertToTime(setting->endTime);
      break;
    case fixedTime:
      val = convertToTime(setting->fixedTime);
      break;
    default:
      return "default"; 
  }

  return val;
}

void enterSettingEditMode(MenuNode *node)
{
  node->inEditSetting = true;
}

void alterSettingValue(float *value, float max)
{
  if (*value >= max)
  {
    *value = 0.0;
  }
  else
  {
    *value += 0.05;
  }
}

void alterValue(Setting *setting)
{
  switch (setting->type)
  {
  case threshold:
    alterSettingValue(&setting->threshold, 1.0);
    break;
  case toggle:
    setting->toggle = !setting->toggle;
    break;
  case startTime:
    alterSettingValue(&setting->startTime, 24);
    break;
  case endTime:
    alterSettingValue(&setting->endTime, 24);
    break;
  case fixedTime:
    alterSettingValue(&setting->fixedTime, 24);
    break;
  default:
    break;
  }
}

void navigateMenu()
{
  MenuNode *currentNode = &menu.nodes[menu.nodePos];

  if (navState == 0x1)
  {
    if (currentNode->settingsPos == (settingNodes - 1) && currentNode->onBackButton == false && currentNode->inSettingsNode == true)
    {
      currentNode->onBackButton = true;
    }
    else if (currentNode->settingsPos == (settingNodes - 1) && currentNode->onBackButton == true)
    {
      currentNode->onBackButton = false;
      currentNode->settingsPos = determineNextPos(currentNode->settingsPos, settingNodes);
    }
    else if (currentNode->inSettingsNode == true && currentNode->inEditSetting == true)
    {
      currentNode->settings[currentNode->settingsPos].setting.onBackButton = !currentNode->settings[currentNode->settingsPos].setting.onBackButton;
    }
    else if (currentNode->inSettingsNode == false)
    {
      menu.nodePos = determineNextPos(menu.nodePos, totalNodes);
    }
    else
    {
      currentNode->settingsPos = determineNextPos(currentNode->settingsPos, settingNodes);
    }
    changeSelection();
    return;
  }

  if (okState == 0x1)
  {
    if (currentNode->onBackButton == true)
    {
      currentNode->onBackButton = false;
      currentNode->inSettingsNode = false;
      currentNode->inEditSetting = false;
    }
    else if (currentNode->inSettingsNode == true && currentNode->inEditSetting == true && currentNode->settings[currentNode->settingsPos].setting.onBackButton == true)
    {
      currentNode->inEditSetting = !currentNode->inEditSetting;
    }
    else if (currentNode->inSettingsNode == true && currentNode->inEditSetting == true && currentNode->settings[currentNode->settingsPos].setting.onBackButton == false)
    {
      alterValue(&currentNode->settings[currentNode->settingsPos].setting);
    }
    else if (menu.nodes[menu.nodePos].inSettingsNode == true)
    {
      enterSettingEditMode(&menu.nodes[menu.nodePos]);
    }
    else
    {
      enterSettingsNodes(&menu.nodes[menu.nodePos]);
    }

    changeSelection();
    return;
  }
}

void processOutput(LiquidCrystal *lcd)
{
  if (changeDetector == 0)
  {
    return;
  }

  printMenu(lcd);
  changeDetector = 0;
}