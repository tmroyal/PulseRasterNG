#pragma once

#include <raylib.h>
#define PD_BUFFER_SIZE 64
#define N_CHANNELS 2
#define SR 44100

class pdEngine {
public:
    pdEngine();
    ~pdEngine();

    void init();
    void chirp();
    void process(float* in, float* out, unsigned int frames);
private:
    float pdBuffer[N_CHANNELS * PD_BUFFER_SIZE] = {};
    unsigned int pdIndex = N_CHANNELS * PD_BUFFER_SIZE;
    unsigned int pdTop = N_CHANNELS * PD_BUFFER_SIZE;
    AudioStream s;
};