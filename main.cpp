#include "AudioApi.hpp"
#include "raylib.h"

#include <sol/sol.hpp>
#include "script_runner/ScriptRunner.h"
#include "timing_api/Scheduler.hpp"
#include "visual_engine/VisualEngine.hpp"
#include "timing_api/TimingApi.hpp"

int main(){
    ScriptRunner runner;

    // setup timing and visuals
    TimingApi timing_api;
    timing_api.applyTimingApi(runner.lua);

    VisualEngine ve(runner);

    // Setup audio engine
    static pdEngine pd;
    AudioApi audio_api(pd);
    audio_api.applyAudioApi(runner.lua);

    pd.init();

    InitAudioDevice();
    AudioStream s = LoadAudioStream(SR, 32, N_CHANNELS);
    SetAudioStreamCallback(s, [](void * buffer, unsigned int frames){
        pd.process((float*)buffer, (float*)buffer, frames);
    });
    PlayAudioStream(s);

    // Setup scheduler

    Scheduler scheduler;
    scheduler.applySchedulerApi(runner.lua);
    scheduler.start();

    runner.load_script("scripts/wiggle_ball.lua");
    runner.load_script("scripts/twirling_cube.lua");
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
        WaitTime(0.001);
    }

    scheduler.stop();
    UnloadAudioStream(s);

    CloseWindow();

    return 0;
}
