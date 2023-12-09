#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

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

    auto answer = 0LL;

    // remove trailing newline from input
    auto const sanitized = std::string_view(input).substr(0, input.size() - 1);
    for (auto const line : split(sanitized, '\n'))
    {
        auto sequence = split(line, ' ')
            | std::views::transform([](auto const t) { return svtoll(t); })
            | std::ranges::to<std::vector>();

        answer += extrapolateSequence(sequence);
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, millis);
}