﻿#include <string>
#include <chrono>
#include <format>
#include <iostream>
#include <ranges>

#include "aoc.h"

using std::operator""sv;

static long long svtoll(std::string_view const& sv)
{
    long long value;
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{})
    {
        return value;
    }

    throw std::invalid_argument("not a number");
}

inline long long parseNumber(std::string_view const& input)
{
    auto const& numberStr = std::views::split(input, " "sv)
        | std::views::drop(1)
        | std::views::transform([](auto token) { return std::string_view(token); })
        | std::views::filter([](auto svToken) { return !svToken.empty(); })
        | std::views::join
        | std::ranges::to<std::string>();
    return svtoll(numberStr);
}

constexpr long long calcTravelDistance(long long const holdTime, long long const totalTime)
{
    // travel time: totalTime - holdTime
    // speed (mm/ms): hold time (1 mm per 1 ms)
    // total travel distance: travel time * speed
    return (totalTime - holdTime) * holdTime;
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto const eolPos = input.find('\n');
    auto const raceTime = parseNumber(input.substr(0, eolPos));
    auto const recordDistance = parseNumber(input.substr(eolPos + 1));
    auto answer = long long{ 0 };
    for (auto holdTime = 1; holdTime < raceTime; holdTime++)
    {
        if (calcTravelDistance(holdTime, raceTime) > recordDistance)
        {
            ++answer;
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}