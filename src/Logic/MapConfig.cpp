#include "Logic/MapConfig.h"

#include <fstream>
#include <sstream>

MapConfig::MapConfig()
{}

std::pair<int, std::string> MapConfig::readVal(std::string timingPoint)
{
    std::stringstream ss(timingPoint);
    std::string val;

    std::getline(ss, val, ',');
    int time = std::stoi(val);

    std::getline(ss, val, ',');

    return std::make_pair(time, val);
}

MapConfig &MapConfig::i()
{
    static MapConfig I;
    return I;
}

/// @brief 
/// @param mapPath 
/// @return 0 - ok, 1 - failed to open file
short MapConfig::loadMap(std::wstring mapPath)
{
    this->mapPath = mapPath;
    isMapLoaded = false;

    std::ifstream inputMap(mapPath);
    std::string line;

    if (!inputMap.is_open()) return 1;
    
    // Info
    while (std::getline(inputMap, line, '\n')) {
        infoBeforeTPs += line + '\n';
        if (line == "[TimingPoints]") {
            break;
        }
    }

    // Timing points
    while (std::getline(inputMap, line, '\n')) {
        if (line.empty() || line.at(0) == '[') {
            infoAfterTPs += line;
            break;
        }
        std::string val;
        std::stringstream ss(line);
        TimingPoint *tp = new TimingPoint();

        std::getline(ss, val, ',');
        tp->time = std::stoi(val);
        std::getline(ss, val, ',');
        tp->beatLength = std::stod(val);
        std::getline(ss, val, ',');
        tp->meter = std::stoi(val);
        std::getline(ss, val, ',');
        tp->sampleSet = std::stoi(val);
        std::getline(ss, val, ',');
        tp->sampleIndex = std::stoi(val);
        std::getline(ss, val, ',');
        tp->volume = std::stoi(val);
        std::getline(ss, val, ',');
        tp->uninherited = std::stoi(val);
        std::getline(ss, val, ',');
        tp->effects = std::stoi(val);

        timingPoints.push_back(tp);
    }

    // Info
    while (std::getline(inputMap, line, '\n')) {
        infoAfterTPs += line + '\n';
        if (line == "[HitObjects]") {
            break;
        }
    }

    // Trying to get the last object
    // Last non-empty line should be it?
    std::string lastLine;
    while (std::getline(inputMap, line, '\n')) {
        infoAfterTPs += line + '\n';
        if (line.empty()) {
            break;
        }
        lastLine = line;
    }

    // Third value of an object is its time value
    std::stringstream ss(lastLine);
    for (int i = 0; i < 3; i++) {
        std::getline(ss, line, ',');
    }
    lastObjectTime = std::stoi(line);

    inputMap.close();

    isMapLoaded = true;
    return 0;
}

short MapConfig::normalize()
{
    std::ofstream output(mapPath);

    output << infoBeforeTPs;

    for (auto &tp : timingPoints) {
        output << tp->time << ',' << tp->beatLength << ','
               << tp->meter << ',' << tp->sampleSet << ','
               << tp->sampleIndex << ',' << tp->volume << ','
               << tp->uninherited << ',' << tp->effects << '\n';
        if (tp->uninherited && tp->beatLength != BPMtoNormalize) {
            double normalizedSV = -100.0 / (tp->beatLength / BPMtoNormalize);
            output << tp->time << ',' << normalizedSV << ','
                   << tp->meter << ',' << tp->sampleSet << ','
                   << tp->sampleIndex << ',' << tp->volume << ','
                   << 0 << ',' << tp->effects << '\n';
        }
    }

    output << infoAfterTPs;

    output.close();
    return 0;
}

void MapConfig::setBPM(double BPMtoNormalize)
{
    this->BPMtoNormalize = 60000.0 / BPMtoNormalize;
}

double MapConfig::getBPM()
{
    return BPMtoNormalize;
}

/// @brief 
/// @return 0 - ok, 1 - map not loaded
short MapConfig::autoDetectBPM()
{
    if (!isMapLoaded) return 1;

    std::deque<Duration> durations;
    std::string timingPoint;

    int firstTime = 0;
    int secondTime = 0;
    double firstBPM;
    double secondBPM;

    bool oneTimeFlag = true;
    bool exceededLastObject = false;

    int priority = 0;
    for (auto &tp : timingPoints) {
        if (!tp->uninherited) continue;
        secondTime = tp->time;
        secondBPM = tp->beatLength;

        if (secondTime > lastObjectTime) {
            exceededLastObject = true;
            secondTime = lastObjectTime;
        }

        if (oneTimeFlag) {
            oneTimeFlag = false;
            firstBPM = secondBPM;
        }

        bool found = false;
        for (auto &duration : durations) {
            if (duration.BPM == firstBPM) {
                found = true;
                duration.duration += (secondTime - firstTime);
                duration.priority = priority;
            }
        }
        if (!found) {
            durations.push_back(Duration((secondTime - firstTime), priority, firstBPM));
        }
        firstBPM = secondBPM;
        firstTime = secondTime;
        priority++;
    }

    for (auto &duration : durations) {
        if (duration.BPM == firstBPM) {
            duration.priority = priority;
        }
    }

    Duration maxDuration(-1, -1, 0.0);
    for (auto &duration : durations) {
        if ((duration.duration > maxDuration.duration)
        || (duration.duration == maxDuration.duration && duration.priority > maxDuration.priority)) {
            maxDuration.duration = duration.duration;
            maxDuration.BPM = duration.BPM;
            maxDuration.priority = duration.priority;
        }
    }

    BPMtoNormalize = maxDuration.BPM;

    return 0;
}
