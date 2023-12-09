#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

constexpr int DIM = 140;

struct NumberInfo
{
    int i, j1, j2, n;
};

constexpr int getPos(int const i, int const j)
{
    return i * (DIM + 1) + j;
}

constexpr bool isNumber(char const* c)
{
    return *c >= '0' && *c <= '9';
}

constexpr bool isSymbol(char const* c)
{
    return *c != '.' && *c != '\n' && !isNumber(c);
}

constexpr bool hasAdjacentSymbol(std::string const& input, NumberInfo const& info)
{
    // check rows above and below the number
    for (auto const i : std::array{ info.i - 1, info.i + 1 })
    {
        for (int j = info.j1 - 1; j < info.j2 + 2; j++)
        {
            if (auto const index = getPos(i, j); index > -1 && index < input.length() && isSymbol(&input[index]))
            {
                return true;
            }
        }
    }

    // check leading and trailing character of the number
    for (auto const j : std::array{ info.j1 - 1, info.j2 + 1 })
    {
        if (auto const index = getPos(info.i, j); index > 0 && index < input.length() && isSymbol(&input[index]))
        {
            return true;
        }
    }

    return false;
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

    auto numbers = std::vector<NumberInfo>{};

    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (auto const first = &input[getPos(i, j)]; isNumber(first))
            {
                auto& currentNumber = numbers.emplace_back(i, j, j);
                while (isNumber(&input[getPos(i, ++j)]))
                {
                    ++currentNumber.j2;
                }

                auto const last = &input[getPos(i, j)];

                int parsedNumber;
                auto [ptr, _] = std::from_chars(first, last, parsedNumber);
                assert(ptr == last);

                currentNumber.n = parsedNumber;
            }
        }
    }

    auto answer = 0;
    for (auto const& info : numbers)
    {
        if (hasAdjacentSymbol(input, info))
        {
            answer += info.n;
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}