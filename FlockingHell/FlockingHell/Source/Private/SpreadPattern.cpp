#include "SpreadPattern.h"
#include "Assets.h"

const char* SpreadPatternNames[]
{
	Stringify(SPREAD EIGHT WAY),
	Stringify(SPREAD EIGHT WAY LOCK ON)
};

SpreadPattern::SpreadPattern()
{
	SpreadPattern::Init();
}

void SpreadPattern::Init()
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
		case SPREAD_EIGHT_WAY:
			CreateSpreadPattern(80, 8, 200.0f, 50.0f);
		break;

		case SPREAD_EIGHT_WAY_LOCK_ON:
			CreateSpreadPattern(160, 8, 200.0f, 50.0f);
			AddDebugInitCode();
		break;

		default:
		break;
	}

	for (unsigned short i = 0; i < Bullet.size(); i++)
		Bullet[i].Init();
}

void SpreadPattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

	switch (CurrentPattern)
	{
		case SPREAD_EIGHT_WAY:
			UpdateSpreadBullet(false);
		break;

		case SPREAD_EIGHT_WAY_LOCK_ON:
			UpdateSpreadBullet(true);
			AddDebugUpdateCode();
		break;

		default:
		break;
	}
}

void SpreadPattern::Draw()
{
	if (bDebug)
	{
		DrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);

		switch (CurrentPattern)
		{
			case SPREAD_EIGHT_WAY:
				DrawDebugInfo();
			break;

			case SPREAD_EIGHT_WAY_LOCK_ON:
				DrawDummy();
				DrawDebugInfo();
			break;

			default:
			break;
		}
		
		DrawText(SpreadPatternNames[CurrentPattern-31], 10, 60, 20, WHITE);
		DrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}
	
	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < NumOfBullets; i++)
			Bullet[i].Draw();
}

void SpreadPattern::Delay(const float Seconds)
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

void SpreadPattern::AddDebugSwitchPatternCode()
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
			if (static_cast<Pattern>(int(CurrentPattern-1)) < SPREAD_EIGHT_WAY)
				SetBulletPattern(static_cast<Pattern>(SPREAD_EIGHT_WAY_LOCK_ON));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > SPREAD_EIGHT_WAY_LOCK_ON)
				SetBulletPattern(static_cast<Pattern>(SPREAD_EIGHT_WAY));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
		}
	}
}

void SpreadPattern::DrawDebugInfo()
{
	DrawDebugPoints(NumOfWay);
	DrawText(FormatText("Ways: %0i", NumOfWay), 10, 120, 18, WHITE);
}

