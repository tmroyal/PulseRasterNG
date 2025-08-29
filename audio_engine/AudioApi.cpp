#include "AudioApi.hpp"

void AudioApi::applyAudioApi(sol::state& lua, ScriptRunner& runner, pdEventQueue& eventQueue) {
    lua.set_function("load_synth", [&](const char* name) {
        return pde.load_synth(name);
    });

    lua.set_function("free_synth", [&](int handle){
        return pde.free_synth(handle);
    });

    lua.set_function("synth", [&](int handle, const char* msg, sol::variadic_args args) {
        pde.synth(handle, msg, args);
    });

    lua.set_function("audioEvent", [&](std::string name, sol::protected_function callback) {
        int token = runner.store_callback(callback);
        eventQueue.bind(name, token);
    });
}