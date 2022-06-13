/*
 * Rudy Castan, Jooho Jeong
 * Graphics Library
 * CS200
 * Fall 2020
 */
#include "Image.hpp"
#include "draw_line.h"

using namespace CS200;

constexpr Color BLACK{0x000000FF};
constexpr Color BEAUTY_BUSH_PINK{0xf0bcd4FF};

constexpr int WIDTH  = 256;
constexpr int HEIGHT = 256;

Image gImage{WIDTH, HEIGHT, BEAUTY_BUSH_PINK};

void SETPIXEL(int column, int row, Color intensity) { gImage.SetPixel(column, row, intensity); }

void DrawLine(int x0, int y0, int x1, int y1, Color intensity);

int main(void)
{
    DrawLine(10, 50, 246, 206, BLACK);
    gImage.SaveToPNG("test0.png");

    gImage.Clear(BEAUTY_BUSH_PINK);
    DrawLine(246, 206, 10, 50, BLACK);
    gImage.SaveToPNG("test1.png");

    gImage.Clear(BEAUTY_BUSH_PINK);
    DrawLine(50, 10, 206, 246, BLACK);
    gImage.SaveToPNG("test2.png");

    gImage.Clear(BEAUTY_BUSH_PINK);
    DrawLine(206, 246, 50, 10, BLACK);
    gImage.SaveToPNG("test3.png");

    gImage.Clear(BEAUTY_BUSH_PINK);
    DrawLine(50, 246, 206, 10, BLACK);
    gImage.SaveToPNG("test4.png");

    gImage.Clear(BEAUTY_BUSH_PINK);
    DrawLine(206, 10, 50, 246, BLACK);
    gImage.SaveToPNG("test5.png");

    gImage.Clear(BEAUTY_BUSH_PINK);
    DrawLine(10, 206, 246, 50, BLACK);
    gImage.SaveToPNG("test6.png");

    gImage.Clear(BEAUTY_BUSH_PINK);
    DrawLine(246, 50, 10, 206, BLACK);
    gImage.SaveToPNG("test7.png");

    gImage.Clear(BEAUTY_BUSH_PINK);
    DrawLine(10, 128, 246, 128, BLACK);
    gImage.SaveToPNG("test8.png");

    gImage.Clear(BEAUTY_BUSH_PINK);
    DrawLine(128, 10, 128, 248, BLACK);
    gImage.SaveToPNG("test9.png");

    return 0;
}

void DrawLine(int x0, int y0, int x1, int y1, Color intensity)
{
    CS200::draw_line(x0, y0, x1, y1, intensity, SETPIXEL);
}