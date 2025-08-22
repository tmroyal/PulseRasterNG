#pragma once

#include <queue>
#include <sol/sol.hpp>

using Clock = std::chrono::steady_clock;
using Time  = Clock::time_point;
using Sec = std::chrono::duration<double>;

struct Task {
    Time time;
    // sol::function callback;
    std::function <void()> callback;

    bool operator<(const Task& other) const {
        // later time has lower priority
        return time > other.time;
    }
};

class Scheduler {
public:
    Scheduler() = default;
    ~Scheduler(){
        stop();
    };

    void start();
    void stop();

    void timer(double seconds, sol::function callback);
    void metro(double interval_seconds, sol::function callback);
    void vary_metro(double initial_delay_seconds, sol::function callback);

    void consume();

    void applySchedulerApi(sol::state& lua) {
        lua.set_function("timer", [&](double seconds, sol::function callback) {
            timer(seconds, callback);
        });

        lua.set_function("metro", [&](double interval_seconds, sol::function callback) {
            metro(interval_seconds, callback);
        });

        lua.set_function("vary_metro", [&](double initial_delay_seconds, sol::function callback) {
            vary_metro(initial_delay_seconds, callback);
        });
    }
private:
    std::priority_queue<Task> tasks;
    bool running;
};