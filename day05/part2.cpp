#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <map>
#include <ranges>
#include <regex>

using std::operator""sv;
using llong = long long;

struct Mapping
{
    std::string_view to;
    std::vector<std::pair<llong, llong>> destinationRanges;
    std::vector<std::pair<llong, llong>> sourceRanges;
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

std::vector<std::pair<llong, llong>> parseSeedNrs(std::string const& idsStr)
{
    auto nrs = std::vector<std::pair<llong, llong>>{};
    auto start = -1ll;
    for (auto const nrToken : std::views::split(std::string_view(idsStr), " "sv))
    {
        if (start == -1)
        {
            start = svtoll(std::string_view(nrToken));
        }
        else
        {
            auto const length = svtoll(std::string_view(nrToken));
            nrs.emplace_back(start, start + length);
            start = -1;
        }
    }

    std::ranges::sort(nrs, std::ranges::less());
    return nrs;
}

Mapping parseMapping(std::string_view const& mappingStr)
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
            mapping.destinationRanges.emplace_back(destination, destination + length - 1);
            mapping.sourceRanges.emplace_back(source, source + length - 1);
        }
    }

    auto sorting = std::views::iota(0) | std::views::take(mapping.sourceRanges.size()) | std::ranges::to<std::vector>();
    std::ranges::sort(sorting, [&mapping](auto& a, auto& b) { return mapping.sourceRanges[a] < mapping.sourceRanges[b]; });
    mapping.destinationRanges = sorting | std::views::transform([&mapping](auto& x) { return mapping.destinationRanges[x]; }) | std::ranges::to<std::vector>();
    mapping.sourceRanges = sorting | std::views::transform([&mapping](auto& x) { return mapping.sourceRanges[x]; }) | std::ranges::to<std::vector>();
    return mapping;
}

void addMapped(std::vector<std::pair<llong, llong>>& newDestinations, llong newDestStart, llong srcStart, llong newStart, llong newEnd)
{
    auto delta = newDestStart - srcStart;
    newDestinations.emplace_back(newStart + delta, newEnd + delta);
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

    auto const colonPos = input.find(':');
    auto const eolPos = input.find('\n');
    auto seedRanges = parseSeedNrs(input.substr(colonPos + 2, eolPos - colonPos));

    auto mappings = std::map<std::string_view, Mapping>{};
    for (auto const mappingToken : std::views::split(std::string_view(input), "\n\n"sv) | std::views::drop(1))
    {
        auto const mappingSv = std::string_view(mappingToken);
        auto const source = mappingSv.substr(0, mappingSv.find_first_of('-'));
        mappings[source] = parseMapping(mappingSv);
    }

    auto nextLocation = "seed"sv;
    while (mappings.contains(nextLocation))
    {
        auto mappedSeeds = std::vector<std::pair<llong, llong>>{};
        auto nextMapping = mappings[nextLocation];
        for (size_t i = 0; i < nextMapping.sourceRanges.size(); i++)
        {
            auto const [destStart, destEnd] = nextMapping.sourceRanges[i];
            auto const [newDestStart, _] = nextMapping.destinationRanges[i];
            auto const destDelta = newDestStart - destStart;
            for (auto srcIt = seedRanges.begin(); srcIt != seedRanges.end();)
            {
                auto const [srcStart, srcEnd] = *srcIt;
                srcIt = seedRanges.erase(srcIt);
                if (srcStart < destEnd && srcEnd >= destStart)
                {
                    auto const subStart = std::max(srcStart, destStart);
                    auto const subEnd = std::min(srcEnd, destEnd - 1);
                    mappedSeeds.emplace_back(subStart + destDelta, subEnd + destDelta); // remapped
                }

                if (srcStart < destStart)
                {
                    srcIt = std::next(seedRanges.emplace(srcIt, srcStart, std::min(srcEnd, destStart - 1)));
                }

                if (srcEnd >= destEnd)
                {
                    srcIt = std::next(seedRanges.emplace(srcIt, std::max(srcStart, destEnd), srcEnd));
                }
            }
        }

        seedRanges.insert(seedRanges.end(), mappedSeeds.begin(), mappedSeeds.end());
        nextLocation = nextMapping.to;
    }



    auto answer = std::ranges::min(seedRanges | std::views::keys);


    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, millis);
}