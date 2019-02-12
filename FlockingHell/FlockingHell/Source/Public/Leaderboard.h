#pragma once
#include "Stats.h"
#include <string>

struct Leaderboard
{
	Leaderboard();

	void RetrieveData();
	void AddEntry(const Stats& Stats);

	void Sort();

	void Update();
	void Draw();

	unsigned short Rank{};
	std::string Name{};
	unsigned int Score{};
	unsigned int GrazingScore{};
	unsigned short Deaths{};
	unsigned short BombsUsed{};
	unsigned short FlawlessRuns{};

	std::string RawData{};
};

