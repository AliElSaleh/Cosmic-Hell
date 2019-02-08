#include "LinearPattern.h"
#include "Assets.h"
#include "Enemy.h"
#include "Player.h"

const char* LinearPatternNames[]
{
	Stringify(LINEAR),
	Stringify(LINEAR LOCK ON),
	Stringify(LINEAR AIMING)
};

LinearPattern::LinearPattern()
{
	LinearPattern::Init();
}

void LinearPattern::Init()
{
	BulletSprite = GetAsset(RedBullet);
	DummySprite = GetAsset(Alien);

	// Default values
	Bullet.clear();
	
	ShootRate = 0;
	DummySpeed = 200.0f;
	bRelease = false;
	bIsInProgress = false;

	switch (CurrentPattern)
	{
		case LINEAR:
			CreateLinearPattern(5, 500.0f);
		break;

		case LINEAR_LOCK_ON:
			CreateLinearPattern(5, 500.0f);
			AddDebugInitCode();
		break;

		case LINEAR_AIMING:
			CreateLinearPattern(50, 350.0f);
			AddDebugInitCode();
		break;

		default:
		break;
	}

	for (unsigned short i = 0; i < Bullet.size(); i++)
		Bullet[i].Init();
}

void LinearPattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

	switch (CurrentPattern)
	{
		case LINEAR:
			UpdateLinearBullet(false);
		break;

		case LINEAR_LOCK_ON:
			UpdateLinearBullet(true);

			if (bDebug)
				AddDebugUpdateCode();
		break;

		case LINEAR_AIMING:
			UpdateLinearBullet(false);

			if (bDebug)
				AddDebugUpdateCode();
		break;

		default:
		break;
	}

	// Player's bomb mechanic
	if (IsKeyPressed(KEY_B))
		if (Enemy->Player->BombsLeft >= 0 && Enemy->Player->bWasBombUsed)
			DestroyActiveBullets();
}

void LinearPattern::Draw()
{
	if (bDebug)
	{
		RDrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);

		switch (CurrentPattern)
		{
			case LINEAR_LOCK_ON:
				DrawDummy();
			break;

			case LINEAR_AIMING:
				DrawDummy();
			break;

			default:
			break;
		}

		RDrawText(LinearPatternNames[CurrentPattern], 10, 60, 20, WHITE);
		RDrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < NumOfBullets; i++)
			Bullet[i].Draw();
}

void LinearPattern::Delay(const float Seconds)
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

void LinearPattern::AddDebugSwitchPatternCode()
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
			if (static_cast<Pattern>(int(CurrentPattern-1)) < 0)
				SetBulletPattern(static_cast<Pattern>(LINEAR_AIMING));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > LINEAR_AIMING)
				SetBulletPattern(static_cast<Pattern>(0));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
		}
	}
}

