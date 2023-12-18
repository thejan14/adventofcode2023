#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

#include "aoc.h"

enum Direction { N = 3, E = 0, S = 1, W = 2 };

static long long hextoll(std::string_view const& sv)
{
    long long value;
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value, 16).ec == std::errc{})
    {
        return value;
    }

    throw std::invalid_argument("not a number");
}

template <typename T>
auto split(T const& str, char sep)
{
    return str
        | std::views::split(sep)
        | std::views::transform([](auto const t) { return std::string_view(t); });
}

std::pair<Direction, long long> parseHexInstruction(std::string_view const& instruction)
{
    auto const distance = hextoll(instruction.substr(0, 5));
    auto const direction = static_cast<Direction>(hextoll(instruction.substr(5)));
    return std::make_pair(direction, distance);
}

std::pair<long long, long long> moveToDirection(std::pair<long long, long long> current, Direction const direction, long long const distance)
{
    auto const [i, j] = current;
    switch (direction)
    {
        case N: return std::make_pair(i - distance, j);
        case E: return std::make_pair(i, j + distance);
        case S: return std::make_pair(i + distance, j);
        case W: return std::make_pair(i, j - distance);
    }
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto area = 0LL;
    auto outline = 0LL;
    auto current = std::make_pair(0LL, 0LL);
    for (auto const line : split(input, '\n'))
    {
        auto const hashPos = line.find('#');
        auto const [direction, distance] = parseHexInstruction(line.substr(hashPos + 1, 6));
        auto const next = moveToDirection(current, direction, distance);
        auto const [ci, cj] = current;
        auto const [ni, nj] = next;
        area += (ci + ni) * (cj - nj); // https://en.wikipedia.org/wiki/Shoelace_formula
        outline += distance;
        current = next;
    }

    // thanks to u/echlos021 (https://www.reddit.com/r/adventofcode/comments/18l2tap/comment/kdv8imu) 
    auto const answer = (area / 2) + (outline / 2) + 1;

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}
