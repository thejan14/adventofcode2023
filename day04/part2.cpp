#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <set>

#include "aoc.h"

using std::operator""sv;

struct Scratchcard
{
    int count, matches;
};

static std::optional<int> svtoi(std::string_view const& sv)
{
    int value;
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{})
    {
        return value;
    }

    return std::nullopt;
}

constexpr std::string trim(std::string const& str)
{
    return str.substr(str.find_first_not_of(" \n\r\t") + 1, str.find_last_not_of(" \n\r\t"));
}

std::set<int> parseNumbers(std::string const& numbersStr)
{
    auto numbersRes = std::set<int>{};
    for (auto const numberToken : std::views::split(std::string_view(numbersStr), " "sv))
    {
        // in case of two consecutive spaces we may have an empty token
        auto const numberTokenSv = std::string_view(numberToken);
        if (!numberTokenSv.empty())
        {
            auto const parsedNumber = svtoi(numberTokenSv);
            assert(parsedNumber.has_value());
            if (parsedNumber.has_value())
            {
                numbersRes.insert(parsedNumber.value());
            }
        }
    }

    return numbersRes;
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto scratchcards = std::vector<Scratchcard>();

    std::string line;
    auto parseStream = std::stringstream(input);
    while (getline(parseStream, line))
    {
        auto const colonPos = line.find(':');
        auto const numbersTable = line.substr(colonPos + 1);
        auto const pipePos = numbersTable.find('|');
        auto const winningNumbers = parseNumbers(numbersTable.substr(0, pipePos - 1));
        auto const drawnNumbers = parseNumbers(numbersTable.substr(pipePos + 2));
        auto matchingNumbers = std::vector<int>{};
        std::ranges::set_intersection(winningNumbers, drawnNumbers, std::back_inserter(matchingNumbers));
        scratchcards.emplace_back(1, matchingNumbers.size());
    }

    // as the last card cannot win any additional cards we do not need to consider it
    auto const penultimate = scratchcards.size() - 1;
    for (size_t i = 0; i < penultimate; i++)
    {
        // add a scratchcard for each match consecutively from below (i+1) the current scratchcard
        auto limit = std::min(scratchcards.size(), i + scratchcards[i].matches + 1);
        for (size_t j = i + 1; j < limit; j++)
        {
            // add scratchcards for each (count) of the scratchcards with the same id
            scratchcards[j].count += scratchcards[i].count;
        }
    }

    auto const answer = std::ranges::fold_left(scratchcards | std::views::transform([](auto const& sc) { return sc.count; }), 0, std::plus());

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}