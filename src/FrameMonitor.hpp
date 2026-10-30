#pragma once

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <deque>

namespace LeviSmoothCore {

class FrameMonitor {
public:
    using Clock = std::chrono::steady_clock;

    void beginFrame() {
        const auto now = Clock::now();

        if (mLastFrame.time_since_epoch().count() != 0) {
            const double frameTime =
                std::chrono::duration<double, std::milli>(
                    now - mLastFrame
                ).count();

            if (frameTime > 0.0 && frameTime < 10000.0) {
                addSample(frameTime);
            }
        }

        mLastFrame = now;
    }

    void reset() {
        mSamples.clear();
        mLastFrame = {};
        mAverageFrameTime = 0.0;
        mFps = 0.0;
        mStutterPercent = 0.0;
    }

    double getFPS() const {
        return mFps;
    }

    double getAverageFrameTimeMs() const {
        return mAverageFrameTime;
    }

    double getStutterPercent() const {
        return mStutterPercent;
    }

private:
    static constexpr std::size_t MAX_SAMPLES = 120;
    static constexpr double STUTTER_THRESHOLD_MS = 33.33;

    void addSample(double frameTime) {
        mSamples.push_back(frameTime);

        if (mSamples.size() > MAX_SAMPLES) {
            mSamples.pop_front();
        }

        double total = 0.0;
        std::size_t stutters = 0;

        for (double sample : mSamples) {
            total += sample;

            if (sample > STUTTER_THRESHOLD_MS) {
                ++stutters;
            }
        }

        mAverageFrameTime =
            total / static_cast<double>(mSamples.size());

        mFps = 1000.0 / mAverageFrameTime;

        mStutterPercent =
            100.0 * static_cast<double>(stutters) /
            static_cast<double>(mSamples.size());
    }

    std::deque<double> mSamples;
    Clock::time_point mLastFrame{};

    double mAverageFrameTime = 0.0;
    double mFps = 0.0;
    double mStutterPercent = 0.0;
};

}
