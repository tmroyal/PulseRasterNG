#pragma once

#include <sol/sol.hpp>
#include "pdEngine.hpp"
#include "../script_runner/ScriptRunner.h"
#include "pdEventQueue.hpp"

struct Patch {
    int handle;
    Patch(std::string name, sol::table args, pdEngine& pd){

        for (auto& kv : args){
            sol::object key = kv.first;
            sol::object value = kv.second;    
        
            if (!key.is<std::string>()){
                std::cout << "Warning: non-string detected in table: " << name;
                continue;
            }

        }
    }
    Patch(std::string name, pdEngine& pd){
        handle = pd.load_patch(name.c_str());
    }
};

class AudioApi {
public:
    AudioApi(pdEngine& pd) : pde(pd) {}
    void applyAudioApi(sol::state& lua, ScriptRunner& runner, pdEventQueue& eventQueue);
private:
    pdEngine& pde;
};