#include "Stats.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <raylib.h>

Stats::Stats() = default;

void Stats::Save()
{
	system("attrib +h +s -r Player.stats");

	std::ofstream SaveStats;
	SaveStats.open("Player.stats", std::ios::out | std::ios::binary | std::ios::app);

	if (SaveStats.fail())
	{
		std::cerr << "ERROR: Unable to save player stats.\n";
		exit(EXIT_FAILURE);
	}

	SaveStats.write(reinterpret_cast<char*>(&Highscore), sizeof(Highscore));
	SaveStats.write(reinterpret_cast<char*>(&HighGrazingScore), sizeof(HighGrazingScore));
	SaveStats.write(reinterpret_cast<char*>(&LifetimeKills), sizeof(LifetimeKills));
	SaveStats.write(reinterpret_cast<char*>(&LifetimeDeaths), sizeof(LifetimeDeaths));
	SaveStats.write(reinterpret_cast<char*>(&LifetimeHits), sizeof(LifetimeHits));
	SaveStats.write(reinterpret_cast<char*>(&LifetimeBombsUsed), sizeof(LifetimeBombsUsed));
	SaveStats.write(reinterpret_cast<char*>(&FlyingEnemiesKilled), sizeof(FlyingEnemiesKilled));
	SaveStats.write(reinterpret_cast<char*>(&BossEnemiesKilled), sizeof(BossEnemiesKilled));
	SaveStats.write(reinterpret_cast<char*>(&TotalEnemiesKilled), sizeof(TotalEnemiesKilled));
	SaveStats.write(reinterpret_cast<char*>(&TotalTimePlayedHrs), sizeof(TotalTimePlayedHrs));
	SaveStats.write(reinterpret_cast<char*>(&TotalTimePlayedMin), sizeof(TotalTimePlayedMin));
	SaveStats.write(reinterpret_cast<char*>(&TotalTimePlayedSec), sizeof(TotalTimePlayedSec));
	SaveStats.write(reinterpret_cast<char*>(&TotalGamesPlayed), sizeof(TotalGamesPlayed));
	SaveStats.write(reinterpret_cast<char*>(&TotalFlawlessRuns), sizeof(TotalFlawlessRuns));
	SaveStats.close();

	system("attrib +h +s +r Player.stats");
}

void Stats::Load()
{
	std::ifstream LoadStats;
	LoadStats.open("Player.stats", std::ios::in | std::ios::binary);

	if (LoadStats.fail())
	{
		std::cerr << "\nERROR: No stats file found.\n";
		std::cout << "INFO: Creating new stats file...\n";

		LoadStats.open("Player.stats", std::ios::in | std::ios::binary);

		std::cout << "INFO: File created successfully.\n";
	}

	LoadStats.seekg(0, std::ios::end);
	const int Size = int(LoadStats.tellg());
	LoadStats.seekg(0, std::ios::beg);

	while (LoadStats.tellg() < Size)
	{
		LoadStats.read(reinterpret_cast<char*>(&Highscore), sizeof(Highscore));
		LoadStats.read(reinterpret_cast<char*>(&HighGrazingScore), sizeof(HighGrazingScore));
		LoadStats.read(reinterpret_cast<char*>(&LifetimeKills), sizeof(LifetimeKills));
		LoadStats.read(reinterpret_cast<char*>(&LifetimeDeaths), sizeof(LifetimeDeaths));
		LoadStats.read(reinterpret_cast<char*>(&LifetimeHits), sizeof(LifetimeHits));
		LoadStats.read(reinterpret_cast<char*>(&LifetimeBombsUsed), sizeof(LifetimeBombsUsed));
		LoadStats.read(reinterpret_cast<char*>(&FlyingEnemiesKilled), sizeof(FlyingEnemiesKilled));
		LoadStats.read(reinterpret_cast<char*>(&BossEnemiesKilled), sizeof(BossEnemiesKilled));
		LoadStats.read(reinterpret_cast<char*>(&TotalEnemiesKilled), sizeof(TotalEnemiesKilled));
		LoadStats.read(reinterpret_cast<char*>(&TotalTimePlayedHrs), sizeof(TotalTimePlayedHrs));
		LoadStats.read(reinterpret_cast<char*>(&TotalTimePlayedMin), sizeof(TotalTimePlayedMin));
		LoadStats.read(reinterpret_cast<char*>(&TotalTimePlayedSec), sizeof(TotalTimePlayedSec));
		LoadStats.read(reinterpret_cast<char*>(&TotalGamesPlayed), sizeof(TotalGamesPlayed));
		LoadStats.read(reinterpret_cast<char*>(&TotalFlawlessRuns), sizeof(TotalFlawlessRuns));
	}
	
	LoadStats.close();
}

void Stats::UpdateTimePlayed()
{
	FramesCounter++;

	if (FramesCounter/GetFPS()%2)
	{
		FramesCounter = 0;
		TotalTimePlayedSec++;
	}

	if (TotalTimePlayedSec > 59)
	{
		TotalTimePlayedSec = 0;
		FramesCounter = 0;
		TotalTimePlayedMin++;
	}

	if (TotalTimePlayedMin > 59)
	{
		TotalTimePlayedMin = 0;
		TotalTimePlayedHrs++;
	}
}

void Stats::Clear()
{
	LifetimeKills = 0;
	LifetimeDeaths = 0;
	LifetimeHits = 0;
	LifetimeBombsUsed = 0;

	FlyingEnemiesKilled = 0;
	BossEnemiesKilled = 0;
	TotalEnemiesKilled = 0;

	Highscore = 0;
	HighGrazingScore = 0;

	TotalTimePlayedHrs = 0;
	TotalTimePlayedMin = 0;
	TotalTimePlayedSec = 0;
	TotalGamesPlayed = 0;
	TotalFlawlessRuns = 0;
}

