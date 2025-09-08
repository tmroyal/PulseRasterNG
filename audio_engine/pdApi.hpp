#pragma once

#include "../script_runner/ScriptRunner.h"
#include "pdEngine.hpp"
#include "pdEventQueue.hpp"
#include <sol/sol.hpp>

class Patch {
public:
  Patch(std::string name, sol::table args, pdEngine &pd): pd(pd) {
    init(name, pd);
    pd.msg_init(handle, args);
  }
  Patch(std::string name, pdEngine &pd): pd(pd) {
    init(name, pd);
  }
  ~Patch(){
    pd.free_patch(handle);
  }
  Patch& msg(const char *msg, sol::variadic_args args) {
    pd.msg(handle, msg, args);
    return *this;
  }
  int id(){
    return handle;
  }

private:
  int handle;
  pdEngine& pd;
  void init(std::string name, pdEngine &pd){
    handle = pd.load_patch(name.c_str());
  }
};

class pdApi {
public:
  pdApi(pdEngine &pd) : pde(pd) {}
  void applyPDApi(sol::state &lua, ScriptRunner &runner,
                  pdEventQueue &eventQueue);

private:
  pdEngine &pde;
};