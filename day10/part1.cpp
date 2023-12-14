﻿#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

#include "aoc.h"

constexpr int DIM = 140;
constexpr auto validN = std::array{ '|', 'F', '7' };
constexpr auto validO = std::array{ '-', 'J', '7' };
constexpr auto validS = std::array{ '|', 'L', 'J' };
constexpr auto validW = std::array{ '-', 'F', 'L' };

enum Direction { N, E, S, W };

inline int getIndex(int const i, int const j)
{
    return i * (DIM + 1) + j;
}

inline std::pair<int, int> getPos(int const index)
{
    return { index / (DIM + 1), index % (DIM + 1) };
}

std::pair<int, Direction> traverse(std::string const& input, int const current, Direction const from)
{
    auto const [i, j] = getPos(current);
    switch (input[current])
    {
        case '|': return from == N
            ? std::make_pair(getIndex(i - 1, j), N)
            : std::make_pair(getIndex(i + 1, j), S);
        case '-': return from == E
            ? std::make_pair(getIndex(i, j + 1), E)
            : std::make_pair(getIndex(i, j - 1), W);
        case 'L': return from == S
            ? std::make_pair(getIndex(i, j + 1), E)
            : std::make_pair(getIndex(i - 1, j), N);
        case 'J': return from == S
            ? std::make_pair(getIndex(i, j - 1), W)
            : std::make_pair(getIndex(i - 1, j), N);
        case '7': return from == N
            ? std::make_pair(getIndex(i, j - 1), W)
            : std::make_pair(getIndex(i + 1, j), S);
        case 'F': return from == N
            ? std::make_pair(getIndex(i, j + 1), E)
            : std::make_pair(getIndex(i + 1, j), S);
        default: throw std::invalid_argument("no pipe at given position");
    }
}

std::pair<char, Direction> getStartPipe(std::string const& input, int const start)
{
    auto const [si, sj] = getPos(start);
    auto const north = std::ranges::contains(validN, input[getIndex(si - 1, sj)]);
    auto const east = std::ranges::contains(validO, input[getIndex(si, sj + 1)]);
    auto const south = std::ranges::contains(validS, input[getIndex(si + 1, sj)]);
    auto const west = std::ranges::contains(validW, input[getIndex(si, sj - 1)]);

    if (north && south) return { '|', N };
    if (east && west) return { '-', E };
    if (north && east) return { 'L', S };
    if (north && west) return { 'J', S };
    if (south && west) return { '7', N };
    if (south && east) return { 'F', N };

    throw std::invalid_argument("start position cannot be a pipe");
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto const startIndex = static_cast<int>(input.find('S'));
    auto const [pipe, startDirection] = getStartPipe(input, startIndex);
    input[startIndex] = pipe;

    auto length = 0;
    auto currentIndex = startIndex;
    auto currentDirection = startDirection;
    do
    {
        auto const [newIndex, newDirection] = traverse(input, currentIndex, currentDirection);
        currentIndex = newIndex;
        currentDirection = newDirection;
        ++length;
    }
    while (currentIndex != startIndex);

    auto const answer = length / 2;

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}