#include "LinearPattern.h"
#include "Assets.h"

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
			CreateLinearPattern(10, 500.0f);
			AddDebugInitCode();
		break;

		default:
		break;
	}
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
			AddDebugUpdateCode();
		break;

		case LINEAR_AIMING:
			UpdateLinearBullet(false);
			AddDebugUpdateCode();
		break;

		default:
		break;
	}
}

void LinearPattern::Draw()
{
	if (bDebug)
	{
		DrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);

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

		DrawText(LinearPatternNames[CurrentPattern], 10, 60, 20, WHITE);
		DrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < NumOfBullets; i++)
			DrawTexture(BulletSprite, int(Bullet[i].Location.x), int(Bullet[i].Location.y), WHITE);
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

