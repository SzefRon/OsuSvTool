#include "Logic/Normalizer.h"

#include "Logic/MapConfig.h"

void Normalizer::setBPM(double BPMtoNormalize)
{
    this->BPMtoNormalize = 60000.0 / BPMtoNormalize;
}

double Normalizer::getBPM()
{
    return BPMtoNormalize;
}

/// @brief 
/// @return 0 - ok, 1 - map not loaded
short Normalizer::autoDetectBPM()
{
    if (MapConfig::i().loadMap()) return 1;

    std::deque<TimingPoint *> timingPoints = MapConfig::i().getTPs();
    std::deque<Duration> UTPdurations;  // UTP - uninherited timing point

    int firstTime = 0;
    int secondTime = 0;
    double firstBPM;
    double secondBPM;

    TimingPoint *lastUTP;
    int lastObjectTime = MapConfig::i().getLastObjectTime();

    bool oneTimeFlag = true;
    bool exceededLastObject = false;

    // Setting up duration info for every unique bpm
    for (auto &tp : timingPoints) {
        if (!tp->uninherited) continue;
        Duration newDuration(0, 0, tp->beatLength);
        bool found = false;
        for (auto &duration : UTPdurations) {
            if (duration.BPM == newDuration.BPM) {
                found = true;
                break;
            }
        }
        if (!found) {
            UTPdurations.push_back(newDuration);
        }
    }

    int priority = 0;
    for (auto &tp : timingPoints) {
        if (exceededLastObject) break;
        if (!tp->uninherited) continue;

        lastUTP = tp;
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

        for (auto &duration : UTPdurations) {
            if (duration.BPM == firstBPM) {
                duration.duration += (secondTime - firstTime);
                duration.priority = priority;
                break;
            }
        }

        firstBPM = secondBPM;
        firstTime = secondTime;
        priority++;
    }

    // Giving highest priority to the last UTP
    for (auto &duration : UTPdurations) {
        if (duration.BPM == firstBPM) {
            duration.priority = priority;
            break;
        }
    }

    // Counting extra duration for when
    // last UTP is before last object
    if (lastUTP->time < lastObjectTime) {
        for (auto &duration : UTPdurations) {
            if (duration.BPM == lastUTP->beatLength) {
                duration.duration += lastObjectTime - lastUTP->time;
                break;
            }
        }
    }

    // Finding the BPM to normalize to
    Duration maxDuration(-1, -1, 0.0);
    for (auto &duration : UTPdurations) {
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

short Normalizer::normalize()
{
    if (autoDetectBPM() == 1) return 1;
    std::deque<TimingPoint *> &timingPoints = MapConfig::i().getTPs();
    std::deque<TimingPoint *> newTimingPoints;

    for (auto &tp : timingPoints) {
        newTimingPoints.push_back(tp);
        if (tp->uninherited && tp->beatLength != BPMtoNormalize) {
            double normalizedSV = -100.0 / (tp->beatLength / BPMtoNormalize);

            TimingPoint *normalizedTP = new TimingPoint(
                tp->time, normalizedSV, tp->meter, tp->sampleSet,
                tp->sampleIndex, tp->volume, 0, tp->effects
            );

            newTimingPoints.push_back(normalizedTP);
        }
    }

    timingPoints = newTimingPoints;

    return 0;
}