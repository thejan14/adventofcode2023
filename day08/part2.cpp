#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <numeric>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

using std::operator""sv;

struct PathCycle
{
    std::string_view end, current;
    long long length;
};

inline std::string getCycleState(std::vector<std::string_view> const& currentNodes)
{
    return currentNodes | std::views::join | std::ranges::to<std::string>();
}

int main()
{
    std::string input;
    if (auto readStream = std::ifstream("input.txt"); readStream.is_open())
    {
        std::string line;
        while (getline(readStream, line))
        {
            input += line;
            input.push_back('\n');
        }
    }

    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    /* we assume that every path contains a cycle through the exit (**Z). It takes a long time for the cycles to
     * "synchronise", i.e. all end on the exit at the same step. To avoid calculating through all these steps we
     * detect the length of each cycle and then find the least common multiple (LCM), which then is exactly how
     * many times the cycles need to loop in order to synchronise.
     */

    auto nodeMap = std::unordered_map<std::string_view, std::pair<std::string_view, std::string_view>>{};
    auto linesView = std::string_view(input.begin(), std::prev(input.end())) | std::views::split("\n"sv); // drop trailing newline
    auto directions = std::string_view(*std::ranges::begin(linesView)); // first line are direction instructions
    auto cycleInfos = std::vector<PathCycle>{};
    for (auto const line : linesView | std::views::drop(2) | std::views::transform([](auto const l) {return std::string_view(l); }))
    {
        auto const key = line.substr(0, 3);
        nodeMap.emplace(
            key,
            std::make_pair(line.substr(7, 3), line.substr(12, 3)));

        if (key[2] == 'A')
        {
            cycleInfos.emplace_back(""sv, key, 0);
        }
    }

    size_t currentDirection = 0;
    auto cycleLengths = std::vector<long long>{};
    auto paths = cycleInfos.size();
    while (cycleLengths.size() < paths)
    {
        for (auto it = cycleInfos.begin(); it != cycleInfos.end();)
        {
            auto& [end, current, length] = *it;
            current = directions[currentDirection] == 'L'
                ? nodeMap[current].first
                : nodeMap[current].second;

            if (!end.empty())
            {
                ++length;
            }

            if (std::string(end) == std::string(current))
            {
                // cycle found, stop processing this path and save cycle length for LCM calculation
                cycleLengths.push_back(length);
                it = cycleInfos.erase(it);
            }
            else if (current[2] == 'Z')
            {
                // passed though an exit, remember for cycle detection, start couning
                end = current;
                it = std::next(it);
            }
            else
            {
                it = std::next(it);
            }
        }

        currentDirection = (currentDirection + 1) % directions.size();
    }

    auto const answer = std::ranges::fold_left(cycleLengths | std::views::drop(1), cycleLengths[0], [](auto const acc, auto const x)
    {
        return std::lcm(acc, x);
    });

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}