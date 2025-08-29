#pragma once

#include <raylib.h>
#include <sol/variadic_args.hpp>
#define PD_BUFFER_SIZE 64
#define N_CHANNELS 2
#define SR 44100


class pdEngine {
public:
    pdEngine();
    ~pdEngine();

    void init(std::string path);
    void process(float* in, float* out, unsigned int frames);

    int load_synth(const char* name);
    bool free_synth(int handle);
    void synth(int handle, const char* msg, sol::variadic_args args);

    void load_all_patches(const std::string& dir);
    bool is_initialized(){ return initialized; }
private:
    float pdBuffer[N_CHANNELS * PD_BUFFER_SIZE] = {};
    unsigned int pdIndex = N_CHANNELS * PD_BUFFER_SIZE;
    unsigned int pdTop = N_CHANNELS * PD_BUFFER_SIZE;
    AudioStream s;
    bool initialized = false;
    std::unordered_map<int, void*> handles;
    std::string pd_path;
};