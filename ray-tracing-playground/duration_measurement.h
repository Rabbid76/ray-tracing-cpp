#ifndef __DURATION_MEASUREMENT_H__
#define __DURATION_MEASUREMENT_H__

#include <vector>
#include <chrono>

class DurationMeasurement {

private:
    std::vector<std::chrono::high_resolution_clock::time_point> timePoints;

public:

    DurationMeasurement() {
        storeTimePoint();
    }

    DurationMeasurement& storeTimePoint() {
        timePoints.push_back(std::chrono::high_resolution_clock::now());
        return *this;
    }

    double getTotalDuration() const {
        auto duration = timePoints.back() - timePoints.front();
        return (double)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0;
    }

    double getLastDuration() const {
        auto duration = timePoints.back() - timePoints[timePoints.size() - 2];
        return (double)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0;
    }
};

#endif