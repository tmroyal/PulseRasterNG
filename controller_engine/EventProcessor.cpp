#include "EventProcessor.hpp"

void EventProcessor::process() {
    // These will need to ve updated if raylib adds more mouse buttons
    for (int i = 0; i <= MOUSE_BUTTON_BACK; i++) {
        if (IsMouseButtonPressed(i)) {
            std::cout << "Mouse button " << i << " pressed" << std::endl;
        }
        if (IsMouseButtonReleased(i)) {
            std::cout << "Mouse button " << i << " released" << std::endl;
        }
    }
    // TODO: Support custom touch events
}