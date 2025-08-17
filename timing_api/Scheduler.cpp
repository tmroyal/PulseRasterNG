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
        {
            std::lock_guard<std::mutex> lg(task_mutex);
            tasks.push(Task{current_time, *thunk});
        }
        task_cv.notify_one();
    };

    {
        std::lock_guard<std::mutex> lg(task_mutex);
        tasks.push(Task{
            current_time,
            *thunk
        });
    }
    task_cv.notify_one();
}

void Scheduler::vary_repeat(double initial_delay_seconds, sol::function callback) {
    Time current_time = Clock::now();
    auto initial_delay = Sec(initial_delay_seconds);

    auto thunk = std::make_shared<std::function<void()>>();    
    *thunk = [this, cb = std::move(callback), thunk, current_time]() mutable {
        double next_interval = cb();
        Time next_time = std::chrono::time_point_cast<Clock::duration>(
            Clock::now() + Sec(next_interval)
        );
        {
            std::lock_guard<std::mutex> lg(task_mutex);
            tasks.push(Task{next_time, *thunk});
        }
        task_cv.notify_one();
    };

    {
        std::lock_guard<std::mutex> lg(task_mutex);
        tasks.push(Task{
            std::chrono::time_point_cast<Clock::duration>(current_time + initial_delay),
            *thunk
        });
    }
    task_cv.notify_one();
}