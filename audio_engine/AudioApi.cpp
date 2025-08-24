#include "AudioApi.hpp"

void AudioApi::applyAudioApi(sol::state& lua){
    // lua.set_function("synth", [&](const std::string& name) {
    //     return sc_server.synth(name);
    // });

    // lua.set_function("synth_set", [&](node_id node, const char* name, int value) {
    //     sc_server.set(node, name, value);
    // });

    // lua.set_function("synth_set", [&](node_id node, const char* name, double value) {
    //     sc_server.set(node, name, value);
    // });
    lua.set_function("synth", [&](const char* name, const char* msg, sol::variadic_args args) {
        pde.synth(name, msg, args);
    });
}