#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <map>
#include <regex>

const std::regex numbersRegex = std::regex(R"((\d|zero|one|two|three|four|five|six|seven|eight|nine))");

auto const numberLookup = std::map<std::string const, int>{
    {"zero", 0},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
};

inline int getNumber(std::string const& numberStr)
{
    if (numberStr.length() == 1)
    {
        // string is a single digit in numeric representation (e.g. "0")
        return numberStr[0] - '0';
    }

    // string is a single digit in literal representation (e.g. "zero")
    return numberLookup.at(numberStr);
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
        auto first = -1;
        auto last = -1;

        std::smatch match;
        auto it = line.cbegin();
        while (regex_search(it, line.cend(), match, numbersRegex))
        {
            auto const digit = getNumber(match[1]);
            if (first == -1)
            {
                first = digit;
            }

            last = digit;
            if (match.length() > 1)
            {
                // move the iterator to the penultimate character of the match to account for overlapping calibration values like twone (21)
                it = match.suffix().first - 1;
            }
            else
            {
                ++it;
            }
        }

        answer += first * 10 + last;
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, millis);
}