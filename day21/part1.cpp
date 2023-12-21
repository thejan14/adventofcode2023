#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <bitset>
#include <unordered_set>

#include "aoc.h"

constexpr auto DIM = 131;
constexpr auto STEPS = 64;

struct pair_hash
{
    inline std::size_t operator()(std::pair<int, int> const& v) const
    {
        return v.first * DIM + v.second;
    }
};

bool isValid(std::array<std::bitset<DIM>, DIM> const& gardenMap, std::pair<int, int> const& next)
{
    auto const [i, j] = next;
    return i > -1 && i < DIM && j > -1 && j < DIM && !gardenMap[i].test(j);
}

auto getNext(std::array<std::bitset<DIM>, DIM> const& gardenMap, std::pair<int, int> from)
{
    auto [i, j] = from;
    return std::array{
        std::make_pair(i - 1, j),
        std::make_pair(i, j + 1),
        std::make_pair(i + 1, j),
        std::make_pair(i, j - 1),
    } | std::views::filter([&gardenMap](auto const& next) { return isValid(gardenMap, next); });
}

size_t findReachablePlots(std::array<std::bitset<DIM>, DIM>& gardenMap, std::pair<int, int> const& start)
{
    auto reachable = std::unordered_set<std::pair<int, int>, pair_hash>{ start };
    for (auto i = 0; i < STEPS; i++)
    {
        reachable = reachable
            | std::views::transform([&gardenMap](auto const pos) { return getNext(gardenMap, pos); })
            | std::views::join
            | std::ranges::to<std::unordered_set<std::pair<int, int>, pair_hash>>();
    }

    return reachable.size();
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto gardenMap = std::array<std::bitset<DIM>, DIM>{};
    auto start = std::pair<int, int>{};

    auto i = 0;
    for (auto const c : input | std::views::filter([](auto const c) { return c != '\n'; }))
    {
        gardenMap[i / DIM][i % DIM] = c == '#';
        if (c == 'S')
        {
            start = std::make_pair(i / DIM, i % DIM);
        }
        
        ++i;
    }

    auto const answer = findReachablePlots(gardenMap, start);

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}
