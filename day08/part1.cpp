#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <unordered_map>

using std::operator""sv;

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

    auto nodeMap = std::unordered_map<std::string_view, std::pair<std::string_view, std::string_view>>{};
    auto linesView = std::string_view(input.begin(), std::prev(input.end())) | std::views::split("\n"sv); // drop trailing newline
    auto directions = std::string_view(*std::ranges::begin(linesView)); // first line are direction instructions
    for (auto const line : linesView | std::views::drop(2) | std::views::transform([](auto const l) {return std::string_view(l); }))
    {
        nodeMap.emplace(
            line.substr(0, 3),
            std::make_pair(line.substr(7, 3), line.substr(12, 3)));
    }

    auto answer = 0;
    size_t currentDirection = 0;
    auto currentNode = "AAA"sv;
    while (currentNode != "ZZZ"sv)
    {
        currentNode = directions[currentDirection] == 'L'
            ? nodeMap[currentNode].first
            : nodeMap[currentNode].second;

        currentDirection = (currentDirection + 1) % directions.size();
        ++answer;
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}