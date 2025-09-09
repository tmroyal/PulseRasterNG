#pragma once

#include "../script_runner/ScriptRunner.h"
#include "pdEngine.hpp"
#include "pdEventQueue.hpp"
#include <sol/sol.hpp>

class pdApi {
public:
  pdApi(pdEngine &pd) : pde(pd) {}
  void applyPDApi(sol::state &lua, ScriptRunner &runner,
                  pdEventQueue &eventQueue);

private:
  pdEngine &pde;
};