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

    void init();
    void process(float* in, float* out, unsigned int frames);

    void synth(const char* name, const char* msg, sol::variadic_args args);

    void load_all_patches(const std::string& dir);
private:
    float pdBuffer[N_CHANNELS * PD_BUFFER_SIZE] = {};
    unsigned int pdIndex = N_CHANNELS * PD_BUFFER_SIZE;
    unsigned int pdTop = N_CHANNELS * PD_BUFFER_SIZE;
    AudioStream s;
};