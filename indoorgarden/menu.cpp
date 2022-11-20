#include "menu.h"

MenuBase initializeMenu()
{
    LightSettings lightSettings = {};
    WaterSettings waterSettings = {};
    TempSettings tempSettings = {};

    SubMenu lightControl = buildLightMenu(&lightSettings);

    SubMenu waterControl = {};
    lightControl.title = "Wassersteuerung";
    lightControl.type = MenuType::waterControl;

    SubMenu tempControl = {};
    lightControl.title = "Temp.Steuerung";
    lightControl.type = MenuType::tempControl;

    MenuBase menu = {};
    menu.menuPos = 0;
    menu.lightSettings = lightSettings;
    menu.waterSettings = waterSettings;
    menu.tempSettings = tempSettings;
    SubMenu subMenus[] = {lightControl, waterControl, tempControl};
    for (int i = 0; i < sizeof(menu.entries); i++)
    {
        menu.entries[i] = subMenus[i];
    }
    
    return menu;
}

SubMenu buildLightMenu(LightSettings *settings)
{
    SubMenu lightControl = {};
    lightControl.title = "Lichtsteuerung";
    lightControl.type = MenuType::lightControl;

    MenuItem lightAuto = {};
    lightAuto.title = "Auto. Lichtst.";
    lightAuto.valueToChange = &settings->automatic;
}

// Menu::Menu(MenuItem items[])
// {
//     this->_entries = items;
// }

// Menu::~Menu()
// {
// }


// MenuItem::MenuItem(String title)
// {
//     this->_title = title;
// }

// MenuItem::~MenuItem()
// {
// }