#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <array>
#include <map>
#include <ranges>
#include <set>

using pos = std::pair<int, int>;

constexpr int DIM = 140;

// https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

long long getDistance(std::array<bool, DIM> const& galaxyRows, std::array<bool, DIM> const& galaxyColumns, pos const& galaxy1, pos  const& galaxy2)
{
	auto distance = 0LL;
	auto const [gi1, gj1] = galaxy1;
	auto const [gi2, gj2] = galaxy2;

	auto const rowSign = sgn(gi2 - gi1);
	for (auto i = gi1; i != gi2; i += rowSign)
	{
		distance += galaxyRows[i] ? 1 : 1'000'000;
	}

	auto const columnSign = sgn(gj2 - gj1);
	for (auto j = gj1; j != gj2; j += columnSign)
	{
		distance += galaxyColumns[j] ? 1 : 1'000'000;
	}

	return distance;
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

	auto galaxies = std::map<int, pos>{};
	auto galaxyRows = std::array<bool, DIM>{};
	auto galaxyColumns = std::array<bool, DIM>{};
	auto g = 0; // running galaxy index
	for (auto i = 0; i < DIM; i++)
	{
		for (auto j = 0; j < DIM; j++)
		{
			auto const c = input[i * (DIM + 1) + j];
			if (c == '#')
			{
				galaxies[g++] = { i, j };
				galaxyRows[i] = true;
				galaxyColumns[j] = true;
			}
		}
	}

	auto pairs = std::set<std::pair<int, int>>{};
	for (auto const id1 : galaxies | std::views::keys)
	{
		for (auto const id2 : galaxies | std::views::keys)
		{
			if (id1 != id2)
			{
				pairs.emplace(std::min(id1, id2), std::max(id1, id2));
			}
		}
	}

	auto answer = 0LL;
	for (auto const [galaxy1, galaxy2] : pairs)
	{
		answer += getDistance(galaxyRows, galaxyColumns, galaxies[galaxy1], galaxies[galaxy2]);
	}

	/* end solution */

	const auto execEnd = std::chrono::high_resolution_clock::now();
	const auto micros = std::chrono::duration_cast<std::chrono::microseconds> (execEnd - execStart);
	std::cout << std::format("{} ({})", answer, micros);
}