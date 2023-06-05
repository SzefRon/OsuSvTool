#pragma once

#include <string>
#include <wx/log.h>
#include <deque>

struct TimingPoint;

class MapConfig
{
private:
    MapConfig();

    std::string infoBeforeTPs;
    std::string infoAfterTPs;
    std::deque<TimingPoint *> timingPoints;

    int lastObjectTime = 0;
public:
    static MapConfig &i();

    std::wstring mapPath;

    void reset();
    short loadMap();
    short saveMap();
    void insertTP(TimingPoint *newTp);

    std::deque<TimingPoint *> &getTPs();
    const int &getLastObjectTime();
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