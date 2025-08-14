#include "ScriptRunner.h"

#include "../visual_engine/GraphicsApi.hpp"

ScriptRunner::ScriptRunner(){
    lua.open_libraries(sol::lib::base, sol::lib::math);
}

void ScriptRunner::load_script(const char* path){
    sol::environment env(lua, sol::create, lua.globals());

    try {
        lua.script_file(path, env);
    } 
    catch (const sol::error& e) {
        std::cerr << "Error loading script file: " << e.what() << "\n";
        return;
    }

    if (
        env["draw"].valid()
    ){
        environments.push_back(env);
    } else {
        std::cerr << "All scripts require the following functions: draw" << "\n";
    }
    scripts_loaded = true;

    GraphicsApi::applyGraphicsApi(lua);
}

void ScriptRunner::draw(){
    if (scripts_loaded){
        size_t index = std::max((size_t) 0, std::min(currentDrawScript, environments.size()));
        environments[index]["draw"]();
    }
}