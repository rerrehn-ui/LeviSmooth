#include <pl/Mod.hpp>
#include <pl/ModMenu.hpp>

#include "FrameMonitor.hpp"
#include "core/FrameAnalyzer.hpp"
#include "core/CameraSmoother.hpp"
#include "core/FramePacer.hpp"

#include <string>
#include <string_view>
#include <vector>

class LeviSmooth {
public:
    static LeviSmooth& instance() {
        static LeviSmooth mod;
        return mod;
    }

    LeviSmooth()
        : mSelf(*ll::mod::NativeMod::current()) {}

    ll::mod::NativeMod& getSelf() const {
        return mSelf;
    }

    static LeviSmoothCore::FrameMonitor& frameMonitor() {
        static LeviSmoothCore::FrameMonitor monitor;
        return monitor;
    }

    static LeviSmoothCore::FrameAnalyzer& frameAnalyzer() {
        static LeviSmoothCore::FrameAnalyzer analyzer;
        return analyzer;
    }

    static LeviSmoothCore::CameraSmoother& cameraSmoother() {
        static LeviSmoothCore::CameraSmoother smoother;
        return smoother;
    }

    static LeviSmoothCore::FramePacer& framePacer() {
        static LeviSmoothCore::FramePacer pacer;
        return pacer;
    }

    bool load() {
        getSelf().getLogger().info(
            "LeviSmooth core systems loaded!"
        );
        return true;
    }

    bool enable() {
        return pl::modmenu::ModuleBuilder(
            "levi_smooth.camera",
            "LeviSmooth"
        )
            .modId(getSelf().getId())
            .description(
                "Micro-stutter analysis, frame pacing "
                "and camera smoothing framework"
            )
            .defaultEnabled(true)
            .onToggle(
                [](std::string_view, bool enabled) {
                    if (enabled) {
                        showStatus();
                    } else {
                        clearStatus();
                    }
                }
            )
            .registerModule();
    }

private:
    static void showStatus() {
        using namespace pl::modmenu;

        const std::vector<DrawCommand> commands = {
            {
                .type = DrawCommandType::RectFilled,
                .x = 8.0f,
                .y = 8.0f,
                .w = 310.0f,
                .h = 145.0f,
                .color = 0xAA000000
            },
            {
                .type = DrawCommandType::Text,
                .x = 18.0f,
                .y = 18.0f,
                .color = 0xFFFFFFFF,
                .size = 20.0f,
                .text = "LeviSmooth"
            },
            {
                .type = DrawCommandType::Text,
                .x = 18.0f,
                .y = 48.0f,
                .color = 0xFF00FF00,
                .size = 14.0f,
                .text = "Frame Monitor: Ready"
            },
            {
                .type = DrawCommandType::Text,
                .x = 18.0f,
                .y = 72.0f,
                .color = 0xFF00FF00,
                .size = 14.0f,
                .text = "Frame Analyzer: Ready"
            },
            {
                .type = DrawCommandType::Text,
                .x = 18.0f,
                .y = 96.0f,
                .color = 0xFF00FF00,
                .size = 14.0f,
                .text = "Camera Smoother: Ready"
            },
            {
                .type = DrawCommandType::Text,
                .x = 18.0f,
                .y = 120.0f,
                .color = 0xFFFFFF00,
                .size = 13.0f,
                .text = "Frame Pacer: Framework Ready"
            }
        };

        submitDrawCommands("levi_smooth.camera", commands);
    }

    static void clearStatus() {
        pl::modmenu::submitDrawCommands(
            "levi_smooth.camera", {}
        );
    }

    ll::mod::NativeMod& mSelf;
};

PL_REGISTER_MOD(LeviSmooth, LeviSmooth::instance())
