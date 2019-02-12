#pragma once
#include <string>

struct Stats
{
	Stats();

	void Save();
	void Load();

	void UpdateTimePlayed();

	void Clear();

	std::string Name{};

	unsigned int LifetimeKills{};
	unsigned int LifetimeDeaths{};
	unsigned int LifetimeHits{};
	unsigned int LifetimeBombsUsed{};

	unsigned int FlyingEnemiesKilled{};
	unsigned int BossEnemiesKilled{};
	unsigned int TotalEnemiesKilled{};

	unsigned int Highscore{};
	unsigned int HighGrazingScore{};

	unsigned int TotalTimePlayedHrs{};
	unsigned int TotalTimePlayedMin{};
	unsigned int TotalTimePlayedSec{};
	unsigned int TotalGamesPlayed{};
	unsigned int TotalFlawlessRuns{};

	unsigned int FramesCounter{};

	bool bUpdated{false};
};

