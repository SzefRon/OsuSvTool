#pragma once

#include <string>
#include <wx/log.h>

class MapConfig
{
private:
    MapConfig();

    bool isMapLoaded = false;
    double BPMtoNormalize;

    std::string info;
    std::string timingPoints;
    int lastObjectTime;

    std::pair<int, std::string> readVal(std::string timingPoint);
public:
    static MapConfig &i();
    short loadMap(std::wstring mapPath);
    short normalize();

    void setBPM(double BPMtoNormalize);
    double getBPM();
    short autoDetectBPM();
};