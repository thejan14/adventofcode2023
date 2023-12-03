#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>

using std::operator""sv;

auto constexpr MAX_R = 12;
auto constexpr MAX_G = 13;
auto constexpr MAX_B = 14;

struct CubeBag
{
    int r, g, b;
};

static constexpr std::optional<int> svtoi(std::string_view const& sv)
{
    int value;
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{})
    {
        return value;
    }

    return std::nullopt;
}

constexpr bool tryAddCubes(std::string_view const& cubeColor, int const cubeCount, CubeBag& bag)
{
    if (cubeColor == "red")
    {
        bag.r += cubeCount;
        return bag.r <= MAX_R;
    }

    if (cubeColor == "green")
    {
        bag.g += cubeCount;
        return bag.g <= MAX_G;
    }

    if (cubeColor == "blue")
    {
        bag.b += cubeCount;
        return bag.b <= MAX_B;
    }

    return false;
}

bool parseAndCheckGame(std::string const& setInfosToken)
{
    for (auto const setToken : std::views::split(std::string_view(setInfosToken), "; "sv))
    {
        auto bag = CubeBag{};
        for (auto const cubeToken : std::views::split(std::string_view(setToken), ", "sv))
        {
            auto const cubeTokenSv = std::string_view(cubeToken);
            auto const whitespacePos = cubeTokenSv.find(' ');
            auto const cubeCount = svtoi(cubeTokenSv.substr(0, whitespacePos)).value_or(0);
            auto const cubeColor = cubeTokenSv.substr(whitespacePos + 1, cubeTokenSv.size() - whitespacePos + 1);
            if (!tryAddCubes(cubeColor, cubeCount, bag))
            {
                return false;
            }
        }
    }

    return true;
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

    auto answer = 0;
    std::string line;
    auto parseStream = std::stringstream(input);
    while (getline(parseStream, line))
    {
        auto const colonPos = line.find(':');
        auto const gameToken = line.substr(0, colonPos);
        auto const setInfosToken = line.substr(colonPos + 2); // skip trailing whitespace after colon
        if (parseAndCheckGame(setInfosToken))
        {
            auto const gameId = stoi(gameToken.substr(5)); // skip "Game" and whitespace
            answer += gameId;
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, millis);
}