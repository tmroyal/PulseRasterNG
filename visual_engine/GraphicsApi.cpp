#include "GraphicsApi.hpp"

void GraphicsApi::applyGraphicsApi(sol::state& lua){
    // TODO: support resizing
    lua["width"] = GetScreenWidth();
    lua["height"] = GetScreenHeight();

    lua.set_function("pxl", [&](double x, double y){
        DrawPixel((int)x, (int)y, fillColor);
    });

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

    lua.set_function("thickness", [&](float t){
        thickness = (int)t;
    });

    lua.set_function("box", [&](float x, float y, float width, float height) {
        Rectangle rec{x, y, width, height};
        DrawRectangleLinesEx(rec, thickness, fillColor);
    });

    lua.set_function("circle", [&](float x, float y, float radius) {
        DrawCircle((int)x, (int)y, radius, fillColor);
    });

    lua.set_function("ring", [&](float x, float y, float radius){
        Vector2 center{x, y};
        DrawRing(center, radius-thickness, radius, 0, 360, 0, fillColor);
    });

    lua.set_function("line", [&](float x1, float y1, float x2, float y2){
        Vector2 v1{x1, y1};
        Vector2 v2{x2, y2};
        DrawLineEx(v1, v2, thickness, fillColor);
    });

    lua.set_function("polyfill", [&](float x, float y, int sides, float radius, float rotation){
        Vector2 center{x, y};
        DrawPoly(center, sides, radius, rotation, fillColor);
    });

    lua.set_function("polyline", [&](float x, float y, int sides, float raidus, float rotation){
        Vector2 center{x, y};
        DrawPolyLinesEx(center, sides, raidus, rotation, thickness, fillColor);
    });
}
