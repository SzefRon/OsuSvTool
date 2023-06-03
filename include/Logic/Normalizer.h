#pragma once

class Normalizer
{
private:
    double BPMtoNormalize = 500.0;
public:
    void setBPM(double BPMtoNormalize);
    double getBPM();

    short autoDetectBPM();
    short normalize();
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