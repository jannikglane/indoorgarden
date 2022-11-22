#ifndef MENU_H
#define MENU_H
//#include <String.h> 
#include <WString.h>
#include "settings.h"

// enum MenuType {
//     base = 0,
//     lightControl = 1,
//     waterControl = 2,
//     tempControl = 3
// };

// struct MenuItem
// {
//     String title;
//     int *valueToChange;
// };

// struct SubMenu
// {
//     String title;
//     MenuItem items[5];
//     MenuType type;
// };

// struct MenuBase
// {
//     int menuPos;
//     MenuType currentMenu;
//     SubMenu entries[3];
//     LightSettings lightSettings;
//     WaterSettings waterSettings;
//     TempSettings tempSettings;
// };

// MenuBase initializeMenu();
// SubMenu buildLightMenu(LightSettings *settings);
// SubMenu buildWaterMenu(WaterSettings *settings);
// SubMenu buildTempMenu(TempSettings *settings);
// void navigateMenu(MenuBase *menu, int newPos);

enum ActionType
{
    toggle = 1,
    values = 2
};

enum NodeType
{
    node = 1,
    action = 2
};
struct MenuAction
{
    ActionType type;
    String name;
    bool toogle;
    int values[48];
};

struct MenuNode
{
    NodeType type;
    String name;
    MenuNode *parentNode;
    MenuNode *childNode;
    MenuNode *siblingNode;
    MenuAction action;
};

MenuNode buildMenu();
void navigateToNextSibling(MenuNode *node);
void navigateToParent(MenuNode *node);
void navigateToChild(MenuNode *node);
void navigateToAction(MenuNode *node);
void toggleAction(MenuAction *action);
void navigateToValueSelection(MenuAction *action);

#endif