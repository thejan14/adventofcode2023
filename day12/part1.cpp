﻿#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

#include "aoc.h"

struct ConditionRecord
{
    std::string_view row;
    std::vector<size_t> groups;
};

static size_t svtoi(std::string_view const& sv)
{
    size_t value;
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{})
    {
        return value;
    }

    throw std::invalid_argument("not a number");
}

template <typename T>
auto split(T const& str, char sep)
{
    return str
        | std::views::split(sep)
        | std::views::transform([](auto const t) { return std::string_view(t); });
}

bool couldBe(char c, char test)
{
    return c == test || c == '?';
}

/* Recursively advance through the input row character by character.
 * If '#' is found, start counting a group.
 * If '.' is found either stop counting the current group and advance
 * or simply advance (if no group is currently counted).
 * rp = current position inspected in row (row position)
 * gp = current group length checked (group position)
 * gl = length of currently inspected group (group length)
 * Thanks to jonathanpaulson: https://github.com/jonathanpaulson/AdventOfCode/blob/master/2023/12.py
 */
int getArrangements(ConditionRecord const& record, size_t const rp, size_t const gp, size_t const gl)
{
    if (rp == record.row.length())
    {
        // reached end of row, check if all groups were found and no additional group is currently counting
        if (gp == record.groups.size() && gl == 0)
        {
            return 1;
        }

        // if we are currently counting the last group check if it was completed on the last character
        if (gp == record.groups.size() - 1 && gl == record.groups[gp])
        {
            return 1;
        }

        // group arrangements could not be matched to the row input in this branch
        return 0;
    }

    auto arrangements = 0;
    if (couldBe(record.row[rp], '.'))
    {
        if (gl == 0)
        {
            // no counting group, no new group, advance row
            arrangements += getArrangements(record, rp + 1, gp, 0);
        }
        else if (gl > 0 && gp < record.groups.size() && record.groups[gp] == gl)
        {
            // group closed. Check if current group matches counted group. If so, advance row and inspect next group.
            arrangements += getArrangements(record, rp + 1, gp + 1, 0);
        }
    }

    if (couldBe(record.row[rp], '#'))
    {
        // start or continue counting current group, advance row
        arrangements += getArrangements(record, rp + 1, gp, gl + 1);
    }

    return arrangements;
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto records = std::vector<ConditionRecord>{};
    for (auto const line : split(input, '\n'))
    {
        auto const sep = line.find(' ');
        auto const row = line.substr(0, sep);
        auto const groups = split(line | std::views::drop(sep + 1), ',')
            | std::views::transform([](auto const n) { return svtoi(n); })
            | std::ranges::to<std::vector>();
        records.emplace_back(row, groups);
    }

    auto answer = 0;
    for (auto const& record : records)
    {
        answer += getArrangements(record, 0, 0, 0);
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}