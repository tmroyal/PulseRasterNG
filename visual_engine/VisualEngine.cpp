#include "VisualEngine.hpp"
#include "raylib.h"

VisualEngine::VisualEngine(ScriptRunner& runner) 
    : script_runner(runner)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 800, "PulseRasterNG");
    SetTargetFPS(60);

    graphics_api.applyGraphicsApi(runner.lua);
}


void VisualEngine::draw() {
    BeginDrawing();
        ClearBackground(graphics_api.getBackgroundColor());
        script_runner.draw();
    EndDrawing();
}