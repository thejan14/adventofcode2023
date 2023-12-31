﻿#include <iostream>
#include <string>
#include <chrono>
#include <format>

#include "aoc.h"

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto answer = 0;
    std::string line;
    auto parseStream = std::stringstream(input);
    while (getline(parseStream, line))
    {
        auto first = -1;
        auto last = -1;
        for (char const& c : line)
        {
            if (c >= '0' && c <= '9')
            {
                if (first == -1)
                {
                    first = c - '0';
                }

                last = c - '0';
            }
        }

        answer += first * 10 + last;
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}
