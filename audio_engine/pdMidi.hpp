#pragma once

#include <libremidi/message.hpp>

class pdMidi {
public:
    void sendMidi(int port, const libremidi::message& message);
};