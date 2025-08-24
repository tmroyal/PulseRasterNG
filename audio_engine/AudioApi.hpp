#pragma once

#include <sol/sol.hpp>
#include "pdEngine.hpp"

class AudioApi {
public:
    AudioApi(pdEngine& pd) : pde(pd) {}
    void applyAudioApi(sol::state& lua);
private:
    pdEngine& pde;
};