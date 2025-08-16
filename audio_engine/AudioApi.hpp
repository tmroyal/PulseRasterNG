#pragma once

#include <sol/sol.hpp>
#include "SC_Server.hpp"

class AudioApi {
public:
    AudioApi(SC_Server& server) : sc_server(server) {}
    void applyAudioApi(sol::state& lua);
private:
    SC_Server& sc_server;
};