#include "BulletPatternGenerator.h"
#include "Bullet.h"
#include "Assets.h"

#include <raymath.h>

#define Stringify(name) #name

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

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
	Stringify(SPIRAL MIX),
	Stringify(FIVE WAY LINEAR),
	Stringify(SIX WAY LINEAR),
	Stringify(SEVEN WAY),
	Stringify(EIGHT WAY LINEAR),
	Stringify(ELEVEN WAY AIMING),
	Stringify(TWENTY WAY),
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
	BulletSprite = GetAsset(RedBullet);
	DummySprite = GetAsset(Alien);

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
			CreateLinearPattern(5, 500.0f);
		break;

		case LINEAR_LOCK_ON:
			CreateLinearPattern(5, 500.0f);

			AddDebugInitCode();
		break;

		case LINEAR_AIMING:
			CreateLinearPattern(10, 400.0f);
			
			AddDebugInitCode();
		break;

		case SPIRAL_RIGHT:
			CreateSpiralPattern(false, 300, 100.0f, 1000.0f, 1.0f);
		break;

		case SPIRAL_LEFT:
			CreateSpiralPattern(false, 100, 150.0f, 200.0f, 20.0f);
		break;
		
		case SPIRAL_DOUBLE:
			CreateSpiralPattern(true, 400, 200.0f, 400.0f, 1.0f);
		break;

		case SPIRAL_MULTI_RIGHT:
			CreateSpiralMultiPattern(false, 400, 4, 200.0f, 200.0f, 1.0f);
		break;

		case SPIRAL_MULTI_LEFT:
			CreateSpiralMultiPattern(false, 400, 4, 200.0f, 200.0f, 1.0f);
		break;

		case SPIRAL_MULTI_DOUBLE:
			CreateSpiralMultiPattern(true, 400, 4, 200.0f, 100.0f, 1.0f);
		break;

		case SPIRAL_MIX:
			//CreateSpiralMixPattern(true, 400, 4, 200.0f, 100.0f, 1.0f);
		break;

		case FIVE_WAY_LINEAR:
			CreateLinearMultiPattern(200, 5, 300.0f, 30.0f);
		break;

		case SIX_WAY_LINEAR:
			CreateLinearMultiPattern(300, 6, 300.0f, 1.0f);
		break;

		case SEVEN_WAY:
			CreateLinearMultiPattern(700, 7, 300.0f, 1.0f);
		break;

		case EIGHT_WAY_LINEAR:
			CreateLinearMultiPattern(200, 8, 300.0f, 1.0f);
		break;

		case ELEVEN_WAY_AIMING:
			CreateLinearMultiPattern(242, 11, 300.0f, 1.0f);

			AddDebugInitCode();
		break;

		case TWENTY_WAY:
			CreateLinearMultiPattern(400, 20, 300.0f, 1.0f);
		break;

		case THIRTY_WAY:
			CreateLinearMultiPattern(600, 30, 300.0f, 1.0f);
		break;

		case SPIRAL_FOUR_WAY:
		break;

		case SPIRAL_MULTI_FOUR_WAY:
		break;

		case SPREAD_EIGHT_WAY:
		break;

		case SPREAD_EIGHT_WAY_LOCK_ON:
		break;

		case CIRCLE:
		break;

		case CIRCLE_HOLE:
		break;

		case CIRCLE_HOLE_LOCK_ON:
		break;

		case RANDOM:
		break;

		case RANDOM_AIMING:
		break;

		case RANDOM_SPIRAL:
		break;

		case SINE_WAVE_MULTI_WAY_AIMING:
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
			UpdateLinearBullet(false);

			if (bDebug)
				if (IsKeyPressed(KEY_ENTER))
					AddBullet();
		break;

		case LINEAR_LOCK_ON:
			UpdateLinearBullet(true);

			AddDebugUpdateCode();
		break;

		case LINEAR_AIMING:
			UpdateLinearBullet(false);

			// Calculate direction to target
			for (int i = 0; i < NumOfBullets; i++)
				CalculateDirection(i, DummyLocation);
		
			AddDebugUpdateCode();
		break;

		case SPIRAL_RIGHT:
			UpdateSpiralBullet(false);
		break;

		case SPIRAL_LEFT:
			UpdateSpiralBullet(false);
		break;

		case SPIRAL_DOUBLE:
			UpdateSpiralBullet(true);
		break;

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
			//UpdateSpiralMultiBullet();
		break;

		case FIVE_WAY_LINEAR:
			UpdateLinearMultiBullet(false);
		break;

		case SIX_WAY_LINEAR:
			UpdateLinearMultiBullet(false);
		break;

		case SEVEN_WAY:
			UpdateLinearMultiBullet(false);
		break;

		case EIGHT_WAY_LINEAR:
			UpdateLinearMultiBullet(false);
		break;

		case ELEVEN_WAY_AIMING:
			UpdateLinearMultiBullet(true);

			AddDebugUpdateCode();
		break;

		case TWENTY_WAY:
			UpdateLinearMultiBullet(false);
		break;

		case THIRTY_WAY:
			UpdateLinearMultiBullet(false);
		break;

		case SPIRAL_FOUR_WAY:
		break;

		case SPIRAL_MULTI_FOUR_WAY:
		break;

		case SPREAD_EIGHT_WAY:
		break;

		case SPREAD_EIGHT_WAY_LOCK_ON:
		break;

		case CIRCLE:
		break;

		case CIRCLE_HOLE:
		break;

		case CIRCLE_HOLE_LOCK_ON:
		break;

		case RANDOM:
		break;

		case RANDOM_AIMING:
		break;

		case RANDOM_SPIRAL:
		break;

		case SINE_WAVE_MULTI_WAY_AIMING:
		break;

		default:
		break;
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
				DrawDummy();
			break;

			case LINEAR_AIMING:
				DrawDummy();
			break;

			case SPIRAL_RIGHT:
				DrawDebugPoint();
			break;

			case SPIRAL_LEFT:
				DrawDebugPoint();
			break;
				
			case SPIRAL_DOUBLE:
				DrawDebugPoints(2);
			break;

			case SPIRAL_MULTI_RIGHT:
				DrawDebugPoints(NumOfSpiral);

				DrawText(FormatText("Spirals: %0i", NumOfSpiral), 10, 120, 18, WHITE);
			break;

			case SPIRAL_MULTI_LEFT:
				DrawDebugPoints(NumOfSpiral);

				DrawText(FormatText("Spirals: %0i", NumOfSpiral), 10, 120, 18, WHITE);
			break;

			case SPIRAL_MULTI_DOUBLE:
				DrawDebugPoints(NumOfSpiral);

				DrawText(FormatText("Spirals: %0i", NumOfSpiral), 10, 120, 18, WHITE);
			break;

			case FIVE_WAY_LINEAR:
				DrawDebugPoints(NumOfWay);

				DrawText(FormatText("Ways: %0i", NumOfWay), 10, 120, 18, WHITE);
			break;

			case ELEVEN_WAY_AIMING:
				DrawDummy();
				DrawDebugPoints(NumOfWay);

				DrawText(FormatText("Ways: %0i", NumOfWay), 10, 120, 18, WHITE);
			break;

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

void BulletPatternGenerator::UpdateSpiralBullet(const bool Double)
{
	if (Double)
	{
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
			}				
		}
		else
		{
			StartShotRoutine();
			CheckBulletOutsideWindow();
		}
	}
	else
	{
		if (IsKeyPressed(KEY_SPACE))
			bIsSpacePressed = true;

		if (!bIsSpacePressed)
			for (int i = 0; i < NumOfBullets; i++)
			{
				Center = { GetMousePosition().x - Bullet[i].Radius, GetMousePosition().y - Bullet[i].Radius };
				PointOnCircle = { Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD) };

				Bullet[i].Location = { PointOnCircle.x - BulletRadius, PointOnCircle.y - BulletRadius };
			}
		else
		{
			StartShotRoutine();
			CheckBulletOutsideWindow();
		}
	}
}

void BulletPatternGenerator::UpdateSpiralMultiBullet()
{
	if (IsKeyPressed(KEY_SPACE))
		bIsSpacePressed = true;

	if(!bIsSpacePressed)
	{
		int k = 0;
		for (int i = 0; i < NumOfSpiral; i++)
		{
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};

			for (int j = k; j < NumOfBullets/NumOfSpiral+k; j++)
			{
				Center = {GetMousePosition().x - Bullet[j].Radius, GetMousePosition().y - Bullet[j].Radius}; // Spawn point

				Bullet[j].Location = {Points[i].x - BulletRadius, Points[i].y - BulletRadius};
			}

			k += NumOfBullets/NumOfSpiral;
		}
	}
	else
	{
		StartShotRoutine();
		CheckBulletOutsideWindow();
	}
}

void BulletPatternGenerator::UpdateLinearMultiBullet(const bool Aiming)
{
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
				// Update spawn point on circle
				Center = {GetMousePosition().x - Bullet[j].Radius, GetMousePosition().y - Bullet[j].Radius};
				Bullet[j].Location = Points[i];

				//if (Aiming)
				//	Angles[i] = DummyLocation.x;
			}
			
			k += NumOfBullets/NumOfWay;
		}

	}
	else
	{
		StartShotRoutine();
		CheckBulletOutsideWindow();
	}
}

void BulletPatternGenerator::UpdateLinearBullet(const bool LockOn)
{
	if (LockOn)
	{
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
	}
	else
	{
		if (IsKeyPressed(KEY_SPACE))
			bIsSpacePressed = true;

		if (!bIsSpacePressed)
			for (int i = 0; i < NumOfBullets; i++)
			{
				// Update bullet location to spawn location
				Center = { GetMousePosition().x - Bullet[i].Radius, GetMousePosition().y - Bullet[i].Radius };
				Bullet[i].Location = Center;
			}
		else
		{
			StartShotRoutine();
			CheckBulletOutsideWindow();
		}	
	}
}

void BulletPatternGenerator::AddDebugInitCode()
{
	if (bDebug)
		DummyLocation = {500.0f, 600.0f};
}

void BulletPatternGenerator::AddDebugUpdateCode()
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

void BulletPatternGenerator::DrawDebugPoints(const unsigned short Amount)
{
	DrawCircleLines(int(Center.x + BulletRadius), int(Center.y + BulletRadius), CircleRadius, WHITE);

	for (int i = 0; i < Amount; i++)
		DrawCircle(int(Points[i].x), int(Points[i].y), 2.0f, BLUE);
}

void BulletPatternGenerator::DrawDebugPoint() const
{
	DrawCircleLines(int(Center.x + BulletRadius), int(Center.y + BulletRadius), CircleRadius, WHITE);
	DrawCircle(int(PointOnCircle.x), int(PointOnCircle.y), 2.0f, BLUE);
}

void BulletPatternGenerator::DrawDummy() const
{
	DrawTexture(DummySprite, int(DummyLocation.x), int(DummyLocation.y), WHITE);
}

void BulletPatternGenerator::CreateLinearPattern(const unsigned short AmountOfBullets, const float Speed)
{
	NumOfBullets = AmountOfBullets;
	BulletSpeed = Speed;
	Bullet.reserve(NumOfBullets);

	for (int i = 0; i < NumOfBullets; i++)
		Bullet.emplace_back(::Bullet());
}

void BulletPatternGenerator::CreateSpiralPattern(const bool Double, const unsigned short AmountOfBullets, const float Speed, const float RotSpeed, const float Radius)
{
	if (Double)
	{
		NumOfBullets = AmountOfBullets;
		BulletSpeed = Speed;
		RotationSpeed = RotSpeed;
		CircleRadius = Radius;

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
	}
	else
	{
		NumOfBullets = AmountOfBullets;
		BulletSpeed = Speed;
		RotationSpeed = RotSpeed;
		CircleRadius = Radius;
		Bullet.reserve(NumOfBullets);

		for (int i = 0; i < NumOfBullets; i++)
			Bullet.emplace_back(::Bullet());

		BulletRadius = Bullet[0].Radius;
	}
}

void BulletPatternGenerator::CreateSpiralMultiPattern(const bool Double, const unsigned short AmountOfBullets, unsigned short AmountOfSpirals, const float Speed, const float RotSpeed, const float Radius)
{
	if (Double)
	{
		if (AmountOfSpirals < 3)
			AmountOfSpirals = 3;
		else if (AmountOfSpirals > 20)
			AmountOfSpirals = 20;

		NumOfBullets = AmountOfBullets;
		NumOfSpiral = AmountOfSpirals*2;
		BulletSpeed = Speed;
		RotationSpeed = RotSpeed;
		CircleRadius = Radius;

		Bullet.reserve(NumOfBullets);
		Points.reserve(NumOfSpiral);
		Angles.reserve(NumOfSpiral);

		for (int i = 0; i < NumOfBullets; i++)
			Bullet.emplace_back(::Bullet());

		const float Offset = (360.f/(float(NumOfSpiral)/2));

		for (int i = 0; i < NumOfSpiral; i++)
		{
			Angles.emplace_back(Angle+=Offset);
			Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));
		}

		BulletRadius = Bullet[0].Radius;
	}
	else
	{
		if (AmountOfSpirals < 3)
			AmountOfSpirals = 3;
		else if (AmountOfSpirals > 20)
			AmountOfSpirals = 20;

		NumOfBullets = AmountOfBullets;
		NumOfSpiral = AmountOfSpirals;
		BulletSpeed = Speed;
		RotationSpeed = RotSpeed;
		CircleRadius = Radius;

		Bullet.reserve(NumOfBullets);
		Points.reserve(NumOfSpiral);
		Angles.reserve(NumOfSpiral);

		for (int i = 0; i < NumOfBullets; i++)
			Bullet.emplace_back(::Bullet());

		const float Offset = (360.f/NumOfSpiral);

		for (int i = 0; i < NumOfSpiral; i++)
		{
			Angles.emplace_back(Angle+=Offset);
			Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));
		}

		BulletRadius = Bullet[0].Radius;
	}
}

void BulletPatternGenerator::CreateSpiralMixPattern(const bool Left, const unsigned short AmountOfBullets, unsigned short AmountOfSpirals, const float Speed, const float RotSpeed, const float Radius)
{

}

void BulletPatternGenerator::CreateLinearMultiPattern(const unsigned short AmountOfBullets, const unsigned short AmountOfWays, const float Speed, const float Radius)
{
	NumOfBullets = AmountOfBullets;
	NumOfWay = AmountOfWays;
	BulletSpeed = Speed;
	CircleRadius = Radius;
	Angle = 38.0f;

	Bullet.reserve(NumOfBullets);
	Points.reserve(NumOfWay);
	Angles.reserve(NumOfWay);

	for (int i = 0; i < NumOfBullets; i++)
		Bullet.emplace_back(::Bullet());

	float Offset = 90.0f/NumOfWay;
	
	if (NumOfWay == 7 || NumOfWay >= 30 || NumOfWay == 20)
	{
		Angle = 0.0f;
		Offset = 360.0f/NumOfWay;
	}
	else if (NumOfWay == 11)
	{
		Angle = 15.0f;
		Offset = 140.0f/NumOfWay;
	}
	
	for (int i = 0; i < NumOfWay; i++)
	{
		Angles.emplace_back(Angle+=Offset);
		Points.emplace_back(Vector2{Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)});
	}

	BulletRadius = Bullet[0].Radius;
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
			UpdateLinearPattern();
		break;

		case LINEAR_LOCK_ON:
			UpdateLinearTargetPattern();
		break;

		case LINEAR_AIMING:
			UpdateLinearTargetPattern();
		break;

		case SPIRAL_RIGHT:
			UpdateSpiralPattern(false, true);
		break;

		case SPIRAL_LEFT:
			UpdateSpiralPattern(true, false);
		break;

		case SPIRAL_DOUBLE:
			UpdateSpiralPattern(true, true);
		break;

		case SPIRAL_MULTI_RIGHT:
			UpdateSpiralMultiPattern(false, true);
		break;

		case SPIRAL_MULTI_LEFT:
			UpdateSpiralMultiPattern(true, false);
		break;

		case SPIRAL_MULTI_DOUBLE:
			UpdateSpiralMultiPattern(true, true);
		break;

		case SPIRAL_MIX:
			//UpdateSpiralMultiPattern(false, true);
			//UpdateSpiralMultiPattern(true, true);
		break;

		case FIVE_WAY_LINEAR:
			UpdateLinearMultiPattern(false);
		break;

		case SIX_WAY_LINEAR:
			UpdateLinearMultiPattern(false);
		break;

		case SEVEN_WAY:
			UpdateLinearMultiPattern(false);
		break;

		case EIGHT_WAY_LINEAR:
			UpdateLinearMultiPattern(false);
		break;

		case ELEVEN_WAY_AIMING:
			UpdateLinearMultiPattern(true);
		break;

		case TWENTY_WAY:
			UpdateLinearMultiPattern(false);
		break;

		case THIRTY_WAY:
			UpdateLinearMultiPattern(false);
		break;

		case SPIRAL_FOUR_WAY:
		break;

		case SPIRAL_MULTI_FOUR_WAY:
		break;

		case SPREAD_EIGHT_WAY:
		break;

		case SPREAD_EIGHT_WAY_LOCK_ON:
		break;

		case CIRCLE:
		break;

		case CIRCLE_HOLE:
		break;

		case CIRCLE_HOLE_LOCK_ON:
		break;

		case RANDOM:
		break;

		case RANDOM_AIMING:
		break;

		case RANDOM_SPIRAL:
		break;

		case SINE_WAVE_MULTI_WAY_AIMING:
		break;

		default:
		break;
	}
}

void BulletPatternGenerator::UpdateLinearPattern()
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
	}
}

void BulletPatternGenerator::UpdateLinearTargetPattern()
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

	ApplyBulletMovement();
}

void BulletPatternGenerator::UpdateSpiralPattern(const bool Left, const bool Right)
{
	if (Left && Right)
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
	}
	else
	{
		ShootRate += 4;

		if (Left && !Right)
		{
			// Rotate the spawn point on circle counter-clockwise
			PointOnCircle = {Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD)};
			Angle -= RotationSpeed * GetFrameTime();
		}
		else if (Right && !Left)
		{
			// Rotate the spawn point on circle clockwise
			PointOnCircle = {Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD)};
			Angle += RotationSpeed * GetFrameTime();
		}

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
	}

	ApplyBulletMovement();
}

void BulletPatternGenerator::UpdateSpiralMultiPattern(const bool Left, const bool Right)
{	
	if (Left && Right)
	{
		ShootRate += 4;

		// First half
		for (int i = 0; i < NumOfSpiral/2; i++)
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
		for (int i = 4; i < NumOfSpiral; i++)
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
	}
	else
	{
		ShootRate += 4;

		for (int i = 0; i < NumOfSpiral; i++)
		{
			if(Left && !Right)
			{
				// Rotate the spawn point on circle counter-clockwise
				Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
				Angles[i] -= RotationSpeed * GetFrameTime();
			}
			else if(Right && !Left)
			{
				// Rotate the spawn point on circle clockwise
				Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
				Angles[i] += RotationSpeed * GetFrameTime();
			}

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
	}

	ApplyBulletMovement();
}

void BulletPatternGenerator::UpdateLinearMultiPattern(const bool Aiming)
{
	ShootRate += 2;

	for (int i = 0; i < NumOfWay; i++)
	{
		for (int j = 0; j < NumOfBullets; j++)
		{
			if (!Bullet[j].bActive && ShootRate % 20 == 0)
			{
				Bullet[j].Location = Points[i];
				Bullet[j].Damage = GetRandomValue(20, 40);
				Bullet[j].bActive = true;
				CalculateDirection(j, Points[i]);

				break;
			}
		}
	}

	ApplyBulletMovement();
}

void BulletPatternGenerator::SetBulletPattern(const Pattern NewPattern)
{
	CurrentPattern = NewPattern;
}

void BulletPatternGenerator::CalculateDirection(const int i, const Vector2 Target)
{
	Bullet[i].Direction = Vector2Subtract(Center, Target);
	Bullet[i].Direction = Vector2Normalize(Bullet[i].Direction);
	Bullet[i].Direction = Vector2Negate(Bullet[i].Direction);
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
			CalculateDirection(i, DummyLocation);
		}
	}
}

void BulletPatternGenerator::ApplyBulletMovement()
{
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

void BulletPatternGenerator::SetDebug(const bool Condition)
{
	bDebug = Condition;
}
