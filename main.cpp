#include <lo/lo.h>
#include "AudioApi.hpp"
#include "raylib.h"

#include "audio_engine/SC_Server.hpp"
#include <sol/sol.hpp>
#include "script_runner/ScriptRunner.h"
#include "timing_api/Scheduler.hpp"
#include "visual_engine/VisualEngine.hpp"
#include "timing_api/TimingApi.hpp"

int main(){
    ScriptRunner runner;

    TimingApi timing_api;
    timing_api.applyTimingApi(runner.lua);

    VisualEngine ve(runner);

    SC_Server server("127.0.0.1", "57110");
    AudioApi audio_api(server);
    audio_api.applyAudioApi(runner.lua);

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

    CloseWindow();

    return 0;
}
