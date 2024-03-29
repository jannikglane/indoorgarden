#ifndef MENU_H
#define MENU_H

#include <LiquidCrystal.h>
#include <math.h>

const int navButton = 7;
const int okButton = 5;

extern uint8_t navState;
extern uint8_t okState;
extern String currentSelection;
extern String nextSelection;
extern int changeDetector;

const int totalNodes = 3;
const int settingNodes = 4;

enum SettingType
{
  threshold = 0,
  toggle = 1,
  startTime = 2,
  endTime = 3,
  fixedTime = 4,
};

struct Setting
{
  SettingType type;
  bool onBackButton;
  float threshold;
  bool toggle;
  float startTime;
  float endTime;
  float fixedTime;
};

struct SettingNode {
  String name;
  Setting setting;
};

struct MenuNode {
  String name;
  SettingNode settings[settingNodes];
  bool inEditSetting;
  bool inSettingsNode;
  bool hasSetting;
  int settingsPos;
  bool onBackButton;
};

struct Menu {
  int nodePos;
  MenuNode nodes[totalNodes];
};

extern Menu menu;

void setupMenu(LiquidCrystal *lcd);

int determineNextPos(int currentPos, int maxPos);

String determineNextNodeSelection(int currentPos, int maxPos);

String determineNextSettingSelection(int currentPos, int maxPos);

void printMenu(LiquidCrystal *lcd);

void changeSelection();

void enterSettingsNodes(MenuNode *node);

String convertToTime(float time);

String getSettingValueAsString(Setting *setting);

void enterSettingEditMode(MenuNode *node);

void alterSettingValue(float *value, float max);

void alterValue(Setting *setting);

void navigateMenu();

void processOutput(LiquidCrystal *lcd);

#endif
