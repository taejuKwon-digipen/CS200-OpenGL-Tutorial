/*
 * Rudy Castan
 * Graphics Library
 * CS200
 * Fall 2020
 */
#include "draw_line.h"

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

constexpr int WIDTH  = 16;
constexpr int HEIGHT = 8;

using namespace CS200;

constexpr Color BLACK{0x000000FF};
constexpr Color BEAUTY_BUSH_PINK{0xf0bcd4FF};


template <typename ARRAY>
void print_image(const ARRAY& image)
{
    for (int row = HEIGHT - 1; row >= 0; --row)
    {
        for (int column = 0; column < WIDTH; ++column)
        {
            if (image[row * WIDTH + column])
            {
                std::cout << '#';
            }
            else
            {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }
}


struct TestCase
{
    int                              x0{0}, y0{0}, x1{0}, y1{0};
    std::array<bool, WIDTH * HEIGHT> result{};
    std::array<bool, WIDTH * HEIGHT> alternativeResult{};
};

std::string to_string(const TestCase& test_case)
{
    std::stringstream ss;
    ss << "Test Line from (" << test_case.x0 << "," << test_case.y0 << ") to (" << test_case.x1 << "," << test_case.y1
       << ")";
    return ss.str();
}

int main(void)
{
    std::ifstream file{"test_cases.txt"};
    int           max_visuals_report = 10;
    long          numCasesPassed     = 0;
    long          numCases           = 0;
    TestCase      test_case;
    while (file >> test_case.x0 >> test_case.y0 >> test_case.x1 >> test_case.y1)
    {
        ++numCases;
        for (auto& val : test_case.result)
        {
            char f{0};
            file >> f;
            val = f == '#';
        }
        char same{0};
        file >> same;
        if (same == 'd')
        {
            for (auto& val : test_case.alternativeResult)
            {
                char f{0};
                file >> f;
                val = f == '#';
            }
        }
        else
        {
            test_case.alternativeResult = test_case.result;
        }
        std::array<bool, WIDTH * HEIGHT> image{};
        CS200::draw_line(test_case.x0, test_case.y0, test_case.x1, test_case.y1, BEAUTY_BUSH_PINK,
                         [&](int x, int y, Color) {
                             const bool column_is_good = (x >= 0 && x < WIDTH);
                             const bool row_is_good    = (y >= 0 && y < HEIGHT);

                             if (column_is_good && row_is_good)
                             {
                                 image[y * WIDTH + x] = true;
                             }
                             else
                             {
                                 std::cerr << "Failed Test Case - " << to_string(test_case) << '\n';
                                 std::cerr << "BAD SETPIXEL VALUE: (" << x << ", " << y << ")\n";
                                 std::exit(-1);
                             }
                         });
        if (image != test_case.result && image != test_case.alternativeResult)
        {
            if (max_visuals_report > 0)
            {
                std::cout << " Failed Test Case - " << to_string(test_case) << '\n';

                std::cout << "---------------\n"
                          << "Expected\n"
                          << "---------------\n";
                print_image(test_case.result);
                if (same == 'd')
                {
                    std::cout << "---------------\n"
                              << "Or Expected\n"
                              << "---------------\n";
                    print_image(test_case.alternativeResult);
                }

                std::cout << "---------------\n"
                          << "Student Produced\n"
                          << "---------------\n";
                print_image(image);
                --max_visuals_report;
            }
        }
        else
        {
            ++numCasesPassed;
        }
    }
    std::cout << " Test Cases passed " << numCasesPassed << "/" << numCases << '\n';

    return 0;
}
