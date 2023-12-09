#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <array>
#include <map>
#include <ranges>
#include <set>

enum Tier
{
    HIGH = 0,
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FULL = 4,
    FOUR = 5,
    FIVE = 6,
};

struct Hand
{
    std::array<int, 5> cards;
    Tier tier;
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
        case 'T': return 10;
        case 'J': return 1;
        default: return c - '0';
    }
}

// thanks to charlescochran for inspiration
// https://github.com/charlescochran/aoc-2023/blob/main/day7.cpp
inline Tier getHandTier(size_t const unique, int const maxSame, int const joker)
{
    switch (unique)
    {
        case 2: return maxSame + joker == 4 ? FOUR : FULL;
        case 3: return maxSame + joker == 3 ? THREE : TWO;
        case 4: return ONE;
        case 5: return HIGH;
        default: return FIVE;
    }
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

    auto hands = std::set<Hand>{};
    std::string line;
    auto parseStream = std::stringstream(input);
    while (getline(parseStream, line))
    {
        auto const sep = line.find(' ');
        auto hand = Hand{};
        hand.bid = std::stoi(line.substr(sep + 1));

        auto joker = 0;
        auto cardCounts = std::map<int, int>{};
        auto const cards = line.substr(0, sep);
        for (auto i = 0; i < 5; i++)
        {
            hand.cards[i] = getCardValue(cards[i]);
            if (hand.cards[i] == 1)
            {
                ++joker;
            }
            else
            {
                cardCounts[hand.cards[i]] += 1;
            }
        }

        if (joker == 5)
        {
            hand.tier = FIVE;
        }
        else
        {
            auto const maxSame = std::ranges::max(cardCounts | std::views::values);
            hand.tier = getHandTier(cardCounts.size(), maxSame, joker);
        }

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