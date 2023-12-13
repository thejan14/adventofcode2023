#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

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

size_t getMirrorSize(std::vector<std::string> const& view)
{
    for (size_t i = 1; i < view.size(); i++)
    {
        auto isMirror = true;
        auto const end = std::min(i, view.size() - i);
        size_t j = 0;
        while (isMirror && j < end)
        {
            isMirror &= view[i - j - 1] == view[i + j];
            ++j;
        }

        if (isMirror)
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

    auto answer = 0ULL;
    auto const sanitized = input.substr(0, input.length() - 1);
    for (auto const pattern : split(sanitized, "\n\n"))
    {
        answer += processPattern(pattern);
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}