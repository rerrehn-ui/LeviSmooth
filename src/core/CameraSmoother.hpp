#pragma once

#include <algorithm>
#include <cmath>

namespace LeviSmoothCore {

struct CameraVector3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

class CameraSmoother {
public:
    void setStrength(double strength) {
        mStrength = std::clamp(strength, 0.01, 1.0);
    }

    double getStrength() const {
        return mStrength;
    }

    CameraVector3 smoothPosition(
        const CameraVector3& current,
        const CameraVector3& target,
        double deltaTime
    ) {
        return smoothVector(current, target, deltaTime);
    }

    CameraVector3 smoothRotation(
        const CameraVector3& current,
        const CameraVector3& target,
        double deltaTime
    ) {
        CameraVector3 adjustedTarget = target;

        adjustedTarget.x = current.x +
            shortestAngle(current.x, target.x);

        adjustedTarget.y = current.y +
            shortestAngle(current.y, target.y);

        adjustedTarget.z = current.z +
            shortestAngle(current.z, target.z);

        return smoothVector(current, adjustedTarget, deltaTime);
    }

    void reset(const CameraVector3& value = {}) {
        mCurrent = value;
        mInitialized = true;
    }

    CameraVector3 getCurrent() const {
        return mCurrent;
    }

private:
    double calculateAlpha(double deltaTime) const {
        if (!std::isfinite(deltaTime) || deltaTime <= 0.0) {
            return 0.0;
        }

        const double safeDelta =
            std::min(deltaTime, 0.1);

        return 1.0 - std::exp(
            -mStrength * safeDelta * 60.0
        );
    }

    CameraVector3 smoothVector(
        const CameraVector3& current,
        const CameraVector3& target,
        double deltaTime
    ) {
        const double alpha = calculateAlpha(deltaTime);

        CameraVector3 result{
            current.x + (target.x - current.x) * alpha,
            current.y + (target.y - current.y) * alpha,
            current.z + (target.z - current.z) * alpha
        };

        mCurrent = result;
        mInitialized = true;

        return result;
    }

    static double shortestAngle(
        double current,
        double target
    ) {
        constexpr double PI = 3.14159265358979323846;
        constexpr double TWO_PI = PI * 2.0;

        double difference = std::fmod(
            target - current + PI,
            TWO_PI
        );

        if (difference < 0.0) {
            difference += TWO_PI;
        }

        return difference - PI;
    }

    double mStrength = 0.25;
    CameraVector3 mCurrent{};
    bool mInitialized = false;
};

}
