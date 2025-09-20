#include "pdApi.hpp"
#include "raylib.h"

#include <sol/sol.hpp>
#include "script_runner/ScriptRunner.h"
#include "timing_api/Scheduler.hpp"
#include "visual_engine/VisualEngine.hpp"
#include "timing_api/TimingApi.hpp"
#include "controller_engine/ControllerApi.hpp"
#include "controller_engine/MidiManager.hpp"
#include "audio_engine/pdMidi.hpp"
#include <iostream>

int main(int argc, char* argv[]){
    std::string pd_dir;
    std::string lua_dir;    

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--pdscript" && i + 1 < argc) {
            pd_dir = argv[++i];
        } else if (arg == "--luascript" && i + 1 < argc) {
            lua_dir = argv[++i];
        } else if (arg == "--list-ports") {
            MidiManager::listPorts();
            return 0;
        }
    }
    if (lua_dir.empty()){
        lua_dir = "./scripts";
    }

    ScriptRunner runner;

    // setup timing and visuals
    TimingApi timing_api;
    timing_api.applyTimingApi(runner.lua);

    VisualEngine ve(runner, timing_api);

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

    // Setup MIDI (after pd.init() so libpd is initialized)
    pdMidi pd_midi;
    MidiManager midi_manager(pd_midi);
    
    ControllerApi controller_api(midi_manager);
    controller_api.attach(runner.lua);

    // WaitTime(1);
    runner.init(lua_dir);
    runner.inc();

    SetTargetFPS(0) ;
    double desired_fps = 60.0;
    double frame_duration = 1.0 / desired_fps;
    double init_time = GetTime();
    double next_time = init_time + frame_duration;

    int gc_i = 0;
    while (!WindowShouldClose()) {
        // sleep until the next frame
        PollInputEvents();
        if (GetTime() >= next_time) {
            ve.draw();
            SwapScreenBuffer();
            next_time += frame_duration;
            // manual GC every 1024 frames
            if (++gc_i > 1024){
                runner.gc();
                gc_i = 0;
            }
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
