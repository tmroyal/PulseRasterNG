#include <lo/lo.h>
#include "raylib.h"

// #include "audio_engine/SC_Server.hpp"
#include <sol/sol.hpp>
#include "script_runner/ScriptRunner.h"
#include "visual_engine/VisualEngine.hpp"
#include "timing_api/TimingApi.hpp"

// template <typename T>
// T clamp(T v, T lo, T hi) {
//     return (v < lo) ? lo : (v > hi) ? hi : v;
// }


// std::atomic<bool> running{true};
// std::atomic<uint8_t> alpha{255};

// #define NODE_ID 1000

// void clockThread(SC_Server& sc, node_id node) {
//     using clk = std::chrono::steady_clock;
//     using namespace std::chrono;
//     const double lfoHz = 0.5;
//     const auto period = 20ms; // ~50 Hz publish rate
//     auto t0 = clk::now();
//     auto next = clk::now() + period;

//     while (running.load(std::memory_order_relaxed)) {
//         std::this_thread::sleep_until(next); next += period;
//         double t = duration<double>(clk::now() - t0).count();
//         double s = std::sin(2.0 * M_PI * lfoHz * t) * 0.5 + 0.5; // 0..1
//         sc.set(node, "amp", s);
//         alpha.store((uint8_t)(s * 255.0), std::memory_order_relaxed);
//     }
// }

int main(){
    // SC_Server server("127.0.0.1", "57110");

    // start synth (assumes SynthDef "bear" is loaded on scsynth)
    // node_id node = server.synth("bear");

    // rect state
    // Vector2 rectC = { GetScreenWidth()*0.5f, GetScreenHeight()*0.5f };
    // const float moveSpeed = 300.0f;   // px/sec (arrows)
    // const float stickScale = 400.0f;  // px/sec (analog)
    // const float dead = 0.15f;

    // std::thread th(clockThread, std::ref(server), node);

    ScriptRunner runner;
    runner.load_script("scripts/wiggle_ball.lua");
    runner.load_script("scripts/twirling_cube.lua");
    runner.inc();

    TimingApi timing_api;
    timing_api.applyTimingApi(runner.lua);

    VisualEngine ve(runner);

    while (!WindowShouldClose()) {
        ve.draw();
    }

    CloseWindow();

    // running.store(false, std::memory_order_relaxed);
    // th.join();
    // seWindow();
    return 0;
}
