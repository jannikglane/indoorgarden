#ifndef MENU_H
#define MENU_H
//#include <String.h> 
#include <WString.h>
#include "settings.h"

enum MenuType {
    base = 0,
    lightControl = 1,
    waterControl = 2,
    tempControl = 3
};

struct MenuItem
{
    String title;
    int *valueToChange;
};

struct SubMenu
{
    String title;
    MenuItem items[5];
    MenuType type;
};

struct MenuBase
{
    int menuPos;
    MenuType currentMenu;
    SubMenu entries[3];
    LightSettings lightSettings;
    WaterSettings waterSettings;
    TempSettings tempSettings;
};

MenuBase initializeMenu();
SubMenu buildLightMenu(LightSettings *settings);
SubMenu buildWaterMenu(WaterSettings *settings);
SubMenu buildTempMenu(TempSettings *settings);
void navigateMenu(MenuBase *menu, int newPos);

#endif