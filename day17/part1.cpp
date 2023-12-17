#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <map>
#include <queue>
#include <ranges>
#include <set>

#include "aoc.h"

constexpr int DIM = 141;

enum Direction { N = 0, E = 1, S = 2, W = 3 };

struct PriorityEntry
{
    std::pair<int, int> pos;
    std::pair<Direction, int> steps;
    int priority;

    bool operator<(const PriorityEntry& entry) const
    {
        return this->priority > entry.priority;
    }
};

bool isValidDirection(std::tuple<int, int, Direction> const& next, Direction const currentDirection)
{
    auto const [i, j, d] = next;
    auto const reverseDirection = (static_cast<int>(d) + 2) % 4;
    return i > -1 && i < DIM && j > -1 && j < DIM && static_cast<int>(currentDirection) != reverseDirection;
}

auto getNext(int const i, int const j, Direction const d)
{
    return std::array{
        std::make_tuple(i - 1, j, N),
        std::make_tuple(i, j + 1, E),
        std::make_tuple(i + 1, j, S),
        std::make_tuple(i, j - 1, W),
    } | std::views::filter([d](auto const& next) { return isValidDirection(next, d); });
}

int findPath(std::array<std::array<int, DIM>, DIM> const& heatLossMap)
{
    auto constexpr start = std::make_pair(0, 0);
    auto constexpr target = std::make_pair(DIM - 1, DIM - 1);
    auto frontier = std::priority_queue<PriorityEntry>{};
    frontier.emplace(start, std::make_pair(E, 0), 0);
    frontier.emplace(start, std::make_pair(S, 0), 0);
    auto heatLossSoFar = std::map<std::tuple<int, int, Direction, int>, int>{};
    heatLossSoFar.emplace(std::make_tuple(0, 0, S, 0), 0);
    heatLossSoFar.emplace(std::make_tuple(0, 0, E, 0), 0);

    while (!frontier.empty())
    {
        auto const [current, steps, priority] = frontier.top();
        frontier.pop();

        if (current == target)
        {
            return priority;
        }

        auto const [ci, cj] = current;
        auto const [currentDirection, currentSteps] = steps;
        auto const currentHeatLoss = heatLossSoFar[std::make_tuple(ci, cj, currentDirection, currentSteps)];
        for (auto const& [i, j, nextDirection] : getNext(ci, cj, currentDirection))
        {
            auto const stepsStraight = currentDirection == nextDirection ? currentSteps + 1 : 1;
            auto const newHeatLoss = currentHeatLoss + heatLossMap[i][j];
            auto const next = std::make_tuple(i, j, nextDirection, stepsStraight);
            if (stepsStraight < 4 && (!heatLossSoFar.contains(next) || newHeatLoss < heatLossSoFar[next]))
            {
                heatLossSoFar[next] = newHeatLoss;
                frontier.emplace(
                    std::make_pair(i, j),
                    std::make_pair(nextDirection, stepsStraight),
                    newHeatLoss);
            }
        }

    }

    return -1;
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto heatLossMap = std::array<std::array<int, DIM>, DIM>{};

    auto i = 0;
    for (auto const c : input | std::views::filter([](auto const c) { return c != '\n'; }))
    {
        heatLossMap[i / DIM][i % DIM] = c - '0';
        ++i;
    }

    auto const answer = findPath(heatLossMap);

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}