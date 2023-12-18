#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <map>
#include <set>
#include <queue>

#include "aoc.h"

enum Direction { N, E, S, W };

static int svtoi(std::string_view const& sv)
{
    int value;
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{})
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

Direction getDirection(char c)
{
    switch (c)
    {
        case 'U': return N;
        case 'R': return E;
        case 'D': return S;
        case 'L': return W;
        default: throw std::invalid_argument("direction does not exist");
    }
}

std::pair<int, int> moveToDirection(std::pair<int, int> current, Direction const d)
{
    auto const [i, j] = current;
    switch (d)
    {
        case N: return std::make_pair(i - 1, j);
        case E: return std::make_pair(i, j + 1);
        case S: return std::make_pair(i + 1, j);
        case W: return std::make_pair(i, j - 1);
    }
}

char getWinding(Direction const d)
{
    switch (d)
    {
        case N: return 'N';
        case E: return 'X';
        case S: return 'S';
        case W: return 'X';
    }
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto outline = std::map<std::pair<int, int>, char>{};
    auto current = std::make_pair(0, 0);
    outline[current] = 'X';
    auto minX = 0;
    auto minY = 0;
    auto maxX = 0;
    auto maxY = 0;
    for (auto const line : split(input, '\n'))
    {
        auto const plan = split(line, ' ') | std::ranges::to<std::vector>();
        auto const direction = getDirection(plan[0][0]);
        auto const distance = svtoi(plan[1]);
        auto const winding = getWinding(direction);
        if (winding == 'S')
        {
            outline[current] = 'S';
        }

        for (auto i = 0; i < distance; i++)
        {
            current = moveToDirection(current, direction);
            outline[current] = winding;
            minX = std::min(current.second, minX);
            minY = std::min(current.first, minY);
            maxX = std::max(current.second, maxX);
            maxY = std::max(current.first, maxY);
        }

        if (winding == 'S')
        {
            outline[current] = 'X';
        }
    }

    auto answer = outline.size();
    for (auto i = minY; i <= maxY; i++)
    {
        auto windings = 0;
        for (auto j = minX; j <= maxX; j++)
        {
            auto current = '.';
            if (auto it = outline.find(std::make_pair(i, j)); it != outline.end())
            {
                auto const [_, w] = *it;
                current = w;
            }

            switch (current)
            {
                case 'N': --windings; break;
                case 'S': ++windings; break;
                default: if (current != 'X' && windings != 0) ++answer;
            }
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}
