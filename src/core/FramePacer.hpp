#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace LeviSmoothCore {

class FramePacer {
public:
    void setTargetFPS(double fps) {
        if (!std::isfinite(fps) || fps <= 0.0) {
            return;
        }

        mTargetFPS = std::clamp(fps, 15.0, 120.0);
        mTargetFrameTimeMs = 1000.0 / mTargetFPS;
    }

    double getTargetFPS() const {
        return mTargetFPS;
    }

    double getTargetFrameTimeMs() const {
        return mTargetFrameTimeMs;
    }

    void submitFrameTime(double frameTimeMs) {
        if (!std::isfinite(frameTimeMs) ||
            frameTimeMs <= 0.0 ||
            frameTimeMs > 10000.0) {
            return;
        }

        mLastFrameTimeMs = frameTimeMs;

        mFrameDeviationMs =
            frameTimeMs - mTargetFrameTimeMs;

        if (mFrameDeviationMs > 0.0) {
            ++mLateFrames;
        }

        ++mTotalFrames;

        if (mTotalFrames > 0) {
            mLateFrameRate =
                100.0 * static_cast<double>(mLateFrames) /
                static_cast<double>(mTotalFrames);
        }
    }

    double getLastFrameTimeMs() const {
        return mLastFrameTimeMs;
    }

    double getFrameDeviationMs() const {
        return mFrameDeviationMs;
    }

    double getLateFrameRate() const {
        return mLateFrameRate;
    }

    std::uint64_t getTotalFrames() const {
        return mTotalFrames;
    }

    void reset() {
        mLastFrameTimeMs = 0.0;
        mFrameDeviationMs = 0.0;
        mLateFrameRate = 0.0;
        mLateFrames = 0;
        mTotalFrames = 0;
    }

private:
    double mTargetFPS = 60.0;
    double mTargetFrameTimeMs = 1000.0 / 60.0;

    double mLastFrameTimeMs = 0.0;
    double mFrameDeviationMs = 0.0;
    double mLateFrameRate = 0.0;

    std::uint64_t mLateFrames = 0;
    std::uint64_t mTotalFrames = 0;
};

}
