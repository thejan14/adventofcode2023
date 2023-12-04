#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <set>

using std::operator""sv;

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

    auto answer = 0;
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
        if (!matchingNumbers.empty())
        {
            answer += static_cast<int>(std::pow(2, matchingNumbers.size() - 1));
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, millis);
}