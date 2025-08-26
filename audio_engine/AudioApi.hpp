#pragma once

#include <sol/sol.hpp>
#include "pdEngine.hpp"
#include "../script_runner/ScriptRunner.h"
#include "pdEventQueue.hpp"

class AudioApi {
public:
    AudioApi(pdEngine& pd) : pde(pd) {}
    void applyAudioApi(sol::state& lua, ScriptRunner& runner, pdEventQueue& eventQueue);
private:
    pdEngine& pde;
};