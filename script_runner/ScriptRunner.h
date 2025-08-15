#pragma once

#include <sol/sol.hpp>

class ScriptRunner{
public:
    ScriptRunner();
    void load_script(const char* path);
    void draw();
    void inc();
    sol::state lua;
private:
    std::vector<sol::environment> environments;
    size_t currentDrawScript;
    bool scripts_loaded = false;
};