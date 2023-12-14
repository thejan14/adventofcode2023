#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <map>
#include <ranges>
#include <regex>

#include "aoc.h"

using std::operator""sv;
using llong = long long;

struct Mapping
{
    std::string_view to;
    std::vector<std::pair<llong, llong>> destinationRanges;
    std::vector<std::pair<llong, llong>> sourceRanges;

    llong getDestination(llong source)
    {
        for (size_t i = 0; i < this->sourceRanges.size(); i++)
        {
            auto const& [start, end] = this->sourceRanges[i];
            if (source >= start && source < end)
            {
                auto const delta = source - start;
                auto const [destinationStart, _] = this->destinationRanges[i];
                return destinationStart + delta;
            }
        }

        return source;
    }
};

static llong svtoll(std::string_view const& sv)
{
    llong value;
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{})
    {
        return value;
    }

    throw std::invalid_argument("not a number");
}

std::vector<llong> parseSeedNrs(std::string const& idsStr)
{
    auto nrs = std::vector<llong>{};
    for (auto const nrToken : std::views::split(std::string_view(idsStr), " "sv))
    {
        nrs.push_back(svtoll(std::string_view(nrToken)));
    }

    return nrs;
}

constexpr Mapping parseMapping(std::string_view const& mappingStr)
{
    auto mapping = Mapping{};
    auto const dashSep = mappingStr.find_last_of('-');
    mapping.to = mappingStr.substr(dashSep + 1, mappingStr.find(' ') - dashSep - 1);
    for (auto const rangeDescrToken : std::views::split(mappingStr, "\n"sv) | std::views::drop(1))
    {
        // the trailing new line at the very end of the input will cause an empty token here for the last mapping
        auto rangeDescrSv = std::string_view(rangeDescrToken);
        if (!rangeDescrSv.empty())
        {
            auto const whitespace1 = rangeDescrSv.find_first_of(' ');
            auto const whitespace2 = rangeDescrSv.find_last_of(' ');
            auto const destination = svtoll(rangeDescrSv.substr(0, whitespace1));
            auto const source = svtoll(rangeDescrSv.substr(whitespace1 + 1, whitespace2 - whitespace1 - 1));
            auto const length = svtoll(rangeDescrSv.substr(whitespace2 + 1));
            mapping.destinationRanges.emplace_back(destination, destination + length);
            mapping.sourceRanges.emplace_back(source, source + length);
        }
    }

    return mapping;
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto const colonPos = input.find(':');
    auto const eolPos = input.find('\n');
    auto const seedNrs = parseSeedNrs(input.substr(colonPos + 2, eolPos - colonPos));

    auto mappings = std::map<std::string_view, Mapping>{};
    for (auto const mappingToken : std::views::split(std::string_view(input), "\n\n"sv) | std::views::drop(1))
    {
        auto const mappingSv = std::string_view(mappingToken);
        auto const source = mappingSv.substr(0, mappingSv.find_first_of('-'));
        mappings[source] = parseMapping(mappingSv);
    }

    auto answer = LLONG_MAX;
    for (auto nr : seedNrs)
    {
        auto currentInfo = "seed"sv;
        auto currentMapping = mappings[currentInfo];
        while (currentInfo != "location"sv)
        {
            nr = currentMapping.getDestination(nr);
            currentInfo = currentMapping.to;
            currentMapping = mappings[currentInfo];
        }

        if (nr < answer)
        {
            answer = nr;
        }
    }


    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}