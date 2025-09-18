#pragma once

#include <sol/sol.hpp>
#include <chrono>

class TimingApi {
public:
    TimingApi() : start_time(std::chrono::steady_clock::now()), last_draw_time(std::chrono::steady_clock::now()), frame_delta(0.0) {}

    void applyTimingApi(sol::state& lua) {
        lua.set_function("timeSec", [this]() {
            auto now = std::chrono::steady_clock::now();
            return std::chrono::duration<double>(
                now - start_time
            ).count();
        });

        lua.set_function("dt", [this]() {
            return frame_delta;
        });
    }

    void updateFrameDelta() {
        auto now = std::chrono::steady_clock::now();
        frame_delta = std::chrono::duration<double>(
            now - last_draw_time
        ).count();
        last_draw_time = now;
    }

private:
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point last_draw_time;
    double frame_delta;
};