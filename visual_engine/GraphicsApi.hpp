#pragma once

#include <raylib.h>
#include <sol/sol.hpp>

class GraphicsApi {
public:
    void applyGraphicsApi(sol::state& lua);
    Color getBackgroundColor() const { return backgroundColor; }
private:
    Color fillColor{255, 255, 255, 255}; // Default fill color
    Color backgroundColor{0, 0, 0, 255}; // Default background color
};