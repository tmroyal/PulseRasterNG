#include <thread>
#include <chrono>
#include <atomic>
#include <cmath>
#include <lo/lo.h>
#include "raylib.h"
#include <iostream>

#include "audio_engine/SC_Server.hpp"
#include <sol/sol.hpp>

template <typename T>
T clamp(T v, T lo, T hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}


std::atomic<bool> running{true};
std::atomic<uint8_t> alpha{255};

#define NODE_ID 1000

void clockThread(SC_Server& sc, node_id node) {
    using clk = std::chrono::steady_clock;
    using namespace std::chrono;
    const double lfoHz = 0.5;
    const auto period = 20ms; // ~50 Hz publish rate
    auto t0 = clk::now();
    auto next = clk::now() + period;

    while (running.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_until(next); next += period;
        double t = duration<double>(clk::now() - t0).count();
        double s = std::sin(2.0 * M_PI * lfoHz * t) * 0.5 + 0.5; // 0..1
        sc.set(node, "amp", s);
        alpha.store((uint8_t)(s * 255.0), std::memory_order_relaxed);
    }
}

int main(){
    sol::state lua; lua.open_libraries(sol::lib::base);
    lua.set_function("add", [](int a,int b){return a+b;});
    std::cout << lua.script("return add(2,3)").get<int>() << "\n";

    SC_Server server("127.0.0.1", "57110");

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 800, "Alpha publisher");
    SetTargetFPS(60);

    // start synth (assumes SynthDef "bear" is loaded on scsynth)
    node_id node = server.synth("bear");

    // rect state
    Vector2 rectC = { GetScreenWidth()*0.5f, GetScreenHeight()*0.5f };
    const float moveSpeed = 300.0f;   // px/sec (arrows)
    const float stickScale = 400.0f;  // px/sec (analog)
    const float dead = 0.15f;

    std::thread th(clockThread, std::ref(server), node);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        int w = GetScreenWidth(), h = GetScreenHeight();
        int s = w/8;
        Vector2 winC = { w*0.5f, h*0.5f };

        // input vector
        Vector2 v{0,0};
        if (IsKeyDown(KEY_RIGHT)) v.x += 1;
        if (IsKeyDown(KEY_LEFT))  v.x -= 1;
        if (IsKeyDown(KEY_DOWN))  v.y += 1;
        if (IsKeyDown(KEY_UP))    v.y -= 1;

        if (IsGamepadAvailable(0)) {
            float gx = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
            float gy = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
            if (std::fabs(gx) > dead) v.x += gx * (stickScale/moveSpeed);
            if (std::fabs(gy) > dead) v.y += gy * (stickScale/moveSpeed);
        }

        if (v.x != 0 || v.y != 0) {
            float m = std::hypot(v.x, v.y);
            v.x /= m; v.y /= m;
            rectC.x += v.x * moveSpeed * dt;
            rectC.y += v.y * moveSpeed * dt;
        }

        // clamp inside window
        rectC.x = clamp(rectC.x, (float)s*0.5f, (float)w - s*0.5f);
        rectC.y = clamp(rectC.y, (float)s*0.5f, (float)h - s*0.5f);

        // distance -> freq (220..880 Hz)
        float dist = std::hypot(rectC.x - winC.x, rectC.y - winC.y);
        float maxd = std::hypot(winC.x, winC.y);
        float norm = (maxd > 0) ? clamp(dist / maxd, 0.0f, 1.0f) : 0.0f;
        float freq = 220.0f + norm * (880.0f - 220.0f);

        // send freq
        server.set(node, "freq", freq);

        // draw
        uint8_t a = alpha.load(std::memory_order_relaxed);
        BeginDrawing();
            ClearBackground({32,32,42,255});
            DrawText(TextFormat("freq: %.1f Hz", freq), 24, 24, 24, RAYWHITE);
            DrawRectangleLines((int)(rectC.x - s/2), (int)(rectC.y - s/2), s, s, {255,0,0,a});
        EndDrawing();
    }

    running.store(false, std::memory_order_relaxed);
    th.join();
    CloseWindow();
    return 0;
}
