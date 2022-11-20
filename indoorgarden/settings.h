#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct LightSettings
{
    int automatic;
    int lightThreshold;
    int timeFrom;
    int timeTo;
};

typedef struct WaterSettings
{
    int humidityThreshold;
};

typedef struct TempSettings
{
    int tempTreshold;
};

#endif