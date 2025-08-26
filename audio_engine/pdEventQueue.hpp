#pragma once

#include "readerwriterqueue.h"
#include "pdArg.hpp"
#include "z_libpd.h"
#include "../script_runner/ScriptRunner.h"

class pdEventQueue {
public:
    pdEventQueue(ScriptRunner& runner) : runner(runner) {
        if (instance){
            throw std::runtime_error("Only one pdEventQueue instance allowed");
        }
        instance = this;
        libpd_set_banghook(onBang);
        libpd_set_floathook(onFloat);
        libpd_set_symbolhook(onSymbol); 
    }
    ~pdEventQueue(){
        instance = nullptr;
    }
    void bind(std::string name, int token) {
        if (event_map.find(name) == event_map.end()) {
            libpd_bind(name.c_str());
        }
        event_map[name].push_back(token);
    }
    void drain(){
        pdArg arg;
        while (queue.try_dequeue(arg)) {
            runner.run_callback(arg);
        }
    }
private:
    inline static pdEventQueue* instance = nullptr;
    static void onBang(const char* recv){
        if (instance->event_map.find(recv) != instance->event_map.end()) {
            for (int token : instance->event_map[recv]) {
                pdArg arg;
                arg.type = BANG;
                arg.token = token;
                instance->queue.enqueue(arg);
            }
        }
    }
    static void onFloat(const char* recv, float input){
        if (instance->event_map.find(recv) != instance->event_map.end()) {
            for (int token : instance->event_map[recv]) {
                pdArg arg;
                arg.type = FLOAT;
                arg.token = token;
                arg.flt = input;
                instance->queue.enqueue(arg);
            }
        }
    }
    static void onSymbol(const char* recv, const char* sym){
        if (instance->event_map.find(recv) != instance->event_map.end()) {
            for (int token : instance->event_map[recv]) {
                pdArg arg;
                arg.type = CHAR;
                arg.token = token;
                arg.str = std::string(sym);
                instance->queue.enqueue(arg);
            }
        }
    }
    moodycamel::ReaderWriterQueue<pdArg> queue{1024};
    std::pmr::unordered_map<std::string, std::vector<int>> event_map;
    ScriptRunner& runner;
};