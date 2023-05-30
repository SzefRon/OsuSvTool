#include "Logic/MapConfig.h"

#include <fstream>
#include <sstream>
#include <deque>

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
    isMapLoaded = false;

    std::ifstream inputMap(mapPath);
    std::string line;

    if (!inputMap.is_open()) return 1;
    
    // Info
    while (std::getline(inputMap, line, '\n')) {
        info += line + '\n';
        if (line == "[TimingPoints]") {
            break;
        }
    }

    // Timing points
    while (std::getline(inputMap, line, '\n')) {
        if (line.empty() || line.at(0) == '[') {
            info += line + '\n';
            break;
        }
        timingPoints += line + '\n';
    }

    // Info
    while (std::getline(inputMap, line, '\n')) {
        info += line + '\n';
        if (line == "[HitObjects]") {
            break;
        }
    }

    // Trying to get the last object
    // Last non-empty line should be it?
    std::string lastLine;
    while (std::getline(inputMap, line, '\n')) {
        info += line + '\n';
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

    std::deque<std::pair<int, std::string>> durations;
    std::stringstream ss(timingPoints);
    std::string timingPoint;

    int firstTime = 0;
    int secondTime = 0;
    std::string firstBPM;
    std::string secondBPM;

    bool oneTimeFlag = true;
    bool exceededLastObject = false;

    while (!exceededLastObject && std::getline(ss, timingPoint, '\n')) {
        std::pair<int, std::string> vals = readVal(timingPoint);
        secondTime = vals.first;
        secondBPM = vals.second;

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
            if (duration.second == firstBPM) {
                found = true;
                duration.first += (secondTime - firstTime);
            }
        }
        if (!found) {
            durations.push_back(std::make_pair((secondTime - firstTime), firstBPM));
        }
        firstBPM = secondBPM;
        firstTime = secondTime;
    }

    int maxDuration = -1;
    std::string maxBPM;
    for (auto &duration : durations) {
        if (duration.first > maxDuration) {
            maxDuration = duration.first;
            maxBPM = duration.second;
        }
    }

    BPMtoNormalize = std::stod(maxBPM);

    return 0;
}
