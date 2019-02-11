#pragma once

struct Stats
{
	Stats();

	void Save();
	void Load();

	void Update(const Stats& PlayerStats);

	unsigned int LifetimeKills{};
	unsigned int LifetimeDeaths{};
	unsigned int LifetimeHits{};
	unsigned int LifetimeBombsUsed{};

	unsigned int FlyingEnemiesKilled{};
	unsigned int BossEnemiesKilled{};
	unsigned int TotalEnemiesKilled{};

	unsigned int Highscore{};
	unsigned int HighGrazingScore{};

	unsigned int TotalTimePlayed{};
	unsigned int TotalGamesPlayed{};
	unsigned int TotalFlawlessRuns{};

	bool bUpdated{false};
};

