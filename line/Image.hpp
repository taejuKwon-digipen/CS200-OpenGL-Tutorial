/*
 * Rudy Castan
 * Graphics Library
 * CS200
 * Fall 2018
 */
#pragma once
#include <string>
#include <vector>
#include "Color.hpp"

namespace CS200
{
    class [[nodiscard]] Image
    {
    public:
        explicit Image(int pixel_width = 0, int pixel_height = 0, Color color = Color{}) noexcept;
        void SaveToPNG(const std::string& file_path, bool flip_vertically = true) const noexcept;
        void SetPixel(int column, int row, Color color);
        void Clear(Color color);

    private:
        int                width  = 0;
        int                height = 0;
        std::vector<Color> pixels{};

        static constexpr int ChannelsPerColor = sizeof(Color::RGBA32);
    };
}
