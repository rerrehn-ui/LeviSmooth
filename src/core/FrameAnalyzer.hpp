#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <deque>

namespace LeviSmoothCore {

class FrameAnalyzer {
public:
    static constexpr std::size_t MAX_SAMPLES = 120;
    static constexpr double STUTTER_THRESHOLD_MS = 33.33;

    void addFrameSample(double frameTimeMs) {
        if (!std::isfinite(frameTimeMs) ||
            frameTimeMs <= 0.0 ||
            frameTimeMs > 10000.0) {
            return;
        }

        mSamples.push_back(frameTimeMs);

        if (mSamples.size() > MAX_SAMPLES) {
            mSamples.pop_front();
        }

        analyze();
    }

    double getAverageFrameTime() const {
        return mAverage;
    }

    double getFrameTimeVariation() const {
        return mVariation;
    }

    double getStutterRate() const {
        return mStutterRate;
    }

    std::size_t getSampleCount() const {
        return mSamples.size();
    }

    void reset() {
        mSamples.clear();
        mAverage = 0.0;
        mVariation = 0.0;
        mStutterRate = 0.0;
    }

private:
    void analyze() {
        if (mSamples.empty()) {
            return;
        }

        double total = 0.0;
        std::size_t stutters = 0;

        for (double sample : mSamples) {
            total += sample;

            if (sample > STUTTER_THRESHOLD_MS) {
                ++stutters;
            }
        }

        mAverage = total / mSamples.size();

        double squaredDifference = 0.0;

        for (double sample : mSamples) {
            const double difference = sample - mAverage;
            squaredDifference += difference * difference;
        }

        mVariation = std::sqrt(
            squaredDifference / mSamples.size()
        );

        mStutterRate =
            100.0 * stutters / mSamples.size();
    }

    std::deque<double> mSamples;

    double mAverage = 0.0;
    double mVariation = 0.0;
    double mStutterRate = 0.0;
};

}
