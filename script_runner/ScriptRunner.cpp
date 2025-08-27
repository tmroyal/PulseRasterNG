#include "ScriptRunner.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ScriptRunner::ScriptRunner(){
    lua.open_libraries(sol::lib::base, sol::lib::math);

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
        if (env["init"].valid() && env["init"].is<sol::function>()){
            env["init"]();
        }
    }
}

void ScriptRunner::inc(){
    if (scripts_loaded){
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
    std::cout << token << std::endl;
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

    if (
        env["draw"].valid() && env["draw"].is<sol::function>()
    ){
        environments.push_back(env);
    } else {
        std::cerr << "All scripts require the following functions: draw" << "\n";
    }
    scripts_loaded = true;
}

void ScriptRunner::draw(){
    if (scripts_loaded){
        size_t index = std::max((size_t) 0, std::min(currentDrawScript, environments.size()-1));
        sol::environment& env = environments[index];
        env["draw"]();
    }
}