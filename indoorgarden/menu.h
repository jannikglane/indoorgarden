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
    SubMenu entries[3];
    int menuPos;
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


// class Menu
// {
// private:
//     int _menuPos = 0;
//     MenuItem *_entries[];
// public:
//     Menu(MenuItem items[]);
//     ~Menu();
// };


// class MenuItem
// {
// private:
//     String _title;
// public:
//     MenuItem(String title);
//     ~MenuItem();
// };


#endif