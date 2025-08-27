#pragma once

#include <sol/sol.hpp>
#include "pdArg.hpp"

class ScriptRunner{
public:
    ScriptRunner();
    void load_script(const char* path);
    void draw();
    void inc();
    void run_callback(pdArg arg);
    void init(std::string dir);
    int store_callback(sol::protected_function cb);
    sol::state lua;
private:
    std::vector<sol::environment> environments;
    std::unordered_map<int, sol::protected_function> callbacks;
    int current_callback_token = 0;
    size_t currentDrawScript;
    bool draw_scripts_loaded = false;
    bool is_sol_function(std::string key, sol::environment env);
};