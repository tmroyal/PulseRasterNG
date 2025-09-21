#pragma once

#include <raylib.h>
#include <sol/sol.hpp>


class EventProcessor {
public:
    void process();
    void onMouseDown(int button, sol::protected_function handler);
    void onMouseUp(int button, sol::protected_function handler);
private:
    std::unordered_map<int, std::vector<sol::protected_function>> mouseDownHandlers;
    std::unordered_map<int, std::vector<sol::protected_function>> mouseUpHandlers;
    void runHandlers(const std::unordered_map<int, std::vector<sol::protected_function>>& handlersMap, int button);
};