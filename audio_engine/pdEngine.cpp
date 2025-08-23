#include "pdEngine.hpp"
#include "z_libpd.h"
#include <cstring>

pdEngine::pdEngine() {
}

pdEngine::~pdEngine() {
}

void pdEngine::init() {
    // Initialization code for PD engine
    libpd_init();
    libpd_add_to_search_path("puredata");
    libpd_openfile("chirp.pd", "puredata");
    libpd_init_audio(0, 2, 44100);

    // start DSP
    libpd_start_message(1);
    libpd_add_float(1.0f);
    libpd_finish_message("pd", "dsp");
}

void pdEngine::chirp() {
    // Example function to trigger a sound in PD
    libpd_bang("chirp"); 
}

void pdEngine::process(float* in, float* out, unsigned int frames) {
    unsigned samples_needed = frames * N_CHANNELS;
    unsigned copied = 0;

    while (copied < samples_needed) {
        if (pdIndex == pdTop) {              // (re)fill one Pd tick = 64 frames
            libpd_process_float(1, NULL, pdBuffer);
            pdIndex = 0;
            pdTop   = PD_BUFFER_SIZE * N_CHANNELS;
        }
        unsigned take = (pdTop - pdIndex);
        if (take > samples_needed - copied) { take = samples_needed - copied; }
        memcpy(out + copied, pdBuffer + pdIndex, take * sizeof(float));
        pdIndex += take;
        copied  += take;
    }
}
