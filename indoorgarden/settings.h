#ifndef SETTINGS_H
#define SETTINGS_H

struct LightSettings
{
    int automatic;
    int lightThreshold;
    int timeFrom;
    int timeTo;
};

struct WaterSettings
{
    int humidityThreshold;
};

struct TempSettings
{
    int tempTreshold;
};

#endif