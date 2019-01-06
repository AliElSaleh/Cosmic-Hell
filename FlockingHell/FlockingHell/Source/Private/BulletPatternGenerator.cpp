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
	Stringify(SPIRAL DOUBLE),
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
	Bullet.clear();
	Angles.clear();
	Points.clear();
	ShootRate = 0;
	DummySpeed = 200.0f;
	bIsSpacePressed = false;
	bIsInProgress = false;
	bDebug = true;

	switch (CurrentPattern)
	{
		case LINEAR:
			NumOfBullets = 5;
			BulletSpeed = 500.0f;
			Bullet.reserve(NumOfBullets);

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());
		break;

		case LINEAR_LOCK_ON:
			NumOfBullets = 5;
			BulletSpeed = 500.0f;
			Bullet.reserve(NumOfBullets);

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());

			if (bDebug)
				DummyLocation = {500.0f, 600.0f};
		break;

		case LINEAR_AIMING:
			NumOfBullets = 10;
			BulletSpeed = 500.0f;
			Bullet.reserve(NumOfBullets);

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());

			if (bDebug)
				DummyLocation = {500.0f, 600.0f};
		break;

		case SPIRAL_RIGHT:
			NumOfBullets = 300;
			BulletSpeed = 100.0f;
			RotationSpeed = 1000.0f;
			CircleRadius = 1.0f;
			Angle = 0.0f;
			Bullet.reserve(NumOfBullets);

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());

			BulletRadius = Bullet[0].Radius;

			if (bDebug)
				DummyLocation = {500.0f, 600.0f};
		break;

		case SPIRAL_LEFT:
			NumOfBullets = 100;
			BulletSpeed = 150.0f;
			RotationSpeed = 200.0f;
			CircleRadius = 20.0f;
			Angle = 0.0f;
			Bullet.reserve(NumOfBullets);

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());

			BulletRadius = Bullet[0].Radius;

			if (bDebug)
				DummyLocation = {500.0f, 600.0f};
		break;
		
		case SPIRAL_DOUBLE:
			NumOfBullets = 400;
			BulletSpeed = 200.0f;
			RotationSpeed = 400.0f;
			CircleRadius = 1.0f;
			Angle = 0.0f;	
			Bullet.reserve(NumOfBullets);
			Points.reserve(2);
			Angles.reserve(2);

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());

			for (int i = 0; i < 2; i++)
			{
				Angles.emplace_back(Angle += 270.0f);
				Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));	
			}

			BulletRadius = Bullet[0].Radius;

			if (bDebug)
				DummyLocation = {500.0f, 600.0f};
		break;

		case SPIRAL_MULTI_RIGHT:
		{
			NumOfBullets = 400;
			NumOfWay = 4;
			BulletSpeed = 200.0f;
			RotationSpeed = 200.0f;
			CircleRadius = 1.0f;
			Angle = 0.0f;
			Bullet.reserve(NumOfBullets);
			Points.reserve(NumOfWay);
			Angles.reserve(NumOfWay);

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());

			const float Offset = (360.f/NumOfWay);

			for (int i = 0; i < NumOfWay; i++)
			{
				Angles.emplace_back(Angle+=Offset);
				Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));
			}

			BulletRadius = Bullet[0].Radius;

			if (bDebug)
				DummyLocation = {500.0f, 600.0f};
		}
		break;

		case SPIRAL_MULTI_LEFT:
		{
			NumOfBullets = 400;
			NumOfWay = 4;
			BulletSpeed = 200.0f;
			RotationSpeed = 200.0f;
			CircleRadius = 1.0f;
			Angle = 0.0f;
			Bullet.reserve(NumOfBullets);
			Points.reserve(NumOfWay);
			Angles.reserve(NumOfWay);

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());

			const float Offset = (360.f/NumOfWay);

			for (int i = 0; i < NumOfWay; i++)
			{
				Angles.emplace_back(Angle+=Offset);
				Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));
			}

			BulletRadius = Bullet[0].Radius;

			if (bDebug)
				DummyLocation = {500.0f, 600.0f};
		}
		break;

		case SPIRAL_MULTI_DOUBLE:
		{
			NumOfBullets = 400;
			NumOfWay = 4*2;
			BulletSpeed = 200.0f;
			RotationSpeed = 100.0f;
			CircleRadius = 1.0f;
			Angle = 0.0f;
			Bullet.reserve(NumOfBullets);
			Points.reserve(NumOfWay);
			Angles.reserve(NumOfWay);

			for (int i = 0; i < NumOfBullets; i++)
				Bullet.emplace_back(::Bullet());

			const float Offset = (360.f/(float(NumOfWay)/2));

			for (int i = 0; i < NumOfWay; i++)
			{
				Angles.emplace_back(Angle+=Offset);
				Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));
			}

			BulletRadius = Bullet[0].Radius;

			if (bDebug)
				DummyLocation = {500.0f, 600.0f};
		}
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
	if (!Bullet.empty() && bIsSpacePressed)
		bIsInProgress = true;
	else
		bIsInProgress = false;

	if (!bIsInProgress)
	{
		if (IsKeyPressed(KEY_LEFT))
		{
			if (static_cast<Pattern>(int(CurrentPattern-1)) < 0)
				SetBulletPattern(static_cast<Pattern>(SINE_WAVE_MULTI_WAY_AIMING));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > SINE_WAVE_MULTI_WAY_AIMING)
				SetBulletPattern(static_cast<Pattern>(0));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern+1)));

			Init();
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
					CalculateDirection(i, DummyLocation);
				}
			}
			else
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
			}

			AddDebugCode();
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
				CalculateDirection(i, DummyLocation);
		
			AddDebugCode();
		break;

		case SPIRAL_RIGHT:
			if (IsKeyPressed(KEY_SPACE))
				bIsSpacePressed = true;

			if(!bIsSpacePressed)
				for (int i = 0; i < NumOfBullets; i++)
				{
					Center = {GetMousePosition().x - Bullet[i].Radius, GetMousePosition().y - Bullet[i].Radius};
					PointOnCircle = {Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD)};

					Bullet[i].Location = {PointOnCircle.x - BulletRadius, PointOnCircle.y - BulletRadius};
				}
			else
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
			}

			AddDebugCode();
		break;

		case SPIRAL_LEFT:
			if (IsKeyPressed(KEY_SPACE))
				bIsSpacePressed = true;

			if(!bIsSpacePressed)
				for (int i = 0; i < NumOfBullets; i++)
				{
					Center = {GetMousePosition().x - Bullet[i].Radius, GetMousePosition().y - Bullet[i].Radius};
					PointOnCircle = {Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD)};

					Bullet[i].Location = {PointOnCircle.x - BulletRadius, PointOnCircle.y - BulletRadius};
				}
			else
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
			}

			AddDebugCode();
		break;

		case SPIRAL_DOUBLE:
			if (IsKeyPressed(KEY_SPACE))
				bIsSpacePressed = true;

			if(!bIsSpacePressed)
			{
				for (int i = 0; i < 2; i++)
				{
					Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
					Bullet[i].Location = {Points[i].x - BulletRadius, Points[i].y - BulletRadius};
				}

				for (int i = 0; i < NumOfBullets; i++)
				{
					Center = {GetMousePosition().x - Bullet[i].Radius, GetMousePosition().y - Bullet[i].Radius};
					//PointOnCircle = {Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD)};
					//PointOnCircle2 = {Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD)};

				}				
			}
			else
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
			}

			AddDebugCode();
		break;

		case SPIRAL_MULTI_RIGHT:
			if (IsKeyPressed(KEY_SPACE))
				bIsSpacePressed = true;

			if(!bIsSpacePressed)
			{
				int k = 0;
				for (int i = 0; i < NumOfWay; i++)
				{
					Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};

					for (int j = k; j < NumOfBullets/NumOfWay+k; j++)
					{
						Center = {GetMousePosition().x - Bullet[j].Radius, GetMousePosition().y - Bullet[j].Radius}; // Spawn point

						Bullet[j].Location = {Points[i].x - BulletRadius, Points[i].y - BulletRadius};
					}

					k += NumOfBullets/NumOfWay;
				}
			}
			else
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
			}

			AddDebugCode();
		break;

		case SPIRAL_MULTI_LEFT:
			if (IsKeyPressed(KEY_SPACE))
				bIsSpacePressed = true;

			if(!bIsSpacePressed)
			{
				int k = 0;
				for (int i = 0; i < NumOfWay; i++)
				{
					Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};

					for (int j = k; j < NumOfBullets/NumOfWay+k; j++)
					{
						Center = {GetMousePosition().x - Bullet[j].Radius, GetMousePosition().y - Bullet[j].Radius}; // Spawn point

						Bullet[j].Location = {Points[i].x - BulletRadius, Points[i].y - BulletRadius};
					}

					k += NumOfBullets/NumOfWay;
				}
			}
			else
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
			}

			AddDebugCode();
		break;

		case SPIRAL_MULTI_DOUBLE:
			if (IsKeyPressed(KEY_SPACE))
				bIsSpacePressed = true;

			if(!bIsSpacePressed)
			{
				int k = 0;
				for (int i = 0; i < NumOfWay; i++)
				{
					Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};

					for (int j = k; j < NumOfBullets/NumOfWay+k; j++)
					{
						Center = {GetMousePosition().x - Bullet[j].Radius, GetMousePosition().y - Bullet[j].Radius}; // Spawn point

						Bullet[j].Location = {Points[i].x - BulletRadius, Points[i].y - BulletRadius};
					}

					k += NumOfBullets/NumOfWay;
				}
			}
			else
			{
				StartShotRoutine();
				CheckBulletOutsideWindow();
			}

			AddDebugCode();
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

void BulletPatternGenerator::AddDebugCode()
{
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
}

void BulletPatternGenerator::Draw()
{
	if (bDebug)
	{
		DrawText("Switch patterns using the LEFT or RIGHT arrow keys", 10, 30, 16, WHITE);

		switch (CurrentPattern)
		{
			case LINEAR_LOCK_ON:
				DrawTexture(DummySprite, int(DummyLocation.x), int(DummyLocation.y), WHITE);
			break;

			case LINEAR_AIMING:
				DrawTexture(DummySprite, int(DummyLocation.x), int(DummyLocation.y), WHITE);
			break;

			case SPIRAL_RIGHT:
				DrawCircleLines(int(Center.x + BulletRadius), int(Center.y + BulletRadius), CircleRadius, WHITE);
				DrawCircle(int(PointOnCircle.x), int(PointOnCircle.y), 2.0f, BLUE);
				
				DrawTexture(DummySprite, int(DummyLocation.x), int(DummyLocation.y), WHITE);
			break;

			case SPIRAL_LEFT:
				DrawCircleLines(int(Center.x + BulletRadius), int(Center.y + BulletRadius), CircleRadius, WHITE);
				DrawCircle(int(PointOnCircle.x), int(PointOnCircle.y), 2.0f, BLUE);
				
				DrawTexture(DummySprite, int(DummyLocation.x), int(DummyLocation.y), WHITE);
			break;
				
			case SPIRAL_DOUBLE:
				DrawCircleLines(int(Center.x + BulletRadius), int(Center.y + BulletRadius), CircleRadius, WHITE);

				for (int i = 0; i < 2; i++)
					DrawCircle(int(Points[i].x), int(Points[i].y), 2.0f, BLUE);

				//DrawCircle(int(PointOnCircle.x), int(PointOnCircle.y), 2.0f, BLUE);
				//DrawCircle(int(PointOnCircle2.x), int(PointOnCircle2.y), 2.0f, BLUE);

				DrawTexture(DummySprite, int(DummyLocation.x), int(DummyLocation.y), WHITE);
			break;

			case SPIRAL_MULTI_RIGHT:
				DrawCircleLines(int(Center.x + BulletRadius), int(Center.y + BulletRadius), CircleRadius, WHITE);

				for (int i = 0; i < NumOfWay; i++)
					DrawCircle(int(Points[i].x), int(Points[i].y), 2.0f, BLUE);

				DrawTexture(DummySprite, int(DummyLocation.x), int(DummyLocation.y), WHITE);

				DrawText(FormatText("Ways: %0i", NumOfWay), 10, 120, 18, WHITE);
			break;

			case SPIRAL_MULTI_LEFT:
				DrawCircleLines(int(Center.x + BulletRadius), int(Center.y + BulletRadius), CircleRadius, WHITE);

				for (int i = 0; i < NumOfWay; i++)
					DrawCircle(int(Points[i].x), int(Points[i].y), 2.0f, BLUE);

				DrawTexture(DummySprite, int(DummyLocation.x), int(DummyLocation.y), WHITE);
				
				DrawText(FormatText("Ways: %0i", NumOfWay), 10, 120, 18, WHITE);
			break;

			case SPIRAL_MULTI_DOUBLE:
				DrawCircleLines(int(Center.x + BulletRadius), int(Center.y + BulletRadius), CircleRadius, WHITE);

				for (int i = 0; i < NumOfWay; i++)
					DrawCircle(int(Points[i].x), int(Points[i].y), 2.0f, BLUE);

				DrawTexture(DummySprite, int(DummyLocation.x), int(DummyLocation.y), WHITE);
				
				DrawText(FormatText("Ways: %0i", NumOfWay), 10, 120, 18, WHITE);

			default:
			break;
		}

		DrawText(PatternNames[CurrentPattern], 10, 60, 20, WHITE);
		DrawText(FormatText("Bullets: %0i", Bullet.size()), 10, 90, 18, WHITE);
	}

	// Bullets
	if (!Bullet.empty())
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

void BulletPatternGenerator::CalculateDirection(const int i, Vector2 Target)
{
	Bullet[i].Direction = Vector2Subtract(Center, Target);
	Bullet[i].Direction = Vector2Normalize(Bullet[i].Direction);
	Bullet[i].Direction = Vector2Negate(Bullet[i].Direction);
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
			UpdateBulletSpiral(false, true);
		break;

		case SPIRAL_LEFT:
			UpdateBulletSpiral(true, false);
		break;

		case SPIRAL_DOUBLE:
			UpdateBulletSpiral(true, true);
		break;

		case SPIRAL_MULTI_RIGHT:
			UpdateBulletSpiralMulti(false, true);
		break;

		case SPIRAL_MULTI_LEFT:
			UpdateBulletSpiralMulti(true, false);
		break;

		case SPIRAL_MULTI_DOUBLE:
			UpdateBulletSpiralMulti(true, true);
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
			Bullet[i].Location.y += BulletSpeed * GetFrameTime();
		}
		else
			Bullet[i].Location = Center;
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
			Bullet[i].Location.x += Bullet[i].Direction.x * BulletSpeed * GetFrameTime();
			Bullet[i].Location.y += Bullet[i].Direction.y * BulletSpeed * GetFrameTime();
		}
	}
}

void BulletPatternGenerator::UpdateBulletSpiral(const bool Left, const bool Right)
{
	if (Left && !Right)
	{ 
		ShootRate += 4;

		// Rotate the spawn point on circle
		PointOnCircle = {Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD)};
		Angle -= RotationSpeed * GetFrameTime();

		for (int i = 0; i < NumOfBullets; i++)
		{
			if (!Bullet[i].bActive && ShootRate % 28 == 0)
			{
				Bullet[i].Location = PointOnCircle;
				Bullet[i].Damage = GetRandomValue(20, 40);
				Bullet[i].bActive = true;
				CalculateDirection(i, PointOnCircle);

				break;
			}
		}

		for (int i = 0; i < NumOfBullets; i++)
		{
			if (Bullet[i].bActive)
			{
				// Bullet movement
				Bullet[i].Location.x += Bullet[i].Direction.x * BulletSpeed * GetFrameTime();
				Bullet[i].Location.y += Bullet[i].Direction.y * BulletSpeed * GetFrameTime();
			}
		}
	}
	else if (Right && !Left)
	{
		ShootRate += 4;

		// Rotate the spawn point on circle
		PointOnCircle = {Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD)};
		Angle += RotationSpeed * GetFrameTime();

		for (int i = 0; i < NumOfBullets; i++)
		{
			if (!Bullet[i].bActive && ShootRate % 16 == 0)
			{
				Bullet[i].Location = PointOnCircle;
				Bullet[i].Damage = GetRandomValue(20, 40);
				Bullet[i].bActive = true;
				CalculateDirection(i, PointOnCircle);

				break;
			}
		}

		for (int i = 0; i < NumOfBullets; i++)
		{
			if (Bullet[i].bActive)
			{
				// Bullet movement
				Bullet[i].Location.x += Bullet[i].Direction.x * BulletSpeed * GetFrameTime();
				Bullet[i].Location.y += Bullet[i].Direction.y * BulletSpeed * GetFrameTime();
			}
		}
	}
	else if (Left && Right)
	{
		ShootRate += 4;

		// Rotate the spawn point on circle
		Points[0] = {Center.x + CircleRadius * cosf(Angles[0]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[0]*DEG2RAD)};
		Angles[0] += RotationSpeed * GetFrameTime();

		for (int i = 0; i < NumOfBullets/2; i++)
		{
			if (!Bullet[i].bActive && ShootRate % 28 == 0)
			{
				Bullet[i].Location = Points[0];
				Bullet[i].Damage = GetRandomValue(20, 40);
				Bullet[i].bActive = true;
				CalculateDirection(i, Points[0]);

				break;
			}
		}

		// Rotate the spawn point on circle
		Points[1] = {Center.x + CircleRadius * cosf(Angles[1]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[1]*DEG2RAD)};
		Angles[1] -= RotationSpeed * GetFrameTime();

		for (int i = NumOfBullets/2; i < NumOfBullets; i++)
		{
			if (!Bullet[i].bActive && ShootRate % 28 == 0)
			{
				Bullet[i].Location = Points[1];
				Bullet[i].Damage = GetRandomValue(20, 40);
				Bullet[i].bActive = true;
				CalculateDirection(i, Points[1]);

				break;
			}
		}

		for (int i = 0; i < NumOfBullets; i++)
		{
			if (Bullet[i].bActive)
			{
				// Bullet movement
				Bullet[i].Location.x += Bullet[i].Direction.x * BulletSpeed * GetFrameTime();
				Bullet[i].Location.y += Bullet[i].Direction.y * BulletSpeed * GetFrameTime();
			}
		}
	}
}

void BulletPatternGenerator::UpdateBulletSpiralMulti(bool Left, bool Right)
{
	if (Left && !Right)
	{
		ShootRate += 4;

		for (int i = 0; i < NumOfWay; i++)
		{
			// Rotate the spawn point on circle
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
			Angles[i] -= RotationSpeed * GetFrameTime();

			for (int j = 0; j < NumOfBullets; j++)
			{
				if (!Bullet[j].bActive && ShootRate % 28 == 0)
				{
					Bullet[j].Location = Points[i];
					Bullet[j].Damage = GetRandomValue(20, 40);
					Bullet[j].bActive = true;
					CalculateDirection(j, Points[i]);

					break;
				}
			}
		}

		for (int i = 0; i < NumOfBullets; i++)
		{
			if (Bullet[i].bActive)
			{
				// Bullet movement
				Bullet[i].Location.x += Bullet[i].Direction.x * BulletSpeed * GetFrameTime();
				Bullet[i].Location.y += Bullet[i].Direction.y * BulletSpeed * GetFrameTime();
			}
		}
	}
	else if (Right && !Left)
	{
		ShootRate += 4;

		for (int i = 0; i < NumOfWay; i++)
		{
			// Rotate the spawn point on circle
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
			Angles[i] += RotationSpeed * GetFrameTime();

			for (int j = 0; j < NumOfBullets; j++)
			{
				if (!Bullet[j].bActive && ShootRate % 28 == 0)
				{
					Bullet[j].Location = Points[i];
					Bullet[j].Damage = GetRandomValue(20, 40);
					Bullet[j].bActive = true;
					CalculateDirection(j, Points[i]);

					break;
				}
			}
		}

		for (int i = 0; i < NumOfBullets; i++)
		{
			if (Bullet[i].bActive)
			{
				// Bullet movement
				Bullet[i].Location.x += Bullet[i].Direction.x * BulletSpeed * GetFrameTime();
				Bullet[i].Location.y += Bullet[i].Direction.y * BulletSpeed * GetFrameTime();
			}
		}
	}
	else if (Left && Right)
	{
		ShootRate += 4;

		// First half
		for (int i = 0; i < NumOfWay/2; i++)
		{
			// Rotate the spawn point on circle
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
			Angles[i] += RotationSpeed * GetFrameTime();

			for (int j = 0; j < NumOfBullets/2; j++)
			{
				if (!Bullet[j].bActive && ShootRate % 28 == 0)
				{
					Bullet[j].Location = Points[i];
					Bullet[j].Damage = GetRandomValue(20, 40);
					Bullet[j].bActive = true;
					CalculateDirection(j, Points[i]);

					break;
				}
			}
		}

		// Second half
		for (int i = 4; i < NumOfWay; i++)
		{
			// Rotate the spawn point on circle
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
			Angles[i] -= RotationSpeed * GetFrameTime();

			for (int j = NumOfBullets/2; j < NumOfBullets; j++)
			{
				if (!Bullet[j].bActive && ShootRate % 28 == 0)
				{
					Bullet[j].Location = Points[i];
					Bullet[j].Damage = GetRandomValue(20, 40);
					Bullet[j].bActive = true;
					CalculateDirection(j, Points[i]);

					break;
				}
			}
		}

		for (int i = 0; i < NumOfBullets; i++)
		{
			if (Bullet[i].bActive)
			{
				// Bullet movement
				Bullet[i].Location.x += Bullet[i].Direction.x * BulletSpeed * GetFrameTime();
				Bullet[i].Location.y += Bullet[i].Direction.y * BulletSpeed * GetFrameTime();
			}
		}
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
	bIsSpacePressed = false;

	for (int i = 0; i < NumOfBullets; i++)
	{
		if (!Bullet[i].bActive)
		{
			// Set inital bullet location to be the center location
			Center = {GetMousePosition().x - Bullet[i].Radius, GetMousePosition().y - Bullet[i].Radius};
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
				Bullet.erase(Bullet.begin()+i);
				NumOfBullets--;
			}
		}
}
