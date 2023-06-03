#include "Logic/MapConfig.h"

#include <fstream>
#include <sstream>
#include <set>

MapConfig::MapConfig()
{}

MapConfig &MapConfig::i()
{
    static MapConfig I;
    return I;
}

void MapConfig::reset()
{
    infoBeforeTPs.clear();
    infoAfterTPs.clear();

    for (auto &tp : timingPoints) {
        delete tp;
    }

    timingPoints.clear();
    lastObjectTime = 0;
}

/// @brief 
/// @param mapPath 
/// @return 0 - ok, 1 - failed to open file
short MapConfig::loadMap()
{
    reset();

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

    return 0;
}

short MapConfig::saveMap()
{
    std::ofstream output(mapPath);

    output << infoBeforeTPs;

    for (auto &tp : timingPoints) {
        output << tp->time << ',' << tp->beatLength << ','
               << tp->meter << ',' << tp->sampleSet << ','
               << tp->sampleIndex << ',' << tp->volume << ','
               << tp->uninherited << ',' << tp->effects << '\n';
    }

    output << infoAfterTPs;

    output.close();
    return 0;
}

std::deque<TimingPoint *> &MapConfig::getTPs()
{
    return timingPoints;
}

const int &MapConfig::getLastObjectTime()
{
    return lastObjectTime;
}

/*short MapConfig::normalize()
{
    if (autoDetectBPM() == 1) return 1;
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
}*/
