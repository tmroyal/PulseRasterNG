#include "VisualEngine.hpp"
#include "raylib.h"

VisualEngine::VisualEngine(ScriptRunner& runner, TimingApi& timing_api)
    : script_runner(runner), timing_api(timing_api)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 800, "PulseRasterNG");
    SetTargetFPS(0);
    ClearWindowState(FLAG_VSYNC_HINT);

    graphics_api.applyGraphicsApi(runner.lua);
}


void VisualEngine::draw() {
    timing_api.updateFrameDelta();
    BeginDrawing();
        ClearBackground(graphics_api.getBackgroundColor());
        script_runner.draw();
    EndDrawing();
}