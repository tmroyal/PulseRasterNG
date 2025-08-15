#include "GraphicsApi.hpp"

void GraphicsApi::applyGraphicsApi(sol::state& lua){
    // TODO: support resizing
    lua["width"] = GetScreenWidth();
    lua["height"] = GetScreenHeight();

    lua.set_function("fill", [&](unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
        fillColor = {r, g, b, a};
    });
    
    lua.set_function("background", [&](unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
        backgroundColor = {r, g, b, a};
    });

    lua.set_function("rect", [&](float x, float y, float width, float height) {
        DrawRectangle((int)x, (int)y, (int)width, (int)height, fillColor);
    });

    lua.set_function("circle", [&](float x, float y, float radius) {
        DrawCircle((int)x, (int)y, radius, fillColor);
    });
}
