#include "SpiralOneWayPattern.h"
#include "Assets.h"


const char* SpiralOneWayPatternNames[]
{
	Stringify(SPIRAL FOUR WAY)
};

SpiralOneWayPattern::SpiralOneWayPattern()
{
	SpiralOneWayPattern::Init();
}

void SpiralOneWayPattern::Init()
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
		case SPIRAL_FOUR_WAY:
			CreateSpiralMultiPattern(unsigned short(800), 4, 250.0f, 300.0f, 20.0f);
		break;

		default:
		break;
	}
}

void SpiralOneWayPattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

	switch (CurrentPattern)
	{
		case SPIRAL_FOUR_WAY:
			UpdateSpiralMultiBullet(NumOfSpiral);
		break;

		default:
		break;
	}
}

void SpiralOneWayPattern::Draw()
{
	if (bDebug)
	{
		DrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);
		
		switch (CurrentPattern)
		{
			case SPIRAL_FOUR_WAY:
				DrawDebugPoint();
			break;

			default:
			break;
		}

		DrawText(SpiralOneWayPatternNames[CurrentPattern-20], 10, 60, 20, WHITE);
		DrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < NumOfBullets; i++)
			DrawTexture(BulletSprite, int(Bullet[i].Location.x), int(Bullet[i].Location.y), WHITE);
}

void SpiralOneWayPattern::AddDebugSwitchPatternCode()
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
			if (static_cast<Pattern>(int(CurrentPattern-1)) < SPIRAL_FOUR_WAY)
				SetBulletPattern(static_cast<Pattern>(SPIRAL_FOUR_WAY));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > SPIRAL_FOUR_WAY)
				SetBulletPattern(static_cast<Pattern>(SPIRAL_FOUR_WAY));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
		}
	}
}

void SpiralOneWayPattern::DrawDebugInfo()
{
	DrawDebugPoints(NumOfSpiral);
	DrawText(FormatText("Spirals: %0i", NumOfSpiral), 10, 120, 18, WHITE);
}
