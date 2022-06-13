/*
 * Rudy Castan
 * Graphics Library
 * CS200
 * Fall 2020
 */
#pragma once

#include "Color.hpp"

#include <functional>

namespace CS200
{
    // set pixel function (int column, int row, Color intensity)
    using SetPixel = std::function<void(int, int, Color)>;
    void draw_line(int x0, int y0, int x1, int y1, Color intensity, SetPixel setpixel);
}