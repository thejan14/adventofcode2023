#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <map>
#include <ranges>

constexpr int DIM = 140;

struct NumberPos
{
    int i, j1, j2;

    bool operator<(NumberPos const& pos) const
    {
        return std::make_tuple(this->i, this->j1, this->j2) < std::make_tuple(pos.i, pos.j1, pos.j2);
    }
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

constexpr bool hasAdjacentSymbol(std::string const& input, NumberPos const& pos)
{
    // check rows above and below the number
    for (auto const i : std::array{ pos.i - 1, pos.i + 1 })
    {
        for (int j = pos.j1 - 1; j < pos.j2 + 2; j++)
        {
            if (auto const index = getPos(i, j); index > -1 && index < input.length() && isSymbol(&input[index]))
            {
                return true;
            }
        }
    }

    // check leading and trailing character of the number
    for (auto const j : std::array{ pos.j1 - 1, pos.j2 + 1 })
    {
        if (auto const index = getPos(pos.i, j); index > 0 && index < input.length() && isSymbol(&input[index]))
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

    auto numbers = std::map<NumberPos, int>{};

    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (auto const first = &input[getPos(i, j)]; isNumber(first))
            {
                auto currentNumber = NumberPos{ i, j, j };
                while (isNumber(&input[getPos(i, ++j)]))
                {
                    ++currentNumber.j2;
                }

                auto const last = &input[getPos(i, j)];

                int parsedNumber;
                auto [ptr, ec] = std::from_chars(first, last, parsedNumber);
                assert(ptr == last);

                numbers[currentNumber] = parsedNumber;
            }
        }
    }

    auto answer = 0;
    for (auto const& [pos, number] : numbers)
    {
        if (hasAdjacentSymbol(input, pos))
        {
            answer += number;
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::milliseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}