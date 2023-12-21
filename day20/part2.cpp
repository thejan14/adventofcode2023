#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <ranges>
#include <map>
#include <set>
#include <queue>
#include <numeric>
#include <algorithm>

#include "aoc.h"

using std::operator""sv;

enum Pulse { LOW, HIGH };

struct Module
{
    std::vector<std::string_view> inputs;
    std::vector<std::string_view> outputs;
    explicit Module(std::vector<std::string_view> const& outputs) : outputs(outputs) {}
    virtual ~Module() = default;
    virtual std::optional<Pulse> processPulse(std::string_view const sender, Pulse const pulse)
    {
        return pulse;
    }
};

struct FlipFlopModule final : Module
{
    bool state = false;
    explicit FlipFlopModule(std::vector<std::string_view> const& outputs) : Module(outputs) {}
    std::optional<Pulse> processPulse(std::string_view const sender, Pulse const pulse) override
    {
        if (pulse == LOW)
        {
            this->state = !this->state;
            return std::make_optional(this->state ? HIGH : LOW);
        }

        return std::nullopt;
    }
};

struct ConjunctionModule final : Module
{
    std::map<std::string_view, Pulse> inputStates;
    explicit ConjunctionModule(std::vector<std::string_view> const& outputs) : Module(outputs) {}
    std::optional<Pulse> processPulse(std::string_view const sender, Pulse const pulse) override
    {
        this->inputStates[sender] = pulse;
        if (this->inputStates.size() == this->inputs.size() && std::ranges::all_of(this->inputStates | std::views::values, [](auto const p) { return p == HIGH; }))
        {
            return LOW;
        }

        return HIGH;
    }
};

template <typename T>
auto split(T const& str, std::string const& sep)
{
    return str
        | std::views::split(sep)
        | std::views::transform([](auto const t) { return std::string_view(t); });
}

std::map<std::string_view, std::unique_ptr<Module>> parseModules(std::string const& input)
{
    auto modules = std::map<std::string_view, std::unique_ptr<Module>>{};
    for (auto const line : split(input, "\n"))
    {
        auto const type = line[0];
        auto const arrowPos = line.find('-');
        auto const outputs = split(line.substr(arrowPos + 3), ", ") | std::ranges::to<std::vector>();
        if (type == '%')
        {
            auto const id = line.substr(1, arrowPos - 2);
            modules[id] = std::make_unique<FlipFlopModule>(outputs);
        }
        else if (type == '&')
        {
            auto const id = line.substr(1, arrowPos - 2);
            modules[id] = std::make_unique<ConjunctionModule>(outputs);
        }
        else
        {
            auto const id = line.substr(0, arrowPos - 1);
            modules[id] = std::make_unique<Module>(outputs);
        }
    }

    for (auto const& [name, module] : modules)
    {
        for (auto output : module->outputs)
        {
            if (auto const it = modules.find(output); it != modules.end())
            {
                auto const& [_, targetModule] = *it;
                targetModule->inputs.push_back(name);
            }
        }
    }

    return modules;
}

std::map<std::string_view, unsigned long long> computeButtonPressesForSignals(std::map<std::string_view, std::unique_ptr<Module>>& modules, std::vector<std::tuple<std::string_view, std::string_view, Pulse>> signals)
{
    auto reachedAt = std::map<std::string_view, unsigned long long>{};
    auto pressed = 0ULL;
    while (!signals.empty())
    {
        ++pressed;
        auto signalQueue = std::queue<std::tuple<std::string_view, std::string_view, Pulse>>{};
        signalQueue.emplace("button"sv, "broadcaster"sv, LOW);
        while (!signalQueue.empty())
        {
            auto [fromModule, toModule, pulse] = signalQueue.front();
            signalQueue.pop();

            for (auto it = signals.begin(); it != signals.end();)
            {
                auto const& [targetFrom, targetTo, targetPulse] = *it;
                if (fromModule == targetFrom && toModule == targetTo && pulse == targetPulse)
                {
                    reachedAt[targetFrom] = pressed;
                    it = signals.erase(it);
                }
                else
                {
                    it = std::next(it);
                }
            }

            if (auto const it = modules.find(toModule); it != modules.end())
            {
                auto& [_, currentModule] = *it;
                auto const nextPulse = currentModule->processPulse(fromModule, pulse);
                if (nextPulse.has_value())
                {
                    for (auto const output : currentModule->outputs)
                    {
                        signalQueue.emplace(toModule, output, nextPulse.value());
                    }
                }
            }
        }
    }

    return reachedAt;
}

int main()
{
    auto input = aoc::readInput();
    const auto execStart = std::chrono::high_resolution_clock::now();

    /* begin solution */

    auto modules = parseModules(input);

    // find all modules outputting to rx
    auto const rxInputs = modules
        | std::views::filter([](auto const& m) { return std::ranges::contains(m.second->outputs, "rx"sv); })
        | std::views::keys
        | std::ranges::to<std::vector>();

    // assuming there is only one conjunction module outputting to rx
    auto const rxInput = rxInputs.front();
    auto const requiredSignals = modules[rxInput]->inputs
        | std::views::transform([&rxInput](auto const& name) { return std::make_tuple(name, rxInput, HIGH); })
        | std::ranges::to<std::vector>();

    // find how many button presses are required for each of the inputs to the rx conjunction module
    // then compute the LCM to determine when they will send HIGH all at the same time
    auto const answer = std::ranges::fold_left(
        computeButtonPressesForSignals(modules, requiredSignals) | std::views::values,
        1ULL,
        [](auto acc, auto const presses) { return std::lcm(acc, presses); });

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, micros);
}
