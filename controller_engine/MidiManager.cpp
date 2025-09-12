#include "MidiManager.hpp"
#include <iostream>
#include <stdexcept>

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

bool MidiManager::openInputPort(int number, const std::string& name){
    if (openPorts.find(name) != openPorts.end()) {
        return true; // Already open
    }
    
    // Check for port number collision
    for (const auto& pair : openPorts) {
        if (pair.second.portNumber == number) {
            std::cerr << "Port number " << number << " is already assigned to MIDI port '" 
                      << pair.first << "'. Please use a different port number." << std::endl;
            return false;
        }
    }
    
    try {
        auto my_callback = [this, number](const libremidi::message& message) {
            pdm.sendMidi(number, message);
        };
        
        auto midi = std::make_unique<libremidi::midi_in>(
            libremidi::input_configuration{ .on_message = my_callback }
        );
        
        midi->open_port(getPortByName(name));
        openPorts[name] = {std::move(midi), number};
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to open MIDI port '" << name << "': " << e.what() << std::endl;
        return false;
    }
}

void MidiManager::closeInputPort(const std::string& name){
    auto it = openPorts.find(name);
    if (it != openPorts.end()) {
        it->second.midiInput->close_port();
        openPorts.erase(it);
    }
}

void MidiManager::closeAllPorts(){
    for (auto& pair : openPorts) {
        pair.second.midiInput->close_port();
    }
    openPorts.clear();
}

libremidi::input_port& MidiManager::getPortByName(const std::string& name){
    libremidi::observer obs;
    
    for (const libremidi::input_port& port : obs.get_input_ports()) {
        if (port.port_name == name) {
            return const_cast<libremidi::input_port&>(port);
        }
    }
    
    throw std::runtime_error("MIDI input port '" + name + "' not found");
}