#include "Scheduler.hpp"


void Scheduler::start() {
    if (running.exchange(true)) {
        return; // Already running
    }
    worker_thread = std::thread(&Scheduler::worker_loop, this);
}

void Scheduler::stop() {
    if (!running.exchange(false)) {
        return; // Already stopped
    }
    {
        std::lock_guard<std::mutex> lg(task_mutex);
        while (!tasks.empty()) {
            tasks.pop();
        }
    }
    task_cv.notify_all(); // Wake up the worker thread if it's waiting
    if (worker_thread.joinable()) {
        worker_thread.join();
    }
}

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


void Scheduler::worker_loop() {
    std::unique_lock<std::mutex> ul(task_mutex);
    while (running) {
        task_cv.wait(ul, [this] { return !running || !tasks.empty(); });
        if (!running) break;
    
        Task job = tasks.top();
        auto deadline = job.time;
        auto pre = deadline - std::chrono::microseconds(500);        

        if (Clock::now() < deadline) {
            // Wait until the task's scheduled time or until a new task is added/stop signal
            task_cv.wait_until(ul, pre, [this, deadline] {
                return !running || tasks.empty() || tasks.top().time < deadline;
            });
            // if stopped while waiting, abort
            if (!running) break;
            // if tasks have been cleared of if the top task changed, re-evaluate
            if (tasks.empty() || tasks.top().time > deadline) {
                continue;
            }
        }

        tasks.pop();
        auto fn = std::move(job.callback);
        ul.unlock();
        while (Clock::now() < deadline){}
        try {
            fn();
        } catch (const std::exception& e) {
            // Log the error, but keep the scheduler running
            std::cerr << "Error in scheduled task: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error in scheduled task." << std::endl;
        }
        ul.lock();
    }
}