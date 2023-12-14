﻿#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

#include "aoc.h"

template <typename T>
auto split(T const& str, std::string const& sep)
{
    return str
        | std::views::split(sep)
        | std::views::transform([](auto const t) { return std::string_view(t); });
}

std::string getColumn(std::vector<std::string> const& rows, size_t const column)
{
    return rows
        | std::views::transform([column](auto const& line) { return line[column]; })
        | std::ranges::to<std::string>();
}

int getDiff(std::string const& a, std::string const& b)
{
    auto diff = 0;
    for (size_t i = 0; i < a.length(); i++)
    {
        diff += a[i] != b[i];
    }

    return diff;
}

size_t getMirrorSize(std::vector<std::string> const& view)
{
    for (size_t i = 1; i < view.size(); i++)
    {
        auto mirrorDiffs = 0;
        auto const end = std::min(i, view.size() - i);
        size_t j = 0;
        while (mirrorDiffs < 2 && j < end)
        {
            mirrorDiffs += getDiff(view[i - j - 1], view[i + j]);
            ++j;
        }

        // mirror image is off by exactly one (smudge)
        if (mirrorDiffs == 1)
        {
            return i;
        }
    }

    return 0;
}

size_t processPattern(std::string_view const& pattern)
{
    auto const rows = split(pattern, "\n")
        | std::views::transform([](auto const row) { return std::string(row); })
        | std::ranges::to<std::vector>();
    auto const rowMirror = getMirrorSize(rows);
    if (rowMirror != 0)
    {
        return 100ULL * rowMirror;
    }

    auto const columns = std::views::iota(0, static_cast<int>(rows[0].size()))
        | std::views::transform([&rows](auto const column) { return getColumn(rows, column); })
        | std::ranges::to<std::vector>();
    auto const columnMirror = getMirrorSize(columns);
    if (columnMirror != 0)
    {
        return columnMirror;
    }

    throw std::invalid_argument("pattern has no reflection line");
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto answer = 0ULL;
    for (auto const pattern : split(input, "\n\n"))
    {
        answer += processPattern(pattern);
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}