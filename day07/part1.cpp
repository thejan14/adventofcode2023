#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <array>
#include <map>
#include <ranges>
#include <set>

#include "aoc.h"

struct Hand
{
    std::array<int, 5> cards;
    int tier;
    int bid;


    bool operator<(const Hand& hand) const
    {
        if (this->tier == hand.tier)
        {
            return std::tie(this->cards) < std::tie(hand.cards);
        }

        return this->tier < hand.tier;
    }
};

constexpr int getCardValue(char const c)
{
    switch (c)
    {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return 11;
        case 'T': return 10;
        default: return c - '0';
    }
}

constexpr int getHandTier(std::map<int, int> const& countMap)
{
    auto const counts = countMap | std::views::values | std::ranges::to<std::vector>();
    if (std::ranges::contains(counts, 5))
    {
        return 6;
    }

    if (std::ranges::contains(counts, 4))
    {
        return 5;
    }

    auto const pairs = std::ranges::count(counts, 2);
    if (std::ranges::contains(counts, 3))
    {
        if (pairs == 1)
        {
            return 4; // full house
        }

        return 3;
    }

    if (pairs == 2)
    {
        return 2;
    }

    if (pairs == 1)
    {
        return 1;
    }

    return 0;
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto hands = std::set<Hand>{};
    std::string line;
    auto parseStream = std::stringstream(input);
    while (getline(parseStream, line))
    {
        auto const sep = line.find(' ');
        auto hand = Hand{};
        hand.bid = std::stoi(line.substr(sep + 1));

        auto countMap = std::map<int, int>{};
        auto const cards = line.substr(0, sep);
        for (auto i = 0; i < 5; i++)
        {
            hand.cards[i] = getCardValue(cards[i]);
            countMap[hand.cards[i]] += 1;
        }

        hand.tier = getHandTier(countMap);
        hands.insert(hand);
    }

    auto answer = 0;
    auto rank = 1;
    for (auto hand : hands)
    {
        answer += hand.bid * rank++;
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}