#pragma once

#include <string>

class PunchGenerator
{
private:
    double matchingMultiplier = 0.0;
    double matchingPower = 0.0;

    int startingPoint = 0;
    int bsDivisor = 4;
public:
    PunchGenerator() = default;

    int extraDivisor = 1;
    int noSVs = 3;
    double normSV = 1.0;
    double topSV = 1.5;

    short setStartingPoint(std::string strVal);
    void setBSdivisor(std::string strVal);

    double calculateMultiplier(double power);
    double zeroAtMatchingPower(double power);
    void findMatchingPower();
    short generateSVs();
};