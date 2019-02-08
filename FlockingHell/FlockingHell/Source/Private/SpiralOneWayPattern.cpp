#include "SpiralOneWayPattern.h"
#include "Assets.h"
#include "Enemy.h"
#include "Player.h"

const char* SpiralOneWayPatternNames[]
{
	Stringify(SPIRAL FOUR WAY),
	Stringify(SPIRAL FIVE WAY),
	Stringify(SPIRAL SIX WAY),
	Stringify(SPIRAL SEVEN WAY),
	Stringify(SPIRAL EIGHT WAY)
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
	bRelease = false;
	bIsInProgress = false;

	switch (CurrentPattern)
	{
		case SPIRAL_FOUR_WAY:
			CreateSpiralMultiPattern(false, unsigned short(800), 1, 4, 250.0f, 300.0f, 1.0f);
		break;

		case SPIRAL_FIVE_WAY:
			CreateSpiralMultiPattern(false, unsigned short(800), 1, 5, 250.0f, 300.0f, 1.0f);
		break;

		case SPIRAL_SIX_WAY:
			CreateSpiralMultiPattern(false, unsigned short(800), 1, 6, 250.0f, 300.0f, 1.0f);
		break;

		case SPIRAL_SEVEN_WAY:
			CreateSpiralMultiPattern(false, unsigned short(800), 1, 7, 250.0f, 300.0f, 1.0f);
		break;

		case SPIRAL_EIGHT_WAY:
			CreateSpiralMultiPattern(false, unsigned short(800), 1, 8, 250.0f, 300.0f, 1.0f);
		break;

		default:
		break;
	}

	for (unsigned short i = 0; i < Bullet.size(); i++)
		Bullet[i].Init();
}

void SpiralOneWayPattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

	switch (CurrentPattern)
	{
		case SPIRAL_FOUR_WAY:
			UpdateSpiralMultiBullet(false);
		break;

		case SPIRAL_FIVE_WAY:
			UpdateSpiralMultiBullet(false);
		break;

		case SPIRAL_SIX_WAY:
			UpdateSpiralMultiBullet(false);
		break;

		case SPIRAL_SEVEN_WAY:
			UpdateSpiralMultiBullet(false);
		break;

		case SPIRAL_EIGHT_WAY:
			UpdateSpiralMultiBullet(false);
		break;

		default:
		break;
	}

	// Player's bomb mechanic
	if (IsKeyPressed(KEY_B))
		if (Enemy->Player->BombsLeft >= 0 && Enemy->Player->bWasBombUsed)
			DestroyActiveBullets();
}

void SpiralOneWayPattern::Draw()
{
	if (bDebug)
	{
		RDrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);
		
		switch (CurrentPattern)
		{
			case SPIRAL_FOUR_WAY:
				DrawDebugPoint();
			break;

			case SPIRAL_FIVE_WAY:
				DrawDebugPoint();
			break;

			case SPIRAL_SIX_WAY:
				DrawDebugPoint();
			break;

			case SPIRAL_SEVEN_WAY:
				DrawDebugPoint();
			break;

			case SPIRAL_EIGHT_WAY:
				DrawDebugPoint();
			break;

			default:
			break;
		}

		RDrawText(SpiralOneWayPatternNames[CurrentPattern-20], 10, 60, 20, WHITE);
		RDrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < NumOfBullets; i++)
			Bullet[i].Draw();
}

void SpiralOneWayPattern::Delay(const float Seconds)
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

void SpiralOneWayPattern::AddDebugSwitchPatternCode()
{
	if (!Bullet.empty() && bRelease)
		bIsInProgress = true;
	else
		bIsInProgress = false;

	// Switch bullet types with arrow keys when not firing/in progress
	if (!bIsInProgress)
	{
		if (IsKeyPressed(KEY_LEFT))
		{
			if (static_cast<Pattern>(int(CurrentPattern-1)) < SPIRAL_FOUR_WAY)
				SetBulletPattern(static_cast<Pattern>(SPIRAL_EIGHT_WAY));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > SPIRAL_EIGHT_WAY)
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
	RDrawText(FormatText("Spirals: %0i", NumOfSpiral), 10, 120, 18, WHITE);
}
