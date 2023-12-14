#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

#include "aoc.h"

static long long svtoll(std::string_view const& sv)
{
    long long value;
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
        | std::views::transform([](auto const t) {return std::string_view(t); });
}

long long extrapolateSequence(std::vector<long long>& sequence)
{
    size_t depth = 1;
    auto done = bool{};
    do
    {
        done = true;
        for (size_t i = 0; i < sequence.size() - depth; i++)
        {
            sequence[i] = sequence[i + 1] - sequence[i];
            done &= sequence[i] == 0;
        }

        ++depth;
    } while (!done);

    for (auto i = sequence.size() - --depth; i < sequence.size(); i++)
    {
        sequence[i] = sequence[i - 1] + sequence[i];
    }

    return sequence.back();
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto answer = 0LL;

    for (auto const line : split(input, '\n'))
    {
        auto sequence = split(line, ' ')
            | std::views::transform([](auto const t) { return svtoll(t); })
            | std::ranges::to<std::vector>();

        std::ranges::reverse(sequence);
        answer += extrapolateSequence(sequence);
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}