#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <map>

#include "aoc.h"

constexpr auto CYCLES = 1'000'000'000ULL;

std::vector<std::vector<char*>> split(std::string& str, char sep)
{
    auto result = std::vector<std::vector<char*>>{};
    auto current = std::vector<char*>{};
    for (char& i : str)
    {
        if (i == '\n')
        {
            result.push_back(current);
            current.clear();
        }
        else
        {
            current.push_back(&i);
        }
    }

    result.push_back(current);
    return result;
}

std::vector<char*> getColumn(std::vector<std::vector<char*>>& rows, size_t const column)
{
    return rows
        | std::views::transform([column](auto& line) { return line[column]; })
        | std::ranges::to<std::vector>();
}

void tilt(std::vector<std::vector<char*>> const& view)
{
    for (auto& ray : view)
    {
        for (size_t i = 0; i < ray.size(); i++)
        {
            if (*ray[i] == '.' && i < ray.size() - 1)
            {
                auto j = i + 1;
                auto next = *ray[j];
                while (next == '.' && j < ray.size() - 1)
                {
                    next = *ray[++j];
                }

                if (next == 'O')
                {
                    *ray[i] = 'O';
                    *ray[j] = '.';
                }

                if (next == '#')
                {
                    i = j;
                }
            }
        }
    }
}

size_t getLoad(std::vector<std::vector<char*>> const& view)
{
    auto load = 0ULL;
    for (auto& ray : view)
    {
        for (size_t i = 0; i < ray.size(); i++)
        {
            if (*ray[i] == 'O')
            {
                load += ray.size() - i;
            }
        }
    }

    return load;
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    // rows from left to right
    auto westView = split(input, '\n');

    // rows from right to left
    auto eastView = westView
        | std::views::transform([](auto& row) { return row | std::views::reverse | std::ranges::to<std::vector>(); })
        | std::ranges::to<std::vector>();

    // columns from top to bottom
    auto northView = std::views::iota(0, static_cast<int>(westView.size()))
        | std::views::transform([&westView](auto const column) { return getColumn(westView, column); })
        | std::ranges::to<std::vector>();

    // columns from bottom to top
    auto southView = northView
        | std::views::transform([](auto& column) { return column | std::views::reverse | std::ranges::to<std::vector>(); })
        | std::ranges::to<std::vector>();

    auto states = std::map<std::string, size_t>{};
    for (auto i = 0ULL; i < CYCLES; i++)
    {
        tilt(northView);
        tilt(westView);
        tilt(southView);
        tilt(eastView);
        if (auto const it = states.find(input); it == states.end())
        {
            states.emplace(input, i);
        }
        else
        {
            auto const& [_, j] = *it;
            auto const cycleLength = i - j;
            if (auto const remaining = CYCLES - i; remaining > cycleLength)
            {
                auto skip = remaining / cycleLength;
                i += cycleLength * skip;
            }
        }
    }

    auto const answer = getLoad(northView);

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}