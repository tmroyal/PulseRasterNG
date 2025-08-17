#include "Scheduler.hpp"

void Scheduler::timer(double seconds, sol::function callback) {
    Time target_time = std::chrono::time_point_cast<Clock::duration>(
           Clock::now() + Sec(seconds)
    );

    std::lock_guard<std::mutex> lock_guard(task_mutex);
    tasks.push(Task{target_time, [callback]() {
        // Call the Lua callback with the elapsed time
        callback();
    }});
    task_cv.notify_one(); 
}

void Scheduler::repeat(double interval_seconds, sol::function callback) {
    Time current_time = Clock::now();
    auto period = Sec(interval_seconds);

    auto thunk = std::make_shared<std::function<void()>>();    
    *thunk = [this, cb = std::move(callback), thunk, current_time, period]() mutable {
        cb();
        // Reschedule the task
        current_time = std::chrono::time_point_cast<Clock::duration>(
            current_time + period
        );
        push_task(current_time, *thunk);
    };

    push_task(current_time, *thunk);
}

void Scheduler::vary_repeat(double initial_delay_seconds, sol::function callback) {
    Time current_time = Clock::now();
    auto initial_delay = Sec(initial_delay_seconds);
    auto next_time = std::chrono::time_point_cast<Clock::duration>(
        current_time + initial_delay
    );

    auto thunk = std::make_shared<std::function<void()>>();    
    *thunk = [this, cb = std::move(callback), thunk, next_time]() mutable {
        double cb_return = cb();
        next_time = std::chrono::time_point_cast<Clock::duration>(
            next_time + Sec(cb_return)
        );
        push_task(next_time, *thunk);
    };

    push_task(next_time, *thunk);
}

void Scheduler::push_task(Time t, std::function<void()> f) {
    std::lock_guard<std::mutex> lg(task_mutex);
    tasks.push(Task{t, std::move(f)});
    task_cv.notify_one();
}
