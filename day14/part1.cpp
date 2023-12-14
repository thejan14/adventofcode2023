#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

#include "aoc.h"

template <typename T>
auto split(T const& str, char sep)
{
    return str
        | std::views::split(sep)
        | std::views::transform([](auto const t) { return std::string_view(t); });
}

std::string getColumn(std::vector<std::string>& rows, size_t const column)
{
    return rows
        | std::views::transform([column](auto line) { return line[column]; })
        | std::ranges::to<std::string>();
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto rows = split(input, '\n')
        | std::views::transform([](auto const row) { return std::string(row); })
        | std::ranges::to<std::vector>();
    auto columns = std::views::iota(0, static_cast<int>(rows.size()))
        | std::views::transform([&rows](auto const column) { return getColumn(rows, column); })
        | std::ranges::to<std::vector>();

    auto answer = 0ULL;
    for (auto& column : columns)
    {
        for (size_t i = 0; i < column.length(); i++)
        {
            if (column[i] == '.' && i < column.length() - 1)
            {
                auto j = i + 1;
                auto current = column[j];
                while (current == '.' && j < column.length())
                {
                    current = column[++j];
                }

                if (current == 'O')
                {
                    column[i] = 'O';
                    column[j] = '.';
                }

                if (current == '#')
                {
                    i = j;
                }
            }

            if (column[i] == 'O')
            {
                answer += column.length() - i;
            }
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}