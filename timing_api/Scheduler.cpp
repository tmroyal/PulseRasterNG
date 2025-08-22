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
    Time target_time = std::chrono::time_point_cast<Clock::duration>(
           Clock::now() + Sec(seconds)
    );

    tasks.push(Task{target_time, [cb = std::move(callback)]() {
        cb();
    }});
}

void Scheduler::metro(double interval_seconds, sol::function callback) {
    const auto period = Sec(interval_seconds);
    Time next_time = std::chrono::time_point_cast<Clock::duration>(
        Clock::now() + period
    ); // start in the future


    auto thunk = std::make_shared<std::function<void()>>();
    *thunk = [this, cb = std::move(callback), thunk, period, next_time]() mutable {
        cb();
        next_time = std::chrono::time_point_cast<Clock::duration>(
            next_time + period
        );
        tasks.push(Task{next_time, [thunk]{ (*thunk)(); }});
    };
    tasks.push(Task{next_time, [thunk]{ (*thunk)(); }});
}

void Scheduler::vary_metro(double initial_delay_seconds, sol::function callback) {
    std::cout << "vary_metro is not implemented yet." << std::endl;
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

void Scheduler::consume(){
    if (!running) return;
    auto current_time = Clock::now();

    while (!tasks.empty() && tasks.top().time <= current_time) {
        Task task = tasks.top();
        tasks.pop();
        task.callback();
    }
}