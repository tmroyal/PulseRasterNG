#pragma once

#include <raylib.h>
#include <sol/sol.hpp>
#include "MidiManager.hpp"
#include "EventProcessor.hpp"

class ControllerApi {
public:
    ControllerApi(MidiManager& midiManager, EventProcessor& event_processor) : midiManager(midiManager), eventProcessor(event_processor) {}
    
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
            std::cout << "opening port " << name << std::endl;
            return midiManager.openInputPort(number, name);
        });
        lua.set_function("mouseUp", [this](int button, sol::protected_function handler) {
            eventProcessor.onMouseUp(button, handler);
        });
        lua.set_function("mouseDown", [this](int button, sol::protected_function handler){
            eventProcessor.onMouseDown(button, handler);
        });
        lua.set_function("mousePosition", []() -> std::pair<float, float> {
            Vector2 pos = GetMousePosition();
            return {pos.x, pos.y};
        });
    }
    
private:
    MidiManager& midiManager;
    EventProcessor& eventProcessor;
};