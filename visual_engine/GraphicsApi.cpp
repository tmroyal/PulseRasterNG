#include "GraphicsApi.hpp"

void GraphicsApi::applyGraphicsApi(sol::state& lua){
    // TODO: support resizing
    lua["width"] = GetScreenWidth();
    lua["height"] = GetScreenHeight();

    lua.set_function("fill", [&](double r, double g, double b, double a = 1.0) {
        fillColor = {
            GraphicsApi::toUChar(r),
            GraphicsApi::toUChar(g),
            GraphicsApi::toUChar(b),
            GraphicsApi::toUChar(a)
        };
    });

    lua.set_function("background", [&](double r, double g, double b, double a = 1.0) {
        backgroundColor = {
            GraphicsApi::toUChar(r),
            GraphicsApi::toUChar(g),
            GraphicsApi::toUChar(b),
            GraphicsApi::toUChar(a)
        };
    });

    lua.set_function("rect", [&](float x, float y, float width, float height) {
        DrawRectangle((int)x, (int)y, (int)width, (int)height, fillColor);
    });

    lua.set_function("circle", [&](float x, float y, float radius) {
        DrawCircle((int)x, (int)y, radius, fillColor);
    });
}
