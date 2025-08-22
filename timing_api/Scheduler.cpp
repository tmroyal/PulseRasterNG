#include "Scheduler.hpp"


void Scheduler::start() {
    running = true;
}

void Scheduler::stop() {
    if (running) {
        while (!tasks.empty()) {
            tasks.pop();
        }
    }
    running = false;
}

void Scheduler::timer(double seconds, sol::function callback) {
    // Time target_time = std::chrono::time_point_cast<Clock::duration>(
    //        Clock::now() + Sec(seconds)
    // );

    // std::lock_guard<std::mutex> lock_guard(task_mutex);
    // tasks.push(Task{target_time, [callback]() {
    //     // Call the Lua callback with the elapsed time
    //     callback();
    // }});
    // task_cv.notify_one(); 
}

void Scheduler::metro(double interval_seconds, sol::function callback) {
    // const auto period = Sec(interval_seconds);
    // Time next_time = std::chrono::time_point_cast<Clock::duration>(
    //     Clock::now() + period
    // ); // start in the future

    // auto thunk = std::make_shared<std::function<void()>>();
    // *thunk = [this, cb = std::move(callback), thunk, period, next_time]() mutable {
    //     cb();
    //     next_time = std::chrono::time_point_cast<Clock::duration>(
    //         next_time + period
    //     );
    //     push_task(next_time, [thunk]{ (*thunk)(); } );
    // };

    // push_task(next_time, [thunk]{ (*thunk)(); } );
}

void Scheduler::vary_metro(double initial_delay_seconds, sol::function callback) {
    // Time current_time = Clock::now();
    // auto initial_delay = Sec(initial_delay_seconds);
    // auto next_time = std::chrono::time_point_cast<Clock::duration>(
    //     current_time + initial_delay
    // );

    // auto thunk = std::make_shared<std::function<void()>>();    
    // *thunk = [this, cb = std::move(callback), thunk, next_time]() mutable {
    //     double cb_return = cb();
    //     next_time = std::chrono::time_point_cast<Clock::duration>(
    //         next_time + Sec(cb_return)
    //     );
    //     push_task(next_time, [thunk]{ (*thunk)(); } );
    // };

    // push_task(next_time, [thunk]{ (*thunk)(); } );
}

void Scheduler::push_task(Time t, sol::function f){
    tasks.push(Task{t, f});
}
