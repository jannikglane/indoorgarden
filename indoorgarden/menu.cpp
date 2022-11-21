#include "menu.h"

MenuBase initializeMenu()
{
    // Initialize settings
    LightSettings lightSettings = {};
    WaterSettings waterSettings = {};
    TempSettings tempSettings = {};

    // Initializing menu with entries
    SubMenu lightControl = buildLightMenu(&lightSettings);
    SubMenu waterControl = buildWaterMenu(&waterSettings);
    SubMenu tempControl = buildTempMenu(&tempSettings);

    MenuBase menu = {
        0,
        MenuType::base,
        {lightControl, waterControl, tempControl},
        lightSettings,
        waterSettings,
        tempSettings
    };
    
    return menu;
}

SubMenu buildLightMenu(LightSettings *settings)
{
    MenuItem lightAuto = {"Auto. Lichtst.", &settings->automatic};

    MenuItem lightTreshold = {"Auto. %-Wert", &settings->lightThreshold};

    MenuItem lightTimeFrom = {"Zeit Ab", &settings->timeFrom};

    MenuItem lightTimeTo = {"Zeit Bis", &settings->timeTo};

    SubMenu lightControl = 
    {
        "Lichtsteuerung",
        {lightAuto, lightTreshold, lightTimeFrom, lightTimeTo},
        MenuType::lightControl
    };

    return lightControl;
}

SubMenu buildWaterMenu(WaterSettings *settings)
{
    MenuItem humidityTreshold = {"\%-Luftfeucht.", &settings->humidityThreshold};

    SubMenu waterControl = {
        "Wassersteuerung",
        {humidityTreshold},
        MenuType::waterControl
    };

    return waterControl;
}

SubMenu buildTempMenu(TempSettings *settings)
{
    MenuItem tempTreshold = {"Temperatur", &settings->tempTreshold};

    SubMenu tempControl = {
        "Temp.steuerung",
        {tempTreshold},
        MenuType::tempControl
    };

    return tempControl;
}

void navigateMenu(struct MenuBase *menu, int newPos)
{
    if (newPos > sizeof(menu->entries) || newPos < 0)
    {
        return;
    }
    menu->menuPos = newPos;
}
