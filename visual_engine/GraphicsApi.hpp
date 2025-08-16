#pragma once

#include <raylib.h>
#include <sol/sol.hpp>

class GraphicsApi {
    static unsigned char toUChar(double v) {
        return static_cast<unsigned char>(std::clamp(v, 0.0, 1.0) * 255);
    }
public:
    void applyGraphicsApi(sol::state& lua);
    Color getBackgroundColor() const { return backgroundColor; }
private:
    Color fillColor{255, 255, 255, 255}; // Default fill color
    Color backgroundColor{0, 0, 0, 255}; // Default background color
};