#pragma once

#include "m_pd.h"
#include "z_libpd.h"
#include <raylib.h>
#include <sol/sol.hpp>
#include <sol/types.hpp>

#define PD_BUFFER_SIZE 64
#define N_CHANNELS 2
#define SR 44100


class pdEngine {
public:
    pdEngine();
    ~pdEngine();

    void init(std::string path);
    void process(float* in, float* out, unsigned int frames);

    int load_patch(const char* name);
    bool free_patch(int handle);
    void msg(int handle, const char* msg, sol::variadic_args args);
    void msg_init(int handle, sol::table& args);

    void load_all_patches(const std::string& dir);
    bool is_initialized(){ return initialized; }

    static t_atom to_atom(const sol::object& v) {
        t_atom a;
        if (v.is<bool>()) {
            libpd_set_float(&a, v.as<bool>() ? 1.0f : 0.0f);
        } else if (v.is<int>()) {
            libpd_set_float(&a, static_cast<float>(v.as<int>()));
        } else if (v.is<long long>()) {
            libpd_set_float(&a, static_cast<float>(v.as<long long>()));
        } else if (v.is<double>()) {
            libpd_set_float(&a, static_cast<float>(v.as<double>()));
        } else if (v.is<std::string>()) {
            const std::string& s = v.as<std::string>();
            libpd_set_symbol(&a, s.c_str());
        } else {
            libpd_set_float(&a, 0.0f);
        }
        return a;
    }

private:
    float pdBuffer[N_CHANNELS * PD_BUFFER_SIZE] = {};
    unsigned int pdIndex = N_CHANNELS * PD_BUFFER_SIZE;
    unsigned int pdTop = N_CHANNELS * PD_BUFFER_SIZE;
    AudioStream s;
    bool initialized = false;
    std::unordered_map<int, void*> handles;
    std::string pd_path;
};