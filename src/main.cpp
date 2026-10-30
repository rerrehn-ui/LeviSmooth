#include <pl/Mod.hpp>
#include <pl/ModMenu.hpp>

#include <string>
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

    bool load() {
        getSelf().getLogger().info("LeviSmooth loaded!");
        return true;
    }

    bool enable() {
        return pl::modmenu::ModuleBuilder(
            "levi_smooth.camera",
            "LeviSmooth"
        )
            .modId(getSelf().getId())
            .description(
                "Camera smoothing and frame pacing framework"
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
                .w = 300.0f,
                .h = 110.0f,
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
                .color = 0xFFFFFFFF,
                .size = 15.0f,
                .text = "Camera Smoothing: Pending"
            },
            {
                .type = DrawCommandType::Text,
                .x = 18.0f,
                .y = 72.0f,
                .color = 0xFFFFFFFF,
                .size = 15.0f,
                .text = "Frame Pacing: Pending"
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
