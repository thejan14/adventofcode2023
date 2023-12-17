﻿#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <queue>
#include <ranges>
#include <set>

#include "aoc.h"

enum Direction { N = 0, E = 1, S = 2, W = 3 };

struct BeamState
{
    int i, j;
    Direction d;
};

int mod(int const a, int const b)
{
    return ((a % b) + b) % b;
}

template <typename T>
auto split(T const& str, char sep)
{
    return str
        | std::views::split(sep)
        | std::views::transform([](auto const t) { return std::string_view(t); });
}

bool isVertical(Direction const d)
{
    return d == N || d == S;
}

static BeamState getNext(BeamState const& state)
{
    switch (state.d)
    {
        case N: return { state.i - 1, state.j, state.d };
        case E: return { state.i, state.j + 1, state.d };
        case S: return { state.i + 1, state.j, state.d };
        case W: return { state.i, state.j - 1, state.d };
    }
}

BeamState turnClockwise(BeamState const& state)
{
    return getNext({ state.i, state.j, static_cast<Direction>(mod(state.d + 1, 4)) });
}

BeamState turnCounterClockwise(BeamState const& state)
{
    return getNext({ state.i, state.j, static_cast<Direction>(mod(state.d - 1, 4)) });
}

std::vector<BeamState> processTile(char const tile, BeamState const& state)
{
    switch (tile)
    {
        case '\\':
            return isVertical(state.d)
                ? std::vector{ turnCounterClockwise(state) }
                : std::vector{ turnClockwise(state) };
        case '/':
            return isVertical(state.d)
                ? std::vector{ turnClockwise(state) }
                : std::vector{ turnCounterClockwise(state) };
        case '-':
            return isVertical(state.d)
                ? std::vector{ turnCounterClockwise(state), turnClockwise(state) }
            : std::vector{ getNext(state) };
        case '|':
            return isVertical(state.d)
                ? std::vector{ getNext(state) }
                : std::vector{ turnCounterClockwise(state), turnClockwise(state) };
        default:
            return std::vector{ getNext(state) };
    }
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto const layout = split(input, '\n') | std::ranges::to<std::vector>();
    auto visited = std::vector(layout.size(), std::vector(layout.front().size(), std::set<Direction>{}));
    auto queue = std::queue<BeamState>{};
    queue.emplace(0, 0, E);
    while (!queue.empty())
    {
        auto const state = queue.front();
        auto const [i, j, d] = state;
        queue.pop();
        if (i > -1 && i < layout.size() && j > -1 && j < layout[0].size() && !visited[i][j].contains(d))
        {
            visited[i][j].insert(d);
            auto const current = layout[i][j];
            for (auto const& next : processTile(current, state))
            {
                queue.push(next);
            }
        }
    }

    auto const answer = std::ranges::count_if(
        visited | std::views::join,
        [](auto const& set) {return !set.empty(); });

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}