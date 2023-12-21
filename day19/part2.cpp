#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <map>
#include <algorithm>

#include "aoc.h"

struct Rule
{
    char rating;
    char op;
    int limit;
    std::string target;
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

std::pair<std::string, std::vector<Rule>> parseWorkflow(std::string_view line)
{
    auto workflow = std::vector<Rule>{};
    auto const bracketPos = line.find('{');
    auto const name = std::string{ line.substr(0, bracketPos) };
    auto const rules = line.substr(bracketPos + 1, line.size() - bracketPos - 2);
    for (auto const rule : split(rules, ','))
    {
        if (auto const colonPos = rule.find(':'); colonPos != -1)
        {
            auto const rating = rule[0];
            auto const op = rule[1];
            auto const limit = svtoi(rule.substr(2, colonPos));
            auto const target = std::string{ rule.substr(colonPos + 1) };
            workflow.emplace_back(rating, op, limit, target);
        }
        else
        {
            auto const target = std::string{ rule };
            workflow.emplace_back('_', '_', 0, target);
        }
    }

    return std::make_pair(name, workflow);
}

unsigned long long getAccepted(std::map<std::string, std::vector<Rule>> const& workflows, std::map<char, std::pair<int, int>>& ranges, std::string const current)
{
    if (current == "A")
    {
        return std::ranges::fold_left(ranges | std::views::values, 1ULL, [](auto acc, auto const range) { return acc * ((range.second - range.first) + 1); });
    }
    else if (current == "R")
    {
        return 0;
    }
    
    auto accepted = 0ULL;
    for (auto const& [rating, op, limit, target] : workflows.at(current))
    {
        if (rating == '_')
        {
            accepted += getAccepted(workflows, ranges, target);
        }
        else
        {
            auto const& [from, to] = ranges[rating];
            if (from < limit && to > limit)
            {
                auto newRanges = ranges;
                if (op == '<')
                {
                    newRanges[rating] = std::make_pair(from, limit - 1);
                    ranges[rating] = std::make_pair(limit, to);
                }
                else
                {
                    newRanges[rating] = std::make_pair(limit + 1, to);
                    ranges[rating] = std::make_pair(from, limit);
                }

                accepted += getAccepted(workflows, newRanges, target);
            }
        }
    }

    return accepted;
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto const sepPos = input.find("\n\n");
    auto const rulesInput = input.substr(0, sepPos);
    auto workflows = std::map<std::string, std::vector<Rule>>();
    for (auto const line : split(rulesInput, '\n'))
    {
        workflows.insert(parseWorkflow(line));
    }

    auto ranges = std::map<char, std::pair<int, int>>{
        { 'x', { 1, 4000 } },
        { 'm', { 1, 4000 } },
        { 'a', { 1, 4000 } },
        { 's', { 1, 4000 } },
    };
    
    auto const answer = getAccepted(workflows, ranges, "in");

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}
