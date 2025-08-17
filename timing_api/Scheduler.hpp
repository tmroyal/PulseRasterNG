#pragma once

#include <queue>
#include <thread>
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
    void repeat(double interval_seconds, sol::function callback);
    void vary_repeat(double initial_delay_seconds, sol::function callback);
private:
    void push_task(Time t, std::function<void()> f);
    void worker_loop();

    std::priority_queue<Task> tasks;
    std::mutex task_mutex;
    std::condition_variable task_cv; 

    std::thread worker_thread;
    std::atomic<bool> running{false};
};