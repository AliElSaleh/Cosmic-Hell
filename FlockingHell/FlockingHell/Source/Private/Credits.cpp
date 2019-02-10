#include "Credits.h"
#include "Assets.h"

#include <raymath.h>

#define ASSETS Assets::Get()
#define GetFont(Name) ASSETS.GetFont(#Name)
#define GetMusic(Name) ASSETS.GetMusic(#Name)

#define Stringify(Name) #Name

const char* Categories[]
{
	Stringify(PROGRAMMING & ART),
	Stringify(MUSIC)
};

Credits::Credits() = default;

void Credits::Init()
{
	Location = {float(GetScreenWidth())/2 - 180, float(GetScreenHeight()) + 100};

	Speed = 100.0f;
	Spacing = {0.0f, 0.0f};

	CreditsFont = GetFont(VCR);
	Music = GetMusic(Credits);

	PlayMusicStream(Music);
	SetMusicVolume(Music, Volume);
}

void Credits::AddName(const std::string& Name, const Category Category)
{
	List[Name] = Category;
}

void Credits::Update()
{
	Location.y -= Speed * GetFrameTime();

	if (Volume < 0.0f)
		Volume = 0.0f;
	else
		UpdateMusicStream(Music);

	if (IsOutsideWindow())
	{
		if (!bFadeOut)
		{
			if (Alpha < 2.5f)
			{
				Alpha += 0.01f;
			}
			else
			{
				bFadeOut = true;
			}
		}
		else
		{
			Alpha -= 0.01f;
			Volume -= 0.002f;
			SetMusicVolume(Music, Volume);
		}

		if (Alpha < -1.0f)
		{
			Alpha = 0.0f;
			Volume = 0.0f;
			bFinished = true;
		}
		
		bShowMessage = true;
	}
}

void Credits::Draw()
{	
	for (auto It = List.begin(); It != List.end(); ++It)
	{
		RDrawTextEx(CreditsFont, GetCategory(It->first).c_str(), Vector2Add(Location, {Spacing.x + CategorySpacing, Spacing.y}), float(CreditsFont.baseSize), 0.0f, WHITE); // Category
		Spacing.x += 50;
		Spacing.y += 50;
		RDrawTextEx(CreditsFont, It->first.c_str(), Vector2Add(Location, Spacing), float(CreditsFont.baseSize), 0.0f, WHITE); // Person's name
		Spacing.x = -20;
		Spacing.y += 100;
		CategorySpacing = 140; 
	}

	if (bShowMessage)
		RDrawTextEx(CreditsFont, "Thank you for playing!", {float(GetScreenWidth())/2 - MeasureText("Thank you for playing!", 20), float(GetScreenHeight())/2 - 50}, float(CreditsFont.baseSize), 0.0f, Fade(WHITE, Alpha));

	Spacing.x = 0.0f;
	Spacing.y = 0.0f;
	CategorySpacing = 0; 
}

std::string Credits::GetCategory(const std::string& Name)
{
	const auto Category = List[Name];

	switch (Category)
	{
		case Category::PROGRAMMING_ART:
			return Categories[0];

		case Category::MUSIC:
			return Categories[1];

		default:
		break;
	}

	return std::string();
}

bool Credits::IsOutsideWindow() const
{
	return Location.y < -200;
}
