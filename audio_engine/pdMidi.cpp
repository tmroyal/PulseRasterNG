#include "pdMidi.hpp"
#include "z_libpd.h"

void pdMidi::sendMidi(int port, const libremidi::message& message){
    if (message.size() < 1) return;
    
    // Map port to channel range: port 0 = channels 0-15, port 1 = channels 16-31, etc.
    int baseChannel = port * 16;
    
    uint8_t status = message[0];
    uint8_t messageType = status & 0xF0;
    uint8_t midiChannel = status & 0x0F;
    
    // Calculate the actual PD channel (0-based)
    int pdChannel = baseChannel + midiChannel;
    
    switch (messageType) {
        case 0x90: { // Note On
            if (message.size() >= 3) {
                uint8_t note = message[1];
                uint8_t velocity = message[2];
                libpd_noteon(pdChannel, note, velocity);
            }
            break;
        }
        case 0x80: { // Note Off - convert to Note On with velocity 0
            if (message.size() >= 3) {
                uint8_t note = message[1];
                libpd_noteon(pdChannel, note, 0);
            }
            break;
        }
        case 0xB0: { // Control Change
            if (message.size() >= 3) {
                uint8_t controller = message[1];
                uint8_t value = message[2];
                libpd_controlchange(pdChannel, controller, value);
            }
            break;
        }
        case 0xC0: { // Program Change
            if (message.size() >= 2) {
                uint8_t program = message[1];
                libpd_programchange(pdChannel, program);
            }
            break;
        }
        case 0xE0: { // Pitch Bend
            if (message.size() >= 3) {
                uint8_t lsb = message[1];
                uint8_t msb = message[2];
                int value = (msb << 7) | lsb;
                libpd_pitchbend(pdChannel, value);
            }
            break;
        }
        case 0xD0: { // Channel Aftertouch
            if (message.size() >= 2) {
                uint8_t value = message[1];
                libpd_aftertouch(pdChannel, value);
            }
            break;
        }
        case 0xA0: { // Polyphonic Aftertouch
            if (message.size() >= 3) {
                uint8_t note = message[1];
                uint8_t value = message[2];
                libpd_polyaftertouch(pdChannel, note, value);
            }
            break;
        }
        default:
            // Send raw MIDI bytes for system messages or other unhandled messages
            for (size_t i = 0; i < message.size(); ++i) {
                libpd_midibyte(port, message[i]);
            }
            break;
    }
}