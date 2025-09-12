#pragma once

#include <raylib.h>
#include <sol/sol.hpp>
#include "MidiManager.hpp"

class ControllerApi {
public:
    ControllerApi(MidiManager& midiManager) : midiManager(midiManager) {}
    
    void attach(sol::state& lua) {
        lua.set_function("getAxis", [](int gamepad, int axis) {
            if (!IsGamepadAvailable(gamepad)) {
                return 0.0f;
            }
            return GetGamepadAxisMovement(gamepad, axis);   
        });
        lua.set_function("isButtonDown", [](int gamepad, int button) {
            if (!IsGamepadAvailable(gamepad)) {
                return false;
            }
            return IsGamepadButtonDown(gamepad, button);
        });
        lua.set_function("openPort", [this](int number, const std::string& name) -> bool {
            return midiManager.openInputPort(number, name);
        });
    }
    
private:
    MidiManager& midiManager;
};