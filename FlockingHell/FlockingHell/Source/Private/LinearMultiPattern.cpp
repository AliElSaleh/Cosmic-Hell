#include "LinearMultiPattern.h"
#include "Assets.h"
#include "Enemy.h"
#include "Player.h"

const char* LinearMultiPatternNames[]
{
	Stringify(FIVE WAY LINEAR),
	Stringify(FIVE WAY LINEAR LOCK ON),
	Stringify(SIX WAY LINEAR),
	Stringify(SIX WAY LINEAR LOCK ON),
	Stringify(SEVEN WAY),
	Stringify(EIGHT WAY LINEAR),
	Stringify(EIGHT WAY LINEAR LOCK ON),
	Stringify(ELEVEN WAY AIMING),
	Stringify(TWENTY WAY),
	Stringify(THIRTY WAY)
};

LinearMultiPattern::LinearMultiPattern()
{
	LinearMultiPattern::Init();
}

void LinearMultiPattern::Init()
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
		case FIVE_WAY_LINEAR:
			CreateLinearMultiPattern(100, 5, 300.0f, 1.0f);
		break;

		case FIVE_WAY_LINEAR_LOCK_ON:
			CreateLinearMultiPattern(100, 5, 300.0f, 1.0f);
			AddDebugInitCode();
		break;

		case SIX_WAY_LINEAR:
			CreateLinearMultiPattern(120, 6, 300.0f, 1.0f);
		break;

		case SIX_WAY_LINEAR_LOCK_ON:
			CreateLinearMultiPattern(120, 6, 300.0f, 1.0f);
			AddDebugInitCode();
		break;

		case SEVEN_WAY:
			CreateLinearMultiPattern(140, 7, 300.0f, 1.0f);
		break;

		case EIGHT_WAY_LINEAR:
			CreateLinearMultiPattern(160, 8, 300.0f, 1.0f);
		break;

		case EIGHT_WAY_LINEAR_LOCK_ON:
			CreateLinearMultiPattern(160, 8, 300.0f, 1.0f);
			AddDebugInitCode();
		break;

		case ELEVEN_WAY_AIMING:
			CreateLinearMultiPattern(484, 11, 300.0f, 1.0f);
			AddDebugInitCode();			
		break;

		case TWENTY_WAY:
			CreateLinearMultiPattern(400, 20, 300.0f, 1.0f);
		break;

		case THIRTY_WAY:
			CreateLinearMultiPattern(600, 60, 300.0f, 1.0f);
		break;

		default:
		break;
	}

	for (unsigned short i = 0; i < Bullet.size(); i++)
		Bullet[i].Init();
}

void LinearMultiPattern::Update()
{
	if (bDebug)
		AddDebugSwitchPatternCode();

	switch (CurrentPattern)
	{
		case FIVE_WAY_LINEAR:
			UpdateLinearMultiBullet(false);
		break;

		case FIVE_WAY_LINEAR_LOCK_ON:
			UpdateLinearMultiBullet(true);
			if (bDebug)
				AddDebugUpdateCode();
		break;

		case SIX_WAY_LINEAR:
			UpdateLinearMultiBullet(false);
		break;

		case SIX_WAY_LINEAR_LOCK_ON:
			UpdateLinearMultiBullet(true);
			if (bDebug)
				AddDebugUpdateCode();
		break;

		case SEVEN_WAY:
			UpdateLinearMultiBullet(false);
		break;

		case EIGHT_WAY_LINEAR:
			UpdateLinearMultiBullet(false);
		break;

		case EIGHT_WAY_LINEAR_LOCK_ON:
			UpdateLinearMultiBullet(true);
			if (bDebug)
				AddDebugUpdateCode();
		break;

		case ELEVEN_WAY_AIMING:
			UpdateLinearMultiBullet(false);

			if (bDebug)
				AddDebugUpdateCode();
		break;

		case TWENTY_WAY:
			UpdateLinearMultiBullet(false);
		break;

		case THIRTY_WAY:
			UpdateLinearMultiBullet(false);
		break;

		default:
		break;
	}

	// Player's bomb mechanic
	if (IsKeyPressed(KEY_B))
		if (Enemy->Player->BombsLeft >= 0 && Enemy->Player->bWasBombUsed)
			DestroyActiveBullets();
}

void LinearMultiPattern::Draw()
{
	if (bDebug)
	{
		RDrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);

		switch (CurrentPattern)
		{
			case FIVE_WAY_LINEAR:
				DrawDebugInfo();
			break;

			case FIVE_WAY_LINEAR_LOCK_ON:
				DrawDummy();
				DrawDebugInfo();
			break;

			case SIX_WAY_LINEAR:
				DrawDebugInfo();
			break;

			case SIX_WAY_LINEAR_LOCK_ON:
				DrawDummy();
				DrawDebugInfo();
			break;

			case SEVEN_WAY:
				DrawDebugInfo();
			break;

			case EIGHT_WAY_LINEAR:
				DrawDebugInfo();
			break;

			case EIGHT_WAY_LINEAR_LOCK_ON:
				DrawDummy();
				DrawDebugInfo();
			break;

			case ELEVEN_WAY_AIMING:
				DrawDummy();
				DrawDebugInfo();
			break;

			case TWENTY_WAY:
				DrawDebugInfo();
			break;

			case THIRTY_WAY:
				DrawDebugInfo();
			break;

			default:
			break;
		}

		RDrawText(LinearMultiPatternNames[CurrentPattern-10], 10, 60, 20, WHITE);
		RDrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
		for (int i = 0; i < Bullet.size(); i++)
			Bullet[i].Draw();
}

void LinearMultiPattern::Delay(const float Seconds)
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

void LinearMultiPattern::AddDebugSwitchPatternCode()
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
			if (static_cast<Pattern>(int(CurrentPattern-1)) < FIVE_WAY_LINEAR)
				SetBulletPattern(static_cast<Pattern>(THIRTY_WAY));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > THIRTY_WAY)
				SetBulletPattern(static_cast<Pattern>(FIVE_WAY_LINEAR));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
		}
	}
}

void LinearMultiPattern::DrawDebugInfo()
{
	DrawDebugPoints(NumOfWay);
	RDrawText(FormatText("Ways: %0i", NumOfWay), 10, 120, 18, WHITE);
}
