#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <algorithm>

#include "aoc.h"

constexpr auto BOX_COUNT = 256;

struct Lens
{
    std::string_view label;
    int focalLength;
};

static int svtoi(std::string_view const& sv)
{
    int value;
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

int getHash(auto const hash)
{
    auto result = 0;
    for (auto const c : hash)
    {
        result += c;
        result *= 17;
        result %= 256;
    }

    return result;
}

auto findLens(std::vector<Lens>& box, std::string_view const& label)
{
    return std::ranges::find_if(box, [&label](auto const& lens){ return lens.label == label; });
}

void processInstruction(std::string_view const& instruction, std::array<std::vector<Lens>, BOX_COUNT>& boxes)
{
    if (auto dashPos = instruction.find('-'); dashPos != std::string::npos)
    {
        auto const label = instruction.substr(0, dashPos);
        auto const hash = getHash(label);
        auto& box = boxes[hash];
        if (auto const labelPos = findLens(box, label); labelPos != box.end())
        {
            box.erase(labelPos);
        }
    }
    else if (auto eqlPos = instruction.find('='); eqlPos != std::string::npos)
    {
        auto const label = instruction.substr(0, eqlPos);
        auto const focalLength = svtoi(instruction.substr(eqlPos + 1));
        auto const hash = getHash(label);
        auto& box = boxes[hash];
        if (auto const labelPos = findLens(box, label); labelPos != box.end())
        {
            auto const it = box.erase(labelPos);
            box.emplace(it, label, focalLength);
        }
        else
        {
            box.emplace_back(label, focalLength);
        }
    }
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto boxes = std::array<std::vector<Lens>, BOX_COUNT>{};
    for (auto const& instruction : split(input, ','))
    {
        processInstruction(instruction, boxes);
    }

    auto answer = 0;
    for (auto boxNr = 0; boxNr < boxes.size(); boxNr++)
    {
        auto const& box = boxes[boxNr];
        auto slotNr = 1;
        for (auto const& [_, focalLength] : box)
        {
            answer += (1 + boxNr) * slotNr++ * focalLength;
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}