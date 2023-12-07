#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <iostream>
#include <ranges>

using std::operator""sv;

static long long svtoll(std::string_view const& sv)
{
    long long value;
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{})
    {
        return value;
    }

    throw std::invalid_argument("not a number");
}

constexpr std::vector<long long> parseNumbers(std::string_view const& input)
{
    return std::views::split(input, " "sv)
        | std::views::drop(1)
        | std::views::transform([](auto token) { return std::string_view(token); })
        | std::views::filter([](auto svToken) { return !svToken.empty(); })
        | std::views::transform([](auto svToken) { return svtoll(svToken); })
        | std::ranges::to<std::vector>();
}

constexpr auto parseRaceData(std::string_view const& times, std::string_view const& distances)
{
    return std::views::zip(
        parseNumbers(times),
        parseNumbers(distances));
}

constexpr long long calcTravelDistance(long long const holdTime, long long const totalTime)
{
    // travel time: totalTime - holdTime
    // speed (mm/ms): hold time (1 mm per 1 ms)
    // total travel distance: travel time * speed
    return (totalTime - holdTime) * holdTime;
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

    auto answer = 1;
    auto const eolPos = input.find('\n');
    for (auto const [raceTime, recordDistance] : parseRaceData(input.substr(0, eolPos), input.substr(eolPos + 1)))
    {
        auto possibleWins = long long{ 0 };
        for (auto holdTime = 1; holdTime < raceTime; holdTime++)
        {
            if (calcTravelDistance(holdTime, raceTime) > recordDistance)
            {
                ++possibleWins;
            }
        }

        if (possibleWins > 0)
        {
            answer *= possibleWins;
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, millis);
}