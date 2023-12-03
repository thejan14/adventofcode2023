#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>

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
        auto first = -1;
        auto last = -1;
        for (char const& c : line)
        {
            if (c >= '0' && c <= '9')
            {
                if (first == -1)
                {
                    first = c - '0';
                }

                last = c - '0';
            }
        }

        answer += first * 10 + last;
    }

    /* end solution */

    const auto execEnd = std::chrono::high_resolution_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds> (execEnd - execStart);
    std::cout << std::format("{} ({})", answer, millis);
}