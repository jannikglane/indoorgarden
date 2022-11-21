#ifndef MENU_H
#define MENU_H
#include "settings.h"

enum MenuType {
    base = 0,
    lightControl = 1,
    waterControl = 2,
    tempControl = 2
};

typedef struct MenuBase
{
    int menuPos;
    MenuType currentMenu;
    SubMenu entries[3];
    LightSettings lightSettings;
    WaterSettings waterSettings;
    TempSettings tempSettings;
};

typedef struct SubMenu
{
    String title;
    MenuItem items[5];
    MenuType type;
};

typedef struct MenuItem
{
    String title;
    int *valueToChange;
};

MenuBase initializeMenu();
SubMenu buildLightMenu(LightSettings *settings);
SubMenu buildWaterMenu(WaterSettings *settings);
SubMenu buildTempMenu(TempSettings *settings);
void navigateMenu(struct MenuBase *menu, int newPos);

#endif