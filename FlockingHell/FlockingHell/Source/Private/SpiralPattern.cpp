#include "SpiralPattern.h"
#include "Assets.h"
#include "Enemy.h"
#include "Player.h"

const char* SpiralPatternNames[]
{
	Stringify(SPIRAL RIGHT),
	Stringify(SPIRAL LEFT),
	Stringify(SPIRAL DOUBLE)
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
	bRelease = false;
	bIsInProgress = false;

	switch (CurrentPattern)
	{
		case SPIRAL_RIGHT:
			CreateSpiralPattern(false, 200, 100.0f, 600.0f, 1.0f);
		break;

		case SPIRAL_LEFT:
			CreateSpiralPattern(false, 100, 250.0f, 300.0f, 1.0f);
		break;

		case SPIRAL_DOUBLE:
			CreateSpiralPattern(true, 400, 150.0f, 300.0f, 20.0f);
		break;

		default:
		break;
	}

	for (unsigned short i = 0; i < Bullet.size(); i++)
		Bullet[i].Init();
}

void SpiralPattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

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

	// Player's bomb mechanic
	if (IsKeyPressed(KEY_B))
		if (Enemy->Player->BombsLeft >= 0)
			DestroyActiveBullets();
}

void SpiralPattern::Draw()
{
	if (bDebug)
	{
		RDrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);
		
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
			break;
		}

		RDrawText(SpiralPatternNames[CurrentPattern-3], 10, 60, 20, WHITE);
		RDrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < NumOfBullets; i++)
			Bullet[i].Draw();
}

void SpiralPattern::Delay(const float Seconds)
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


void SpiralPattern::AddDebugSwitchPatternCode()
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
}

void SpiralPattern::DrawDebugInfo()
{
	DrawDebugPoints(NumOfSpiral);
	RDrawText(FormatText("Spirals: %0i", NumOfSpiral), 10, 120, 18, WHITE);
}
