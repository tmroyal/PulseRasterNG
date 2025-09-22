#include "EventProcessor.hpp"
#include <iostream>

void EventProcessor::process() {
  // These will need to ve updated if raylib adds more mouse buttons
  for (int i = 0; i <= MOUSE_BUTTON_BACK; i++) {
    if (IsMouseButtonPressed(i)) {
        runHandlers(mouseDownHandlers, i);
    }
    if (IsMouseButtonReleased(i)) {
        runHandlers(mouseUpHandlers, i);
    }
  }
  // TODO: Support custom touch events
}

void EventProcessor::onMouseDown(int button, sol::protected_function handler) {
  mouseDownHandlers[button].push_back(handler);
}

void EventProcessor::onMouseUp(int button, sol::protected_function handler) {
  mouseUpHandlers[button].push_back(handler);
}

void EventProcessor::runHandlers(
    const std::unordered_map<int, std::vector<sol::protected_function>>
        &handlersMap,
    int button) {
  auto it = handlersMap.find(button);
  if (it != handlersMap.end()) {
    Vector2 position = GetMousePosition();
    for (const auto &handler : it->second) {
      sol::protected_function_result result = handler(position.x, position.y, button);
      if (!result.valid()) {
        sol::error err = result;
        std::cerr << "Error running event handler: " << err.what() << std::endl;
      }
    }
  }
}