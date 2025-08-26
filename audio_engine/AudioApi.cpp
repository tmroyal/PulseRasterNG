#include "AudioApi.hpp"

void AudioApi::applyAudioApi(sol::state& lua, ScriptRunner& runner, pdEventQueue& eventQueue) {
    lua.set_function("synth", [&](const char* name, const char* msg, sol::variadic_args args) {
        pde.synth(name, msg, args);
    });

    lua.set_function("audioEvent", [&](std::string name, sol::protected_function callback) {
        int token = runner.store_callback(callback);
        eventQueue.bind(name, token);
    });
}