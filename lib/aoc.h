#pragma once
#include <fstream>
#include <string>

namespace aoc
{
    static std::string readInput() {
        auto input = std::string{};
        if (auto readStream = std::ifstream("input.txt"); readStream.is_open())
        {
            input = std::string(
                std::istreambuf_iterator(readStream),
                std::istreambuf_iterator<char>());

            if (input.find("\r\n") != std::string::npos) {
                throw std::invalid_argument("input appears to be separated with windows style line endings!");
            }

            if (input[input.size() - 1] == '\n') {
                throw std::invalid_argument("input contains trailing new line!");
            }
        }

        return input;
    }
}