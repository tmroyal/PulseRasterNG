#pragma once

#include <libremidi/libremidi.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include "../audio_engine/pdMidi.hpp"

class MidiManager {
public:
    static void listPorts();

    MidiManager(pdMidi& pdm): pdm(pdm), observer() {}
    ~MidiManager(){
        closeAllPorts();
    }

    bool openInputPort(int number, const std::string& portName);
    void closeInputPort(const std::string& portName);
    void closeAllPorts();
private:
    struct MidiPortInfo {
        std::unique_ptr<libremidi::midi_in> midiInput;
        int portNumber;
    };

    libremidi::input_port getPortByName(const std::string& port);
    pdMidi& pdm;
    libremidi::observer observer;
    std::unordered_map<std::string, MidiPortInfo> openPorts;
};