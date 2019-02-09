#include "SpiralMultiWayPattern.h"
#include "Assets.h"
#include "Enemy.h"
#include "Player.h"

const char* SpiralMultiWayPatternNames[]
{
	Stringify(SPIRAL MULTI THREE WAY),
	Stringify(SPIRAL MULTI FOUR WAY),
	Stringify(SPIRAL MULTI FIVE WAY),
	Stringify(SPIRAL MULTI SIX WAY),
	Stringify(SPIRAL MULTI SEVEN WAY),
	Stringify(SPIRAL MULTI EIGHT WAY)
};

SpiralMultiWayPattern::SpiralMultiWayPattern()
{
	SpiralMultiWayPattern::Init();
}

void SpiralMultiWayPattern::Init()
{
	BulletSprite = GetAsset(RedBullet);
	DummySprite = GetAsset(Alien);

	// Default values
	Bullet.clear();
	Angles.clear();
	Points.clear();

	ShootRate = 0;
	bRelease = false;
	bIsInProgress = false;

	switch (CurrentPattern)
	{
		case SPIRAL_MULTI_THREE_WAY:
			CreateSpiralMultiPattern(true, unsigned short(600), 3, 4, 350.0f, 300.0f, 1.0f);
		break;

		case SPIRAL_MULTI_FOUR_WAY:
			CreateSpiralMultiPattern(true, unsigned short(800), 2, 4, 350.0f, 300.0f, 1.0f);
		break;

		case SPIRAL_MULTI_FIVE_WAY:
			CreateSpiralMultiPattern(true, unsigned short(500), 5, 4, 350.0f, 300.0f, 1.0f);
		break;

		case SPIRAL_MULTI_SIX_WAY:
			CreateSpiralMultiPattern(true, unsigned short(600), 6, 4, 350.0f, 300.0f, 1.0f);
		break;

		case SPIRAL_MULTI_SEVEN_WAY:
			CreateSpiralMultiPattern(true, unsigned short(700), 7, 4, 350.0f, 300.0f, 1.0f);
		break;

		case SPIRAL_MULTI_EIGHT_WAY:
			CreateSpiralMultiPattern(true, unsigned short(800), 8, 4, 350.0f, 300.0f, 1.0f);
		break;

		default:
		break;
	}

	for (unsigned short i = 0; i < Bullet.size(); i++)
		Bullet[i].Init();
}

void SpiralMultiWayPattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

	switch (CurrentPattern)
	{
		case SPIRAL_MULTI_THREE_WAY:
			UpdateSpiralMultiBullet(true);
		break;

		case SPIRAL_MULTI_FOUR_WAY:
			UpdateSpiralMultiBullet(true);
		break;

		case SPIRAL_MULTI_FIVE_WAY:
			UpdateSpiralMultiBullet(true);
		break;

		case SPIRAL_MULTI_SIX_WAY:
			UpdateSpiralMultiBullet(true);
		break;

		case SPIRAL_MULTI_SEVEN_WAY:
			UpdateSpiralMultiBullet(true);
		break;

		case SPIRAL_MULTI_EIGHT_WAY:
			UpdateSpiralMultiBullet(true);
		break;

		default:
		break;
	}

	// Player's bomb mechanic
	if (IsKeyPressed(KEY_B))
		if (Enemy->Player->BombsLeft >= 0 && Enemy->Player->bWasBombUsed)
			DestroyActiveBullets();
}

void SpiralMultiWayPattern::Draw()
{
	if (bDebug)
	{
		RDrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);
		
		switch (CurrentPattern)
		{
			case SPIRAL_MULTI_THREE_WAY:
				DrawDebugPoint();
			break;

			case SPIRAL_MULTI_FOUR_WAY:
				DrawDebugPoint();
			break;

			case SPIRAL_MULTI_FIVE_WAY:
				DrawDebugPoint();
			break;

			case SPIRAL_MULTI_SIX_WAY:
				DrawDebugPoint();
			break;

			case SPIRAL_MULTI_SEVEN_WAY:
				DrawDebugPoint();
			break;

			case SPIRAL_MULTI_EIGHT_WAY:
				DrawDebugPoint();
			break;

			default:
			break;
		}

		RDrawText(SpiralMultiWayPatternNames[CurrentPattern-25], 10, 60, 20, WHITE);
		RDrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < Bullet.size(); i++)
			Bullet[i].Draw();
}

void SpiralMultiWayPattern::Delay(const float Seconds)
{
	FramesCounter++;

	if (!bDelayed)
	{
		if (Seconds == 0)
			bDelayed = true;
		else
		{
			if (((FramesCounter/(120*Seconds))/2) == 1)
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
				FramesCounter = 0;
				bDelayed = true;
			}	
		}
	}
	else
	{
		StartShotRoutine();
		CheckBulletOutsideWindow();

		for (unsigned short i = 0; i < Bullet.size(); i++)
			Bullet[i].Update();
	}
}

void SpiralMultiWayPattern::AddDebugSwitchPatternCode()
{
	if (!Bullet.empty() && bRelease)
		bIsInProgress = true;
	else
	{
		bIsInProgress = false;
		bDelayed= false;
	}

	// Switch bullet types with arrow keys when not firing/in progress
	if (!bIsInProgress)
	{
		if (IsKeyPressed(KEY_LEFT))
		{
			if (static_cast<Pattern>(int(CurrentPattern-1)) < SPIRAL_MULTI_THREE_WAY)
				SetBulletPattern(static_cast<Pattern>(SPIRAL_MULTI_EIGHT_WAY));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > SPIRAL_MULTI_EIGHT_WAY)
				SetBulletPattern(static_cast<Pattern>(SPIRAL_MULTI_THREE_WAY));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
		}
	}
}

void SpiralMultiWayPattern::DrawDebugInfo()
{
	DrawDebugPoints(NumOfSpiral);
	RDrawText(FormatText("Spirals: %0i", NumOfSpiral), 10, 120, 18, WHITE);
}

