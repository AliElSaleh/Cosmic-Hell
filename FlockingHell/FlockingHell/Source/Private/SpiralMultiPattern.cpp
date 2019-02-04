#include "SpiralMultiPattern.h"
#include "Assets.h"


const char* SpiralMultiPatternNames[]
{
	Stringify(SPIRAL MULTI RIGHT),
	Stringify(SPIRAL MULTI LEFT),
	Stringify(SPIRAL MULTI DOUBLE),
	Stringify(SPIRAL MIX)
};


SpiralMultiPattern::SpiralMultiPattern()
{
	SpiralMultiPattern::Init();
}

void SpiralMultiPattern::Init()
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
		case SPIRAL_MULTI_RIGHT:
			CreateSpiralMultiPattern(false, 300, 4, 320.0f, 200.0f, 1.0f);
		break;

		case SPIRAL_MULTI_LEFT:
			CreateSpiralMultiPattern(false, 300, 4, 300.0f, 200.0f, 1.0f);
		break;

		case SPIRAL_MULTI_DOUBLE:
			CreateSpiralMultiPattern(true, 400, 4, 200.0f, 100.0f, 1.0f);
		break;

		case SPIRAL_MIX:
			SpiralPattern.SetBulletPattern(SPIRAL_MULTI_FOUR_WAY);
			SpiralPattern.Init();

			CreateSpiralMultiPattern(true, 800, 4, 200.0f, 100.0f, 1.0f);
		break;

		default:
		break;
	}

	for (unsigned short i = 0; i < Bullet.size(); i++)
		Bullet[i].Init();
}

void SpiralMultiPattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

	switch (CurrentPattern)
	{
		case SPIRAL_MULTI_RIGHT:
			UpdateSpiralMultiBullet();
		break;

		case SPIRAL_MULTI_LEFT:
			UpdateSpiralMultiBullet();
		break;

		case SPIRAL_MULTI_DOUBLE:
			UpdateSpiralMultiBullet();
		break;

		case SPIRAL_MIX:
			UpdateSpiralMultiBullet();
			SpiralPattern.Update();
		break;

		default:
		break;
	}
}

void SpiralMultiPattern::Draw()
{
	if (bDebug)
	{
		RDrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);

		switch (CurrentPattern)
		{
			case SPIRAL_MULTI_RIGHT:
				DrawDebugInfo();
			break;

			case SPIRAL_MULTI_LEFT:
				DrawDebugInfo();
			break;

			case SPIRAL_MULTI_DOUBLE:
				DrawDebugInfo();
			break;

			case SPIRAL_MIX:
				DrawDebugInfo();
			break;

			default:
			break;
		}


		RDrawText(SpiralMultiPatternNames[CurrentPattern-6], 10, 60, 20, WHITE);
		RDrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	switch (CurrentPattern)
	{
		case SPIRAL_MIX:
			SpiralPattern.Draw();
		break;

		default:
		break;
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < NumOfBullets; i++)
			Bullet[i].Draw();
}

void SpiralMultiPattern::Delay(const float Seconds)
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

void SpiralMultiPattern::AddDebugSwitchPatternCode()
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
			if (static_cast<Pattern>(int(CurrentPattern-1)) < SPIRAL_MULTI_RIGHT)
				SetBulletPattern(static_cast<Pattern>(SPIRAL_MIX));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > SPIRAL_MIX)
				SetBulletPattern(static_cast<Pattern>(SPIRAL_MULTI_RIGHT));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
		}
	}
}

void SpiralMultiPattern::DrawDebugInfo()
{
	DrawDebugPoints(NumOfSpiral);
	RDrawText(FormatText("Spirals: %0i", NumOfSpiral), 10, 120, 18, WHITE);
}

