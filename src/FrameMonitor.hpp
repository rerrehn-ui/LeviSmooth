#pragma once

#include "core/FrameAnalyzer.hpp"

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
        mAnalyzer.reset();
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

    double getFrameTimeVariation() const {
        return mAnalyzer.getFrameTimeVariation();
    }

    double getStutterRate() const {
        return mAnalyzer.getStutterRate();
    }

    std::size_t getSampleCount() const {
        return mAnalyzer.getSampleCount();
    }

private:
    static constexpr std::size_t MAX_SAMPLES = 120;

    void addSample(double frameTime) {
        mAnalyzer.addFrameSample(frameTime);

        mSamples.push_back(frameTime);

        if (mSamples.size() > MAX_SAMPLES) {
            mSamples.pop_front();
        }

        double total = 0.0;

        for (double sample : mSamples) {
            total += sample;
        }

        if (!mSamples.empty()) {
            mAverageFrameTime =
                total / static_cast<double>(mSamples.size());

            mFps = 1000.0 / mAverageFrameTime;
            mStutterPercent = mAnalyzer.getStutterRate();
        }
    }

    std::deque<double> mSamples;
    FrameAnalyzer mAnalyzer;

    Clock::time_point mLastFrame{};

    double mAverageFrameTime = 0.0;
    double mFps = 0.0;
    double mStutterPercent = 0.0;
};

}
