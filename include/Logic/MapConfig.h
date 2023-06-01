#pragma once

#include <string>
#include <wx/log.h>
#include <deque>

struct TimingPoint;

class MapConfig
{
private:
    MapConfig();

    bool isMapLoaded = false;
    double BPMtoNormalize;

    std::wstring mapPath;
    std::string infoBeforeTPs;
    std::string infoAfterTPs;
    std::deque<TimingPoint *> timingPoints;
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

struct Duration
{
int duration, priority;
double BPM;
Duration(int duration, int priority, double BPM) {
    this->duration = duration;
    this->priority = priority;
    this->BPM = BPM;
}
};

struct TimingPoint
{
    int time, meter, sampleSet, sampleIndex, volume, effects;
    double beatLength;
    bool uninherited;
    TimingPoint() = default;
    TimingPoint(int time, double beatLength, int meter, int sampleSet, int sampleIndex, int volume, bool uninherited, int effects) {
        this->time = time;
        this->beatLength = beatLength;
        this->meter = meter;
        this->sampleSet = sampleSet;
        this->sampleIndex = sampleIndex;
        this->volume = volume;
        this->uninherited = uninherited;
        this->effects = effects;
    }
};