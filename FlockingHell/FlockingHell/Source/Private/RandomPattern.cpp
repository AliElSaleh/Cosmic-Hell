#include "RandomPattern.h"
#include "Assets.h"

const char* RandomPatternNames[]
{
	Stringify(RANDOM),
	Stringify(RANDOM ALL RANGE),
	Stringify(RANDOM AIMING),
	Stringify(RANDOM SPIRAL),
	Stringify(RANDOM SPIRAL MULTI)
};

RandomPattern::RandomPattern()
{
	RandomPattern::Init();
}

void RandomPattern::Init()
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
		case RANDOM:
			CreateRandomPattern(false, 100, 0, 200.0f, 0.0f, 0.0f);
		break;

		case RANDOM_ALL_RANGE:
			CreateRandomPattern(false, 100, 0, 200.0f, 0.0f, 0.0f);
		break;

		case RANDOM_AIMING:
			CreateRandomPattern(false, 100, 0,300.0f, 0.0f, 0.0f);
			AddDebugInitCode();
		break;

		case RANDOM_SPIRAL:
			CreateRandomPattern(true, 100, 1, 300.0f, 200.0f, 15.0f);
		break;

		case RANDOM_SPIRAL_MULTI:
			CreateRandomPattern(true, 400, 4, 300.0f, 200.0f, 15.0f);
		break;

		default:
		break;
	}

	for (unsigned short i = 0; i < Bullet.size(); i++)
		Bullet[i].Init();
}

void RandomPattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

	switch (CurrentPattern)
	{
		case RANDOM:
			UpdateRandomBullet(false);
		break;

		case RANDOM_ALL_RANGE:
			UpdateRandomBullet(false);
		break;

		case RANDOM_AIMING:
			UpdateRandomBullet(false);
			AddDebugUpdateCode();
		break;

		case RANDOM_SPIRAL:
			UpdateRandomBullet(true);
		break;

		case RANDOM_SPIRAL_MULTI:
			UpdateRandomBullet(true);
		break;

		default:
		break;
	}
}

void RandomPattern::Draw()
{
	if (bDebug)
	{
		DrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);

		switch (CurrentPattern)
		{
			case RANDOM_AIMING:
				DrawDummy();
			break;

			case RANDOM_SPIRAL:
				DrawDebugInfo();
			break;

			case RANDOM_SPIRAL_MULTI:
				DrawDebugInfo();
			break;

			default:
			break;
		}
		
		DrawText(RandomPatternNames[CurrentPattern-36], 10, 60, 20, WHITE);
		DrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);

		for (int i = 0; i < NumOfBullets; i++)
		{
			//DrawCircle(Bullet[i].Location.x, Bullet[i].Location.y, 3.0f, WHITE);
			DrawCircle(Bullet[i].CollisionOffset.x, Bullet[i].CollisionOffset.y, 3.0f, WHITE);
		}
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < NumOfBullets; i++)
			if (Bullet[i].bActive)
				DrawTexture(BulletSprite, int(Bullet[i].Location.x), int(Bullet[i].Location.y), WHITE);
}

void RandomPattern::Delay(const float Seconds)
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

void RandomPattern::AddDebugSwitchPatternCode()
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
			if (static_cast<Pattern>(int(CurrentPattern-1)) < RANDOM)
				SetBulletPattern(static_cast<Pattern>(RANDOM_SPIRAL_MULTI));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > RANDOM_SPIRAL_MULTI)
				SetBulletPattern(static_cast<Pattern>(RANDOM));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
		}
	}
}

void RandomPattern::DrawDebugInfo()
{
	DrawDebugPoints(NumOfSpiral);
	DrawText(FormatText("Spirals: %0i", NumOfSpiral), 10, 120, 18, WHITE);
	DrawText(FormatText("Angle: %f", Angles[0]), 10, 140, 18, WHITE);
}
