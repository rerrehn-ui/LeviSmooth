#include <pl/Mod.hpp>

#include "FrameMonitor.hpp"
#include "core/FrameAnalyzer.hpp"
#include "core/CameraSmoother.hpp"
#include "core/FramePacer.hpp"

class LeviSmooth {
public:
    static LeviSmooth& instance() {
        static LeviSmooth mod;
        return mod;
    }

    LeviSmooth()
        : mSelf(*ll::mod::NativeMod::current()) {}

    bool load() {
        getSelf().getLogger().info(
            "LeviSmooth native core loaded"
        );
        return true;
    }

    bool enable() {
        getSelf().getLogger().info(
            "LeviSmooth enabled"
        );
        return true;
    }

    ll::mod::NativeMod& getSelf() const {
        return mSelf;
    }

private:
    ll::mod::NativeMod& mSelf;
};

PL_REGISTER_MOD(LeviSmooth, LeviSmooth::instance())
