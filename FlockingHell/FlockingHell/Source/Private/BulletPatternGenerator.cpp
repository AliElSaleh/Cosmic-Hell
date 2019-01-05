#include "BulletPatternGenerator.h"
#include "Bullet.h"
#include <raymath.h>

#define Stringify(name) #name

const char* PatternNames[] = 
{
	Stringify(LINEAR),
	Stringify(LINEAR LOCK ON),
	Stringify(LINEAR AIMING),
	Stringify(SPIRAL RIGHT),
	Stringify(SPIRAL LEFT),
	Stringify(SPIRAL MULTI RIGHT),
	Stringify(SPIRAL MULTI LEFT),
	Stringify(SPIRAL MULTI DOUBLE),
	Stringify(FIVE WAY LINEAR),
	Stringify(FIVE WAY LINEAR LOCK ON),
	Stringify(SIX WAY LINEAR),
	Stringify(SIX WAY LINEAR LOCK ON),
	Stringify(SEVEN WAY),
	Stringify(EIGHT WAY LINEAR),
	Stringify(EIGHT WAY LINEAR LOCK ON),
	Stringify(ELEVEN WAY AIMING),
	Stringify(THIRTY WAY),
	Stringify(SPIRAL FOUR WAY),
	Stringify(SPIRAL MULTI FOUR WAY),
	Stringify(SPREAD EIGHT WAY),
	Stringify(SPREAD EIGHT WAY LOCK ON),
	Stringify(CIRCLE),
	Stringify(CIRCLE HOLE),
	Stringify(CIRCLE HOLE LOCK ON),
	Stringify(RANDOM),
	Stringify(RANDOM AIMING),
	Stringify(RANDOM SPIRAL),
	Stringify(SINE WAVE MULTI WAY AIMING)
};

void BulletPatternGenerator::Init()
{
	Index = 0;
	DummySpeed = 200.0f;
	bDebug = true;

	switch (CurrentPattern)
	{
		case LINEAR:
			NumOfBullets = 5;
			Speed = 500.0f;

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());
		break;

		case LINEAR_LOCK_ON:
			NumOfBullets = 5;
			Speed = 500.0f;

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());

			if (bDebug)
				DummyLocation = {500.0f, 600.0f};
		break;

		case LINEAR_AIMING:
			NumOfBullets = 10;
			Speed = 500.0f;

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());

			if (bDebug)
				DummyLocation = {500.0f, 600.0f};
		break;

		case SPIRAL_RIGHT:
		break;

		case SPIRAL_LEFT:
		break;

		case SPIRAL_MULTI_RIGHT:
		break;

		case SPIRAL_MULTI_LEFT:
		break;

		case SPIRAL_MULTI_DOUBLE:
		break;

		case FIVE_WAY_LINEAR:
		break;

		case FIVE_WAY_LINEAR_LOCK_ON:
		break;

		case SIX_WAY_LINEAR:
		break;

		case SIX_WAY_LINEAR_LOCK_ON:
		break;

		default:
		break;
	}
}

void BulletPatternGenerator::Update()
{
	if (NumOfBullets > 0)
		bIsInProgress = true;
	else
		bIsInProgress = false;

	if (!bIsInProgress)
	{
		if (IsKeyPressed(KEY_LEFT))
		{
			if (static_cast<Pattern>(int(CurrentPattern-1)) < 0)
				SetBulletPattern(static_cast<Pattern>(27));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > 27)
				SetBulletPattern(static_cast<Pattern>(0));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));
		}
	}

	switch (CurrentPattern)
	{
		case LINEAR:
			if (IsKeyPressed(KEY_SPACE))
				bIsSpacePressed = true;

			if(!bIsSpacePressed)
				for (int i = 0; i < NumOfBullets; i++)
				{
					// Update bullet location to spawn location
					Center = {GetMousePosition().x - Bullet[i].Radius, GetMousePosition().y - Bullet[i].Radius};
					Bullet[i].Location = Center;
				}
			else
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
			}

			if (bDebug)
				if (IsKeyPressed(KEY_ENTER))
					AddBullet();
		break;

		case LINEAR_LOCK_ON:
			if (IsKeyPressed(KEY_SPACE))
				bIsSpacePressed = true;

			if(!bIsSpacePressed)
			{
				for (int i = 0; i < NumOfBullets; i++)
				{
					// Update bullet location to spawn location
					Center = {GetMousePosition().x - Bullet[i].Radius, GetMousePosition().y - Bullet[i].Radius};
					Bullet[i].Location = Center;
					
					// Calculate direction to target
					CalculateDirection(i);
				}
			}
			else
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
			}

			if (bDebug)
			{
				// Add bullets to vector
				if (IsKeyPressed(KEY_ENTER))
					AddBullet();

				// Move dummy
				if (IsKeyDown(KEY_A))
					DummyLocation.x -= DummySpeed * GetFrameTime();

				if (IsKeyDown(KEY_D))
					DummyLocation.x += DummySpeed * GetFrameTime();

				if (IsKeyDown(KEY_S))
					DummyLocation.y += DummySpeed * GetFrameTime();

				if (IsKeyDown(KEY_W))
					DummyLocation.y -= DummySpeed * GetFrameTime();
			}
		break;

		case LINEAR_AIMING:
			if (IsKeyPressed(KEY_SPACE))
				bIsSpacePressed = true;

			if(!bIsSpacePressed)
				for (int i = 0; i < NumOfBullets; i++)
				{
					Center = {GetMousePosition().x - Bullet[i].Radius, GetMousePosition().y - Bullet[i].Radius};
					Bullet[i].Location = Center;
				}
			else
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
			}

			// Calculate direction to target
			for (int i = 0; i < NumOfBullets; i++)
				CalculateDirection(i);
		
			if (bDebug)
			{
				// Add bullets to vector
				if (IsKeyPressed(KEY_ENTER))
					AddBullet();

				// Move dummy
				if (IsKeyDown(KEY_A))
					DummyLocation.x -= DummySpeed * GetFrameTime();

				if (IsKeyDown(KEY_D))
					DummyLocation.x += DummySpeed * GetFrameTime();

				if (IsKeyDown(KEY_S))
					DummyLocation.y += DummySpeed * GetFrameTime();

				if (IsKeyDown(KEY_W))
					DummyLocation.y -= DummySpeed * GetFrameTime();
			}
		break;

		case SPIRAL_RIGHT:
		break;

		case SPIRAL_LEFT:
		break;

		case SPIRAL_MULTI_RIGHT:
		break;

		case SPIRAL_MULTI_LEFT:
		break;

		case SPIRAL_MULTI_DOUBLE:
		break;

		case FIVE_WAY_LINEAR:
		break;

		case FIVE_WAY_LINEAR_LOCK_ON:
		break;

		case SIX_WAY_LINEAR:
		break;

		case SIX_WAY_LINEAR_LOCK_ON:
		break;

		default:
		break;
	}
}

void BulletPatternGenerator::CalculateDirection(const int i)
{
	Bullet[i].Direction = Vector2Subtract(Center, DummyLocation);
	Bullet[i].Direction = Vector2Normalize(Bullet[i].Direction);
	Bullet[i].Direction = Vector2Negate(Bullet[i].Direction);
}

void BulletPatternGenerator::Draw()
{
	if (bDebug)
	{
		DrawText("Switch patterns with <- or -> arrow keys", 10, 30, 16, WHITE);

		switch (CurrentPattern)
		{
			case LINEAR_LOCK_ON:
				DrawTexture(DummySprite, DummyLocation.x, DummyLocation.y, WHITE);
			break;

			case LINEAR_AIMING:
				DrawTexture(DummySprite, DummyLocation.x, DummyLocation.y, WHITE);
			break;

			default:
			break;
		}

		DrawText(PatternNames[CurrentPattern], 10, 60, 20, WHITE);
		DrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	for (int i = 0; i < NumOfBullets; i++)
		DrawTexture(BulletSprite, int(Bullet[i].Location.x), int(Bullet[i].Location.y), WHITE);
}

void BulletPatternGenerator::Delay()
{
}

void BulletPatternGenerator::Resume()
{
}

void BulletPatternGenerator::Pause()
{
}

void BulletPatternGenerator::StartShotRoutine()
{
	switch (CurrentPattern)
	{
		case LINEAR:
			UpdateBullet();
		break;

		case LINEAR_LOCK_ON:
			UpdateBulletLockOn();
		break;

		case LINEAR_AIMING:
			UpdateBulletLockOn();
		break;

		case SPIRAL_RIGHT:
		break;

		case SPIRAL_LEFT:
		break;

		case SPIRAL_MULTI_RIGHT:
		break;

		case SPIRAL_MULTI_LEFT:
		break;

		case SPIRAL_MULTI_DOUBLE:
		break;

		case FIVE_WAY_LINEAR:
		break;

		case FIVE_WAY_LINEAR_LOCK_ON:
		break;

		case SIX_WAY_LINEAR:
		break;

		case SIX_WAY_LINEAR_LOCK_ON:
		break;

		default:
		break;
	}
}

void BulletPatternGenerator::UpdateBulletLockOn()
{
	ShootRate += 2;

	for (int i = 0; i < NumOfBullets; i++)
	{
		if (!Bullet[i].bActive && ShootRate % 20 == 0)
		{
			Bullet[i].Location = Center;
			Bullet[i].Damage = GetRandomValue(20, 40);
			Bullet[i].bActive = true;
			break;
		}
	}

	for (int i = 0; i < NumOfBullets; i++)
	{
		if (Bullet[i].bActive)
		{
			// Bullet movement
			Bullet[i].Location.x += Bullet[i].Direction.x * Speed * GetFrameTime();
			Bullet[i].Location.y += Bullet[i].Direction.y * Speed * GetFrameTime();
		}
	}
}

void BulletPatternGenerator::UpdateBullet()
{
	ShootRate += 2;

	for (int i = 0; i < NumOfBullets; i++)
	{
		if (!Bullet[i].bActive && ShootRate % 20 == 0)
		{
			Bullet[i].Location = Center;
			Bullet[i].Damage = GetRandomValue(20, 40);
			Bullet[i].bActive = true;
			break;
		}
	}

	for (int i = 0; i < NumOfBullets; i++)
	{
		if (Bullet[i].bActive)
		{
			// Bullet movement
			Bullet[i].Location.y += Speed * GetFrameTime();
		}
		else
			Bullet[i].Location = Center;
	}
}

void BulletPatternGenerator::SetBulletPattern(const Pattern NewPattern)
{
	CurrentPattern = NewPattern;
}

void BulletPatternGenerator::AddBullet()
{
	// Create bullet and place in vector
	Bullet.emplace_back(::Bullet());
	NumOfBullets++;

	for (int i = 0; i < NumOfBullets; i++)
	{
		if (!Bullet[i].bActive)
		{
			// Set inital bullet location to be the center location
			Center = { GetMousePosition().x - Bullet[i].Radius, GetMousePosition().y - Bullet[i].Radius };
			Bullet[i].Location = Center;

			// Calculate direction to target
			Bullet[i].Direction = Vector2Subtract(Center, DummyLocation);
			Bullet[i].Direction = Vector2Normalize(Bullet[i].Direction);
			Bullet[i].Direction = Vector2Negate(Bullet[i].Direction);
		}
	}
}

void BulletPatternGenerator::CheckBulletOutsideWindow()
{
	if (NumOfBullets > 0)
		for (int i = 0; i < NumOfBullets; i++)
		{
			if (Bullet[i].Location.x - Bullet[i].Radius > GetScreenWidth() || 
				Bullet[i].Location.x + Bullet[i].Radius < 0 ||
				Bullet[i].Location.y - Bullet[i].Radius > GetScreenHeight() ||
				Bullet[i].Location.y + Bullet[i].Radius < 0)
			{
				Bullet.erase(Bullet.begin());
				NumOfBullets--;
			}
		}
}
