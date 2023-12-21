#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <map>
#include <functional>

#include "aoc.h"

struct Part
{
    int x, m, a, s;

    int getRatingSum() const
    {
        return x + m + a + s;
    }
};

struct Workflow
{
    std::vector<std::function<std::pair<bool, std::string>(Part const)>> rules;

    std::string processPart(Part const part) const
    {
        for (auto const subRule : this->rules)
        {
            auto const [applies, target] = subRule(part);
            if (applies)
            {
                return target;
            }
        }

        throw std::domain_error("invalid rule");
    }
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

std::pair<std::string, Workflow> parseWorkflow(std::string_view line)
{
    auto workflow = Workflow{};
    auto const bracketPos = line.find('{');
    auto const name = std::string{ line.substr(0, bracketPos) };
    auto const rules = line.substr(bracketPos + 1, line.size() - bracketPos - 2);
    for (auto const rule : split(rules, ','))
    {
        if (auto const colonPos = rule.find(':'); colonPos != -1)
        {
            auto const rating = rule[0];
            auto const limit = svtoi(rule.substr(2, colonPos));
            auto const target = std::string{ rule.substr(colonPos + 1) };
            std::function<bool(int const a, int const b)> const op = rule[1] == '<'
                ? [](int const a, int const b) { return a < b; }
                : [](int const a, int const b) { return a > b; };
            switch (rating)
            {
                case 'x':
                    workflow.rules.emplace_back([op, limit, target](Part const p) { return std::make_pair(op(p.x, limit), target); });
                    break;
                case 'm':
                    workflow.rules.emplace_back([op, limit, target](Part const p) { return std::make_pair(op(p.m, limit), target); });
                    break;
                case 'a':
                    workflow.rules.emplace_back([op, limit, target](Part const p) { return std::make_pair(op(p.a, limit), target); });
                    break;
                case 's':
                    workflow.rules.emplace_back([op, limit, target](Part const p) { return std::make_pair(op(p.s, limit), target); });
                    break;
                default:
                    throw std::invalid_argument("rating category does not exist");
            }
        }
        else
        {
            auto const target = std::string{ rule };
            workflow.rules.emplace_back([target](Part const p) { return std::make_pair(true, target); });
        }
    }

    return std::make_pair(name, workflow);
}

Part parsePart(std::string_view const line)
{
    auto const ratings = split(line.substr(1, line.size() - 2), ',')
        | std::views::transform([](auto const r) { return svtoi(r.substr(2)); })
        | std::ranges::to<std::vector>();
    return {
        ratings[0],
        ratings[1],
        ratings[2],
        ratings[3],
    };
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto const sepPos = input.find("\n\n");
    auto const rulesInput = input.substr(0, sepPos);
    auto const partsInput = input.substr(sepPos + 2);
    auto workflows = std::map<std::string, Workflow>();
    for (auto const line : split(rulesInput, '\n'))
    {
        workflows.insert(parseWorkflow(line));
    }

    auto answer = 0;
    for (auto const line : split(partsInput, '\n'))
    {
        auto const part = parsePart(line);
        auto currentWorkflow = std::string{ "in" };
        while (currentWorkflow != "A" && currentWorkflow != "R")
        {
            currentWorkflow = workflows.at(currentWorkflow).processPart(part);
        }

        if (currentWorkflow == "A")
        {
            answer += part.getRatingSum();
        }
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}
