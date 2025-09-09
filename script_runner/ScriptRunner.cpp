#include "ScriptRunner.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ScriptRunner::ScriptRunner(){
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);

    // TODO: iterate and store file_name -> FileName
    sol::table p = lua.require_file("patch", "prng_lua_lib/patch.lua");
    lua["Patch"] = p;
}

void ScriptRunner::init(std::string dir){
    if (!fs::exists(dir ) || !fs::is_directory(dir)){
        std::cerr << "Error: directory does not exist: " << dir << std::endl;
        return;
    }
    for (auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".lua") {
            load_script(entry.path().string().c_str());
        }
    }
    for (auto env : environments){
        if (is_sol_function("init", env)){
            env["init"]();
        }
        if (is_sol_function("config", env)){
            env["config"]();
        }
    }
    lua.set_function("inc", [&](){
        inc();
    });
}

void ScriptRunner::inc(){
    if (draw_scripts_loaded){
        currentDrawScript = (currentDrawScript + 1) % environments.size();
    }
}

void ScriptRunner::run_callback(pdArg arg){
    if (callbacks.find(arg.token) == callbacks.end()) {
        return;
    }
    sol::protected_function cb = callbacks[arg.token];

    switch (arg.type){
        case BANG:
            cb();
            break;
        case FLOAT:
            cb(arg.flt);
            break;
        case CHAR:
            cb(arg.str);
            break;
    }
}

int ScriptRunner::store_callback(sol::protected_function cb){
    int token = current_callback_token++;
    callbacks[token] = std::move(cb);
    return token;
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
    std::filesystem::path p = std::filesystem::absolute(path); 

    std::string dir = p.parent_path().string();

    if (!dir.empty() && dir.back() != '/'){
        dir.push_back('/');
    }
    env["script_dir"] = dir;

    if (is_sol_function("draw", env)){
        environments.push_back(env);
        draw_scripts_loaded = true;
    } else if (is_sol_function("config", env)){
        // don't store env if we have no need to access draw
        env["config"]();
    } else {
        std::cerr << "All scripts require one of the following functions: draw, config" << "\n";
    }
}

void ScriptRunner::draw(){
    if (draw_scripts_loaded){
        size_t index = std::max((size_t) 0, std::min(currentDrawScript, environments.size()-1));
        sol::environment& env = environments[index];
        env["draw"]();
    }
}

bool ScriptRunner::is_sol_function(std::string key, sol::environment env){
    return env[key].valid() && env[key].is<sol::function>();
}