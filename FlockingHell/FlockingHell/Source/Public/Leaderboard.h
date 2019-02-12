#pragma once

#include "Stats.h"

#include <string>
#include <vector>
#include <raylib.h>
#include <map>

constexpr unsigned short MAX_SLOTS = 10;

struct Slot
{
	explicit Slot(const Stats& Stats);

	Vector2 Location{160.0f, 262.0f};

	float YOffset{0.0f};

	unsigned short Rank{};
	std::string Name{};
	unsigned int Score{};
	unsigned int GrazingScore{};
	unsigned short Deaths{};
	unsigned short BombsUsed{};
	unsigned short FlawlessRuns{};

	bool bSwapped{false};
};

struct Leaderboard
{
	Leaderboard();
	~Leaderboard()
	{
		delete Entry;
	}

	void RetrieveData();
	void CreateEntry(const Stats& Stats);

	void Sort();

	void UpdateEntry(const Stats& Stats);
	void Draw();

	unsigned int Scores[10] = {0};
	float Offset{0.0f};

	Slot* Entry{};

	std::string RawData{};

	std::vector<Slot*> Entries{};
	std::map<unsigned short, unsigned int> SlotMap{}; // Rank - Score
};

