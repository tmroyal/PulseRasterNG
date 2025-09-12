#include "MidiManager.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using std::cout;

void MidiManager::listPorts() {
  libremidi::observer obs;

  std::cout << "Available MIDI Input Ports" << std::endl;
  for (const libremidi::input_port &port : obs.get_input_ports()) {
    std::cout << port.port_name << std::endl;
  }
  std::cout << "Available MIDI Output Ports" << std::endl;
  for (const libremidi::output_port &port : obs.get_output_ports()) {
    cout << port.port_name << std::endl;
  }
}