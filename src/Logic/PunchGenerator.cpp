#include "Logic/PunchGenerator.h"

#include <sstream>
#include <math.h>

#include "Logic/MapConfig.h"

/// @brief 
/// @param strVal 
/// @return 0 - ok, 1 - failed to parse value
short PunchGenerator::setStartingPoint(std::string strVal)
{
    strVal = strVal.substr(0, strVal.find(' '));
    std::stringstream ss(strVal);

    int startingPointBuf = 0;
    std::string temp;

    if (strVal.find(':') != strVal.npos) {
        try {
            if (!std::getline(ss, temp, ':')) return 1;
            startingPointBuf += std::stoi(temp) * 60000;
            if (!std::getline(ss, temp, ':')) return 1;
            startingPointBuf += std::stoi(temp) * 1000;
            if (!std::getline(ss, temp, ':')) return 1;
            startingPointBuf += std::stoi(temp);
        }
        catch (...) {
            return 1;
        }
    } else {
        try {
            startingPointBuf = std::stoi(strVal);
        }
        catch (...) {
            return 1;
        }
    }

    startingPoint = startingPointBuf;
    return 0;
}

void PunchGenerator::setBSdivisor(std::string strVal)
{
    strVal = strVal.substr(strVal.find('/') + 1, strVal.npos);
    bsDivisor = std::stoi(strVal);
}

double PunchGenerator::calculateMultiplier(double power)
{
    double distanceSum = 0.0;
    for (double n = 1.0; n <= noSVs; n += 1.0) {
        distanceSum += std::pow(n, power);
    }

    return std::pow((normSV * noSVs) / distanceSum, 1.0 / power);
}

double PunchGenerator::zeroAtMatchingPower(double power)
{
    double multiplier = calculateMultiplier(power);
    return std::pow(noSVs * multiplier, power) - topSV;
}

void PunchGenerator::findMatchingPower()
{
    double a = (-20.0 / noSVs) - 5.0;
    double b = (noSVs - 1) * 10.0;

    double x0 = 0.0;
    double fx0 = 1.0;

    while (fabs(fx0) > 0.000001) {
        x0 = (b + a) * 0.5;
        double fa = zeroAtMatchingPower(a);
        double fb = zeroAtMatchingPower(b);
        fx0 = zeroAtMatchingPower(x0);

        if (fa * fx0 < 0.0) {
            b = x0;
        }
        else if (fb * fx0 < 0.0) {
            a = x0;
        }
    }

    if (fabs(fx0) < 0.001) {
        matchingPower = x0;
        matchingMultiplier = calculateMultiplier(matchingPower);
    }
}

short PunchGenerator::generateSVs()
{
    if (MapConfig::i().loadMap()) return 1;

    const std::deque<TimingPoint *> &timingPoints = MapConfig::i().getTPs();

    double beatLength = 0.0;

    // Find BPM
    TimingPoint *lastTp = nullptr;
    TimingPoint *effectiveTp = nullptr;
    for (auto &tp : timingPoints) {
        if (!tp->uninherited) continue;

        if (tp->time > startingPoint) {
            if (!lastTp) {
                effectiveTp = tp;
                break;
            }
            else {
                effectiveTp = lastTp;
                break;
            }
        }
        lastTp = tp;
    }

    if (!effectiveTp) {
        effectiveTp = lastTp;
    }

    double tickLength = effectiveTp->beatLength / bsDivisor;

    int noTicks = std::nearbyint((startingPoint - effectiveTp->time) / tickLength);

    // On beat verification
    if (std::fabs(effectiveTp->time - (startingPoint - tickLength * noTicks)) >= 1.0) {
        // isn't on beat
    }
    else {
        double smallTickLength = tickLength / extraDivisor;
        double actualStartingPoint = effectiveTp->time + noTicks * tickLength;

        for (int i = noSVs; i >= 0; i--) {
            int newTime = std::floor(actualStartingPoint + smallTickLength * (noSVs - i));

            double newSVval;
            if (i == 0) {
                newSVval = -100.0 / normSV;
            }
            else {
                newSVval = std::pow(i * matchingMultiplier, matchingPower);
                newSVval = -100.0 / newSVval;
            }   

            TimingPoint *newTp = new TimingPoint(
                newTime, newSVval, effectiveTp->meter, effectiveTp->sampleSet,
                effectiveTp->sampleIndex, effectiveTp->volume, 0, effectiveTp->effects
            );

            MapConfig::i().insertTP(newTp);
        }
    }
    return 0;
}