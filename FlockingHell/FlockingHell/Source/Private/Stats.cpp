#include "Stats.h"


Stats::Stats() = default;

void Stats::Save()
{
}

void Stats::Load()
{
}

void Stats::Update(const Stats & PlayerStats)
{
	Highscore = PlayerStats.Highscore;
	HighGrazingScore = PlayerStats.HighGrazingScore;

	LifetimeKills = PlayerStats.LifetimeKills;
	LifetimeDeaths = PlayerStats.LifetimeDeaths;
	LifetimeHits = PlayerStats.LifetimeHits;
	LifetimeBombsUsed = PlayerStats.LifetimeBombsUsed;

	FlyingEnemiesKilled = PlayerStats.FlyingEnemiesKilled;
	BossEnemiesKilled  = PlayerStats.BossEnemiesKilled;
	TotalEnemiesKilled = PlayerStats.TotalEnemiesKilled;

	TotalTimePlayed = PlayerStats.TotalTimePlayed;
	TotalGamesPlayed = PlayerStats.TotalGamesPlayed;
	TotalFlawlessRuns = PlayerStats.TotalFlawlessRuns;

	bUpdated = true;
}

