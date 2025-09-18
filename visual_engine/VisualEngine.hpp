#pragma once

#include "../script_runner/ScriptRunner.h"
#include "../timing_api/TimingApi.hpp"
#include "GraphicsApi.hpp"

class VisualEngine {
public:
    explicit VisualEngine(ScriptRunner& runner, TimingApi& timing_api);
    void draw();
private:
    ScriptRunner& script_runner;
    TimingApi& timing_api;
    GraphicsApi graphics_api;
    double time = 0.0;
};