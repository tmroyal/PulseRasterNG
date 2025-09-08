#include "pdApi.hpp"

void pdApi::applyPDApi(sol::state& lua, ScriptRunner& runner, pdEventQueue& eventQueue) {
    lua.set_function("load_patch", [&](const char* name) {
        return pde.load_patch(name);
    });

    lua.set_function("free_patch", [&](int handle){
        return pde.free_patch(handle);
    });

    lua.set_function("pd_msg", [&](int handle, const char* msg, sol::variadic_args args) {
        pde.msg(handle, msg, args);
    });

    lua.set_function("patchEvent", [&](std::string name, sol::protected_function callback) {
        int token = runner.store_callback(callback);
        eventQueue.bind(name, token);
    });
}