#include "CirclePattern.h"
#include "Assets.h"

const char* CirclePatternNames[]
{
	Stringify(CIRCLE),
	Stringify(CIRCLE HOLE),
	Stringify(CIRCLE HOLE LOCK ON)
};

CirclePattern::CirclePattern()
{
	CirclePattern::Init();
}

void CirclePattern::Init()
{
	BulletSprite = GetAsset(RedBullet);
	DummySprite = GetAsset(Alien);

	// Default values
	Bullet.clear();
	Angles.clear();
	Points.clear();

	FramesCounter = 0;
	ShootRate = 0;
	bDelayed = false;
	bRelease = false;
	bIsInProgress = false;

	switch (CurrentPattern)
	{
		case CIRCLE:
			CreateCirclePattern(false, 50, 300.0f, 1.0f);
		break;

		case CIRCLE_HOLE:
			CreateCirclePattern(true, 50, 200.0f, 1.0f);
		break;

		case CIRCLE_HOLE_LOCK_ON:
			CreateCirclePattern(true, 40, 200.0f, 1.0f);
			AddDebugInitCode();
		break;

		default:
		break;
	}
}

void CirclePattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

	switch (CurrentPattern)
	{
		case CIRCLE:
			UpdateCircleBullet(false);
		break;

		case CIRCLE_HOLE:
			UpdateCircleBullet(false);
		break;

		case CIRCLE_HOLE_LOCK_ON:
			UpdateCircleBullet(true);
			AddDebugUpdateCode();
		break;

		default:
		break;
	}
}

void CirclePattern::Draw()
{
	if (bDebug)
	{
		DrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);
		
		switch (CurrentPattern)
		{
			case CIRCLE:
				DrawDebugInfo();
			break;

			case CIRCLE_HOLE:
				DrawDebugInfo();
			break;

			case CIRCLE_HOLE_LOCK_ON:
				DrawDummy();
				DrawDebugInfo();
			break;

			default:
			break;
		}
		
		DrawText(CirclePatternNames[CurrentPattern-33], 10, 60, 20, WHITE);
		DrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < NumOfBullets; i++)
			if(Bullet[i].bActive)
				DrawTexture(BulletSprite, int(Bullet[i].Location.x), int(Bullet[i].Location.y), WHITE);
}

void CirclePattern::Delay(const float Seconds)
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
	}
}

void CirclePattern::AddDebugSwitchPatternCode()
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
			if (static_cast<Pattern>(int(CurrentPattern-1)) < CIRCLE)
				SetBulletPattern(static_cast<Pattern>(CIRCLE_HOLE_LOCK_ON));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > CIRCLE_HOLE_LOCK_ON)
				SetBulletPattern(static_cast<Pattern>(CIRCLE));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
		}
	}
}

void CirclePattern::DrawDebugInfo()
{
	DrawDebugPoints(NumOfBullets);
	DrawText(FormatText("Ways: %0i", NumOfBullets), 10, 120, 18, WHITE);
}
