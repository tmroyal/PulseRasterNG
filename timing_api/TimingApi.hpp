#pragma once

#import <sol/sol.hpp>
#import <chrono>

class TimingApi {
public:
    TimingApi() : start_time(std::chrono::steady_clock::now()) {}

    void applyTimingApi(sol::state& lua) {
        lua.set_function("timeSec", [this]() {
            auto now = std::chrono::steady_clock::now();
            return std::chrono::duration<double>(
                now - start_time
            ).count();
        });
    }
private:
    std::chrono::steady_clock::time_point start_time;
};