#include "pdApi.hpp"
#include "raylib.h"

#include <sol/sol.hpp>
#include "script_runner/ScriptRunner.h"
#include "timing_api/Scheduler.hpp"
#include "visual_engine/VisualEngine.hpp"
#include "timing_api/TimingApi.hpp"
#include "controller_engine/ControllerApi.hpp"

int main(int argc, char* argv[]){
    std::string pd_dir;
    std::string lua_dir;    

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--pdscript" && i + 1 < argc) {
            pd_dir = argv[++i];
        } else if (arg == "--luascript" && i + 1 < argc) {
            lua_dir = argv[++i];
        }
    }
    if (pd_dir.empty()){
        pd_dir = "./puredata";
    }
    if (lua_dir.empty()){
        lua_dir = "./scripts";
    }

    ScriptRunner runner;

    // setup timing and visuals
    TimingApi timing_api;
    timing_api.applyTimingApi(runner.lua);

    VisualEngine ve(runner);

    // Setup audio engine
    static pdEngine pd;
    pdEventQueue pdq(runner);
    pdApi pd_api(pd);
    pd_api.applyPDApi(runner.lua, runner, pdq);

    pd.init(pd_dir);

    if (!pd.is_initialized()){
        CloseWindow();
        std::cerr << "pd failed to initialize" << std::endl;
        return 1;
    }

    AudioStream s;
    InitAudioDevice();
    s = LoadAudioStream(SR, 32, N_CHANNELS);
    SetAudioStreamCallback(s, [](void * buffer, unsigned int frames){
        pd.process((float*)buffer, (float*)buffer, frames);
    });
    PlayAudioStream(s);

    // Setup scheduler

    Scheduler scheduler;
    scheduler.applySchedulerApi(runner.lua);
    scheduler.start();

    ControllerApi controller_api;
    controller_api.attach(runner.lua);

    runner.init(lua_dir);
    runner.inc();

    SetTargetFPS(0) ;
    double desired_fps = 60.0;
    double frame_duration = 1.0 / desired_fps;
    double init_time = GetTime();
    double next_time = init_time + frame_duration;

    while (!WindowShouldClose()) {
        // sleep until the next frame
        if (GetTime() >= next_time) {
            ve.draw();
            next_time += frame_duration;
        }
        scheduler.consume();
        pdq.drain();
        WaitTime(0.001);
    }

    scheduler.stop();
    if (pd.is_initialized()){
        UnloadAudioStream(s);
    }

    CloseWindow();

    return 0;
}
