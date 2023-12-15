#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <algorithm>

#include "aoc.h"

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto sequence = input
        | std::views::split(',')
        | std::views::transform([](auto const hash)
            {
                auto result = 0;
                for (auto const c : hash)
                {
                    result += c;
                    result *= 17;
                    result %= 256;
                }

                return result;
            });

    auto const answer = std::ranges::fold_left(sequence, 0, std::plus<int>());

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}