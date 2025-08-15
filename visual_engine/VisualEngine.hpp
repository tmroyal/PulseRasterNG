#pragma once

#include "../script_runner/ScriptRunner.h"
#include "GraphicsApi.hpp"

class VisualEngine {
public:
    explicit VisualEngine(ScriptRunner& runner);
    void draw();
private:
    ScriptRunner& script_runner;
    GraphicsApi graphics_api;
};