#include "Leaderboard.h"
#include "Assets.h"
#include <algorithm>
#define ASSETS Assets::Get()
#define GetFont(Name) ASSETS.GetFont(#Name)

Leaderboard::Leaderboard() = default;

void Leaderboard::RetrieveData()
{
}

void Leaderboard::CreateEntry(const Stats &Stats)
{
	if (Entries.size() < MAX_SLOTS)
	{
		Entry = new Slot(Stats);
		Entry->YOffset += Offset;
		Entries.emplace_back(Entry);

		for (unsigned short i = 0; i < Entries.size(); i++)
			Scores[i] = Entries[i]->Score;

		Offset += 40;
	}
	else
	{
		if (Stats.Highscore > Entries[9]->Score)
		{
			Offset -= 40;

			Entries.erase(Entries.begin()+9);

			Entry = new Slot(Stats);
			Entry->YOffset += Offset;
			Entries.emplace_back(Entry);

			for (unsigned short i = 0; i < Entries.size(); i++)
				Scores[i] = Entries[i]->Score;
		}
	}
}

void Leaderboard::Sort()
{
	// Sort in decending order
	std::sort(std::begin(Scores), std::end(Scores));
	std::reverse(std::begin(Scores), std::end(Scores));

	// Update the leaderboard with the new data
	for (unsigned short i = 0; i < Entries.size(); i++)
		SlotMap[i+1] = Scores[i];

	//for(i=0;i<=10;i++)
	//{
	//	for(j=0;j<=10-i;j++)
	//	{
	//		if(a[j]>a[j+1])
	//		{
	//			temp=a[j];
	//			a[j]=a[j+1];
	//			a[j+1]=temp;
	//		}
	//	}
	//}

	// Update the leaderboard with the new data
	//for (unsigned short i = 0; i < Entries.size()-1; i++)
	//{
	//	if (Entries[i]->Score < Scores[i] && !Entries[i]->bSwapped)
	//	{
	//		Entries[i]->YOffset = Entries[i]->YOffset + (i+1) * Offset/2;
	//		Entries[i]->Score = Scores[i+1];
	//		Entries[i+1]->YOffset = Entries[i+1]->YOffset - (i+1) * Offset/2;
	//		Entries[i+1]->Score = Scores[i];
	//		Entries[i]->bSwapped = true;
	//		Entries[i+1]->bSwapped = true;
	//	}
	//}
}

void Leaderboard::UpdateEntry(const Stats &Stats)
{
	Entry->Score = Stats.Highscore;
	Entry->GrazingScore = Stats.HighGrazingScore;
	Entry->Deaths = Stats.LifetimeDeaths;
	Entry->BombsUsed = Stats.LifetimeBombsUsed;
	Entry->FlawlessRuns = Stats.TotalFlawlessRuns;
}

void Leaderboard::Draw()
{
	for (unsigned short i = 0; i < Entries.size(); i++)
	{
		// Name
		RDrawTextEx(GetFont(VCR), "Kratos", {Entries[i]->Location.x, Entries[i]->Location.y + Entries[i]->YOffset}, 20.0f, 0.0f, YELLOW);
		// Score
		RDrawTextEx(GetFont(VCR), FormatText("%06i", Entries[i]->Score), {Entries[i]->Location.x + 150.0f, Entries[i]->Location.y + Entries[i]->YOffset}, 20.0f, 0.0f, WHITE);
		// Grazing score
		RDrawTextEx(GetFont(VCR), FormatText("%06i", Entries[i]->GrazingScore), {Entries[i]->Location.x + 300.0f, Entries[i]->Location.y + Entries[i]->YOffset}, 20.0f, 0.0f, WHITE);
		// Deaths
		RDrawTextEx(GetFont(VCR), FormatText("%04i", Entries[i]->Deaths), {Entries[i]->Location.x + 455.0f, Entries[i]->Location.y + Entries[i]->YOffset}, 20.0f, 0.0f, WHITE);
		// Bombs Used
		RDrawTextEx(GetFont(VCR), FormatText("%04i", Entries[i]->BombsUsed), {Entries[i]->Location.x + 595.0f, Entries[i]->Location.y + Entries[i]->YOffset}, 20.0f, 0.0f, WHITE);
		// Flawless runs
		RDrawTextEx(GetFont(VCR), FormatText("%03i", Entries[i]->FlawlessRuns), {Entries[i]->Location.x + 735.0f, Entries[i]->Location.y + Entries[i]->YOffset}, 20.0f, 0.0f, WHITE);
	}
}

Slot::Slot(const Stats &Stats)
{
	Score = Stats.Highscore;
	GrazingScore = Stats.HighGrazingScore;
	Deaths = Stats.LifetimeDeaths;
	BombsUsed = Stats.LifetimeBombsUsed;
	FlawlessRuns = Stats.TotalFlawlessRuns;
}
