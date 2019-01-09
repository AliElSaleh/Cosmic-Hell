#include "SpiralPattern.h"
#include "Assets.h"

#define Stringify(name) #name

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

const char* SpiralPatternNames[]
{
	Stringify(SPIRAL RIGHT),
	Stringify(SPIRAL LEFT),
	Stringify(SPIRAL DOUBLE),
	Stringify(SPIRAL MULTI RIGHT),
	Stringify(SPIRAL MULTI LEFT),
	Stringify(SPIRAL MULTI DOUBLE),
};

SpiralPattern::SpiralPattern()
{
	SpiralPattern::Init();
}

void SpiralPattern::Init()
{
	BulletSprite = GetAsset(RedBullet);
	DummySprite = GetAsset(Alien);

	// Default values
	Bullet.clear();
	Angles.clear();
	Points.clear();

	ShootRate = 0;
	DummySpeed = 200.0f;
	bIsSpacePressed = false;
	bIsInProgress = false;

	switch (CurrentPattern)
	{
		case SPIRAL_RIGHT:
			CreateSpiralPattern(false, 200, 100.0f, 800.0f, 1.0f);
		break;

		case SPIRAL_LEFT:
			CreateSpiralPattern(false, 100, 150.0f, 200.0f, 1.0f);
		break;

		case SPIRAL_DOUBLE:
			CreateSpiralPattern(true, 400, 250.0f, 150.0f, 20.0f);
		break;

		default:
		break;
	}
}

void SpiralPattern::Update()
{
	if (!Bullet.empty() && bIsSpacePressed)
		bIsInProgress = true;
	else
		bIsInProgress = false;

	// Switch bullet types with arrow keys when not firing/in progress
	if (!bIsInProgress)
	{
		if (IsKeyPressed(KEY_LEFT))
		{
			if (static_cast<Pattern>(int(CurrentPattern-1)) < SPIRAL_RIGHT)
				SetBulletPattern(static_cast<Pattern>(SPIRAL_DOUBLE));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > SPIRAL_DOUBLE)
				SetBulletPattern(static_cast<Pattern>(SPIRAL_RIGHT));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
		}
	}

	switch (CurrentPattern)
	{
		case SPIRAL_RIGHT:
			UpdateSpiralBullet(false);
		break;

		case SPIRAL_LEFT:
			UpdateSpiralBullet(false);
		break;

		case SPIRAL_DOUBLE:
			UpdateSpiralBullet(true);
		break;

		default:
		break;
	}
}

void SpiralPattern::Draw()
{
	if (bDebug)
	{
		DrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);
		
		switch (CurrentPattern)
		{
			case SPIRAL_RIGHT:
				DrawDebugPoint();
			break;

			case SPIRAL_LEFT:
				DrawDebugPoint();
			break;

			case SPIRAL_DOUBLE:
				DrawDebugPoints(2);
			break;

			default:
				CurrentPattern = SPIRAL_RIGHT;
			break;
		}

		DrawText(SpiralPatternNames[CurrentPattern-3], 10, 60, 20, WHITE);
		DrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < NumOfBullets; i++)
			DrawTexture(BulletSprite, int(Bullet[i].Location.x), int(Bullet[i].Location.y), WHITE);
}


void SpiralPattern::DrawDebugInfo()
{
	DrawDebugPoints(NumOfSpiral);
	DrawText(FormatText("Spirals: %0i", NumOfSpiral), 10, 120, 18, WHITE);
}
