﻿#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

#include "aoc.h"

using std::operator""sv;

struct CubeBag
{
    int r, g, b;
};

static constexpr std::optional<int> svtoi(std::string_view const& sv)
{
    int value;
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{})
    {
        return value;
    }

    return std::nullopt;
}

constexpr void addMaxCubes(std::string_view const& cubeColor, int const cubeCount, CubeBag& bag)
{
    if (cubeColor == "red" && bag.r < cubeCount)
    {
        bag.r = cubeCount;
    }

    if (cubeColor == "green" && bag.g < cubeCount)
    {
        bag.g = cubeCount;
    }

    if (cubeColor == "blue" && bag.b < cubeCount)
    {
        bag.b = cubeCount;
    }
}

constexpr int parseAndCalcGamePower(std::string const& setInfosToken)
{
    auto maxBag = CubeBag{};
    for (auto const setToken : std::views::split(std::string_view(setInfosToken), "; "sv))
    {
        for (auto const cubeToken : std::views::split(std::string_view(setToken), ", "sv))
        {
            auto const cubeTokenSv = std::string_view(cubeToken);
            auto const whitespacePos = cubeTokenSv.find(' ');
            auto const cubeCount = svtoi(cubeTokenSv.substr(0, whitespacePos)).value_or(0);
            auto const cubeColor = cubeTokenSv.substr(whitespacePos + 1, cubeTokenSv.size() - whitespacePos + 1);
            addMaxCubes(cubeColor, cubeCount, maxBag);
        }
    }

    return maxBag.r * maxBag.g * maxBag.b;
}

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
        auto const colonPos = line.find(':');
        auto const gameToken = line.substr(0, colonPos);
        auto const setInfosToken = line.substr(colonPos + 2); // skip trailing whitespace after colon
        answer += parseAndCalcGamePower(setInfosToken);
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}