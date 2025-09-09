#include "pdEngine.hpp"

#include <cstring>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

pdEngine::pdEngine() {}

pdEngine::~pdEngine() {}

void pdEngine::init(std::string path) {
  // Initialization code for PD engine
  if (!fs::exists(path) || !fs::is_directory(path)) {
    std::cerr << "Error: directory does not exist: " << path << std::endl;
    return;
  }
  pd_path = path;

  libpd_init();
  libpd_add_to_search_path(path.c_str());
  libpd_init_audio(0, 2, 44100);

  // start DSP
  libpd_start_message(1);
  libpd_add_float(1.0f);
  libpd_finish_message("pd", "dsp");

  initialized = true;
}

void pdEngine::process(float *in, float *out, unsigned int frames) {
  unsigned samples_needed = frames * N_CHANNELS;
  unsigned copied = 0;

  while (copied < samples_needed) {
    if (pdIndex == pdTop) { // (re)fill one Pd tick = 64 frames
      libpd_process_float(1, NULL, pdBuffer);
      pdIndex = 0;
      pdTop = PD_BUFFER_SIZE * N_CHANNELS;
    }
    unsigned take = (pdTop - pdIndex);
    if (take > samples_needed - copied) {
      take = samples_needed - copied;
    }
    memcpy(out + copied, pdBuffer + pdIndex, take * sizeof(float));
    pdIndex += take;
    copied += take;
  }
}

void pdEngine::msg(int handle, const char *msg, sol::variadic_args args) {
  std::vector<t_atom> atoms;
  for (auto v : args) {
    atoms.push_back(pdEngine::to_atom(v));
  }
  std::string name = std::to_string(handle) + "-rec";
  libpd_message(name.c_str(), msg, atoms.size(), atoms.data());
}

int pdEngine::load_patch(const char *name) {
  auto filename = std::string(name) + ".pd";
  void *vp = libpd_openfile(filename.c_str(), pd_path.c_str());
  if (!vp) {
    std::cerr << "Failed to load: " << name << "\n";
    return -1;
  }
  int dollar_zero = libpd_getdollarzero(vp);
  handles[dollar_zero] = vp;
  return dollar_zero;
}

bool pdEngine::free_patch(int handle) {
  auto it = handles.find(handle);
  if (it == handles.end()) {
    return false;
  }
  libpd_closefile(it->second);
  handles.erase(it);
  return true;
}

void pdEngine::load_all_patches(const std::string &dir) {
  for (const auto &entry : fs::directory_iterator(dir)) {
    if (entry.path().extension() == ".pd") {
      auto file = entry.path().filename().string();
      void *patch = libpd_openfile(file.c_str(), dir.c_str());
      if (!patch) {
        std::cerr << "Failed to load: " << file << "\n";
      }
    }
  }
}
