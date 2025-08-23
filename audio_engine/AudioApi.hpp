#pragma once

#include <sol/sol.hpp>
#include "pdEngine.hpp"

class AudioApi {
public:
    AudioApi(pdEngine& pd) : pdEngine(pd) {}
    void applyAudioApi(sol::state& lua);
private:
    pdEngine& pdEngine;
};