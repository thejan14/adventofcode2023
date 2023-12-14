#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <array>

#include "aoc.h"

/* thanks to the reddit solutions thread for inspirations on improving my solution, especially u/endl */

using pos = std::pair<long long, long long>;

constexpr int DIM = 140;

// https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

long long getDistance(pos const& galaxy1, pos  const& galaxy2)
{
    return std::abs(galaxy1.first - galaxy2.first) + std::abs(galaxy1.second - galaxy2.second);
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    // save distance added through expansion for each row/column, i.e.
    // 0 for no expansion (contains galaxy) and 999'999 for expansion (999'999 more row/column)
    auto expansionRows = std::array<long long, DIM>{};
    auto expansionColumns = std::array<long long, DIM>{};
    std::ranges::fill(expansionRows, 999'999);
    std::ranges::fill(expansionColumns, 999'999);

    auto galaxies = std::vector<pos>{};
    for (auto i = 0; i < DIM; i++)
    {
        for (auto j = 0; j < DIM; j++)
        {
            auto const c = input[i * (DIM + 1) + j];
            if (c == '#')
            {
                galaxies.emplace_back(i, j);
                expansionRows[i] = 0;
                expansionColumns[j] = 0;
            }
        }
    }

    // accumulate the expansion for each row/column, e.g:
    // [0, 1, 1, 0, 0, 0, 1, 0] =>
    // [0, 1, 2, 2, 2, 2, 3, 3]
    for (auto i = 1; i < DIM; i++)
    {
        expansionRows[i] += expansionRows[i - 1];
    }

    for (auto j = 1; j < DIM; j++)
    {
        expansionColumns[j] += expansionColumns[j - 1];
    }

    // use the expansion arrays to adjust the galaxy coordinates accordingly
    // (i.e. add the accumulated expansion distance to the coordinates)
    for (auto& [gi, gj] : galaxies)
    {
        gi += expansionRows[gi];
        gj += expansionColumns[gj];
    }

    // calculate the manhattan distance between each unique pair of galaxies
    auto answer = 0LL;
    for (size_t i = 0; i < galaxies.size() - 1; i++)
    {
        for (size_t j = i + 1; j < galaxies.size(); j++)
        {
            answer += getDistance(galaxies[i], galaxies[j]);
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}