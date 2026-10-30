
#pragma once

#include <chrono>
#include <cstdint>

namespace LeviSmooth {

class FrameMonitor {
public:
    using Clock = std::chrono::steady_clock;

    void beginFrame() {
        const auto now = Clock::now();

        if (mLastFrame.time_since_epoch().count() != 0) {
            const auto elapsed =
                std::chrono::duration<double, std::milli>(
                    now - mLastFrame
                ).count();

            mFrameTimeMs = elapsed;

            if (elapsed > 0.0) {
                mFps = 1000.0 / elapsed;
            }
        }

        mLastFrame = now;
    }

    double getFrameTimeMs() const {
        return mFrameTimeMs;
    }

    double getFPS() const {
        return mFps;
    }

private:
    Clock::time_point mLastFrame{};
    double mFrameTimeMs = 0.0;
    double mFps = 0.0;
};

}
