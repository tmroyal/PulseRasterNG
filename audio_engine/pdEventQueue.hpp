#pragma once

#include "readerwriterqueue.h"
#include "z_libpd.h"
#include "../script_runner/ScriptRunner.h"

typedef int luaCbToken;

class pdEventQueue {
public:
    pdEventQueue(ScriptRunner& runner) : runner(runner) {
        if (instance){
            throw std::runtime_error("Only one pdEventQueue instance allowed");
        }
        instance = this;
        libpd_set_banghook(onBang);
    }
    ~pdEventQueue(){
        instance = nullptr;
    }
    void bind(std::string name, luaCbToken token) {
        if (event_map.find(name) == event_map.end()) {
            libpd_bind(name.c_str());
        }
        event_map[name].push_back(token);
    }
    void drain(){
        luaCbToken token;
        while (queue.try_dequeue(token)) {
            runner.run_callback(token);
        }
    }
private:
    inline static pdEventQueue* instance = nullptr;
    static void onBang(const char* recv){
        if (instance->event_map.find(recv) != instance->event_map.end()) {
            for (luaCbToken token : instance->event_map[recv]) {
                instance->queue.enqueue(token);
            }
        }
    }
    moodycamel::ReaderWriterQueue<luaCbToken> queue{1024};
    std::pmr::unordered_map<std::string, std::vector<luaCbToken>> event_map;
    ScriptRunner& runner;
};