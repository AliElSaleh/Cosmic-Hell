#include "BulletPatternGenerator.h"
#include "Bullet.h"

#include <raymath.h>

BulletPatternGenerator::~BulletPatternGenerator() = default;

void BulletPatternGenerator::Init()
{
}

void BulletPatternGenerator::Update()
{
}

void BulletPatternGenerator::Draw()
{
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
			Angles.emplace_back(Angle += 180.0f);
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
		Angle = 0.0f;
		Bullet.reserve(NumOfBullets);

		for (int i = 0; i < NumOfBullets; i++)
			Bullet.emplace_back(::Bullet());

		for (int i = 0; i < 1; i++)
		{
			Angles.emplace_back(Angle);
			Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));
		}

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

	Offset = 90.0f/NumOfWay;
	
	if (NumOfWay == 7 || NumOfWay >= 30 || NumOfWay == 20)
	{
		Angle = 0.0f;
		Offset = 360.0f/NumOfWay;
	}
	else if (NumOfWay == 11)
	{
		Angle = 15.0f; // Default: 15.0f
		Offset = 140.0f/NumOfWay;
	}
	
	for (int i = 0; i < NumOfWay; i++)
	{
		Angles.emplace_back(Angle+=Offset);
		Points.emplace_back(Vector2{Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)});
	}

	BulletRadius = Bullet[0].Radius;
}

void BulletPatternGenerator::CreateSpiralMultiPattern(const bool MultiWay, const unsigned short AmountOfBullets, unsigned short AmountOfSpirals, const unsigned short AmountOfWays, const float Speed, const float RotSpeed, const float Radius)
{
	if (MultiWay)
	{
		if (AmountOfSpirals < 4)
			AmountOfSpirals = 4;
		else if (AmountOfSpirals > 8)
			AmountOfSpirals = 8;

		NumOfBullets = AmountOfBullets;
		NumOfSpiral = AmountOfSpirals;
		NumOfWay = AmountOfWays;
		RotationSpeed = RotSpeed;
		BulletSpeed = Speed;
		CircleRadius = Radius;

		Bullet.reserve(NumOfBullets);
		Points.reserve(NumOfWay*NumOfWay);
		Angles.reserve(NumOfWay*NumOfWay);

		for (int i = 0; i < NumOfBullets; i++)
			Bullet.emplace_back(::Bullet());

		if (NumOfSpiral > NumOfWay)
			Offset = 45.0f/NumOfSpiral;
		else
			Offset = 45.0f/NumOfWay;

		int k = 0;
		for (int i = 0; i < NumOfSpiral; i++)
		{	
			for (int j = k; j < NumOfWay+k; j++)
			{
				Angles.emplace_back(Angle+=Offset);
				Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[j]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[j]*DEG2RAD)}));
			}

			Angle += 360.0f/NumOfSpiral - Offset*NumOfWay;
			
			k += NumOfWay;
		}

		BulletRadius = Bullet[0].Radius;
	}
	else
	{
		NumOfBullets = AmountOfBullets;
		NumOfSpiral = AmountOfSpirals;
		NumOfWay = AmountOfWays;
		RotationSpeed = RotSpeed;
		BulletSpeed = Speed;
		CircleRadius = Radius;

		Bullet.reserve(NumOfBullets);
		Points.reserve(NumOfWay);
		Angles.reserve(NumOfWay);

		for (int i = 0; i < NumOfBullets; i++)
			Bullet.emplace_back(::Bullet());

		Offset = 45.0f/NumOfWay;

		for (int i = 0; i < NumOfWay; i++)
		{
			Angles.emplace_back(Angle+=Offset);
			Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));
		}

		BulletRadius = Bullet[0].Radius;
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

		// Calculate direction to target
		for (int i = 0; i < NumOfBullets; i++)
			CalculateDirection(i, DummyLocation);
	}
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
				Points[0] = { Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD) };

				Bullet[i].Location = { Points[0].x - BulletRadius, Points[0].y - BulletRadius };
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
		if (NumOfWay == 5)
			Angle = Vector2Angle(Center, DummyLocation) - 45.0f; // Offset 90 degress minus the custom angle 45 for 5 way
		if (NumOfWay == 6)
			Angle = Vector2Angle(Center, DummyLocation) - 55.0f; // Offset 90 degress minus the custom angle 55 for 6 way
		else if (NumOfWay == 8)
			Angle = Vector2Angle(Center, DummyLocation) - 55.0f; // Offset 90 degress minus the custom angle 55 for 8 way
		else
			Angle = Vector2Angle(Center, DummyLocation) - 75.0f; // Offset 90 degress minus the default angle 15 for 11 way aiming

		int k = 0;
		for (int i = 0; i < NumOfWay; i++)
		{
			if (Aiming)
				Angles[i] = Angle += Offset;
		
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
			
			for (int j = k; j < NumOfBullets/NumOfWay+k; j++)
			{
				// Update spawn point on circle
				Center = {GetMousePosition().x - Bullet[j].Radius, GetMousePosition().y - Bullet[j].Radius};
				Bullet[j].Location = Points[i];
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

void BulletPatternGenerator::UpdateSpiralMultiBullet(const bool MultiWay)
{
	if (IsKeyPressed(KEY_SPACE))
		bIsSpacePressed = true;

	if(!bIsSpacePressed)
	{
		if (MultiWay)
		{
			int h = 0;
			int l = 0;

			for (int i = 0; i < NumOfSpiral; i++)
			{
				for (int j = h; j < NumOfWay+h; j++)
				{
					// Update points on circle every frame
					Points[j] = {Center.x + CircleRadius * cosf(Angles[j]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[j]*DEG2RAD)};
					
					for (int k = l; k < NumOfBullets/NumOfSpiral/NumOfWay+l; k++)
					{
						Center = {GetMousePosition().x - Bullet[k].Radius, GetMousePosition().y - Bullet[k].Radius};
						Bullet[k].Location = Points[j];
					}
					
					l += NumOfBullets/NumOfSpiral/NumOfWay;
				}

				h += NumOfWay;
			}
		}
		else
		{
			int l = 0;
			for (int j = 0; j < NumOfWay; j++)
			{
				// Update points on circle every frame
				Points[j] = {Center.x + CircleRadius * cosf(Angles[j]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[j]*DEG2RAD)};
				
				for (int k = l; k < NumOfBullets/NumOfWay+l; k++)
				{
					Center = {GetMousePosition().x - Bullet[k].Radius, GetMousePosition().y - Bullet[k].Radius};
					Bullet[k].Location = Points[j];
				}
				
				l += NumOfBullets/NumOfWay;
			}
		}
	}
	else
	{
		StartShotRoutine();
		CheckBulletOutsideWindow();
	}
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
			UpdateSpiralMultiPattern(true, true);
		break;

		case FIVE_WAY_LINEAR:
			UpdateLinearMultiPattern(false);
		break;

		case FIVE_WAY_LINEAR_LOCK_ON:
			UpdateLinearMultiPattern(false);
		break;

		case SIX_WAY_LINEAR:
			UpdateLinearMultiPattern(false);
		break;

		case SIX_WAY_LINEAR_LOCK_ON:
			UpdateLinearMultiPattern(false);
		break;

		case SEVEN_WAY:
			UpdateLinearMultiPattern(false);
		break;

		case EIGHT_WAY_LINEAR:
			UpdateLinearMultiPattern(false);
		break;

		case EIGHT_WAY_LINEAR_LOCK_ON:
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
			UpdateSpiralMultiPattern(false);
		break;

		case SPIRAL_MULTI_FOUR_WAY:
			UpdateSpiralMultiPattern(true);
		break;

		case SPIRAL_MULTI_FIVE_WAY:
			UpdateSpiralMultiPattern(true);
		break;

		case SPIRAL_MULTI_SIX_WAY:
			UpdateSpiralMultiPattern(true);
		break;

		case SPIRAL_MULTI_SEVEN_WAY:
			UpdateSpiralMultiPattern(true);
		break;

		case SPIRAL_MULTI_EIGHT_WAY:
			UpdateSpiralMultiPattern(true);
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
			Points[0] = {Center.x + CircleRadius * cosf(Angles[0]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[0]*DEG2RAD)};
			Angles[0] -= RotationSpeed * GetFrameTime();
		}
		else if (Right && !Left)
		{
			// Rotate the spawn point on circle clockwise
			Points[0] = {Center.x + CircleRadius * cosf(Angles[0]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[0]*DEG2RAD)};
			Angles[0] += RotationSpeed * GetFrameTime();
		}

		for (int i = 0; i < NumOfBullets; i++)
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

	Angle = Vector2Angle(Center, DummyLocation) - 75.0f; // Offset 90 degress minus the default angle 15

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

		if (Aiming)
		{
			Angles[i] = Angle += Offset;
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
		}
	}

	ApplyBulletMovement();
}

void BulletPatternGenerator::UpdateSpiralMultiPattern(const bool MultiWay)
{
	ShootRate += 4;

	Angle = Vector2Angle(Center, Points[0]);

	if (MultiWay)
	{
		int h = 0;
		int l = 0;
		for (int i = 0; i < NumOfSpiral; i++)
		{
			for (int j = l; j < NumOfWay+l; j++)
			{
				for (int k = h; k < NumOfBullets; k++)
				{
					if (!Bullet[k].bActive && ShootRate % 28 == 0)
					{
						Bullet[k].Location = Points[j];
						Bullet[k].Damage = GetRandomValue(20, 40);
						Bullet[k].bActive = true;
						CalculateDirection(k, Points[j]);
						break;
					}
				}

				Angles[j] += RotationSpeed * GetFrameTime();
				Points[j] = {Center.x + CircleRadius * cosf(Angles[j]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[j]*DEG2RAD)};

				h += NumOfBullets/NumOfSpiral/NumOfWay;
			}

			l += NumOfWay;
		}
	}
	else
	{
		for (int i = 0; i < NumOfWay; i++)
		{
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

			Angles[i] += RotationSpeed * GetFrameTime();
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
		}
	}

	ApplyBulletMovement();
}

void BulletPatternGenerator::DrawDummy() const
{
	DrawTexture(DummySprite, int(DummyLocation.x), int(DummyLocation.y), WHITE);
}

void BulletPatternGenerator::DrawDebugPoint() const
{
	DrawCircleLines(int(Center.x + BulletRadius), int(Center.y + BulletRadius), CircleRadius, WHITE);
	DrawCircle(int(Points[0].x + BulletRadius), int(Points[0].y + BulletRadius), 2.0f, BLUE);
}

void BulletPatternGenerator::DrawDebugPoints(const unsigned short Amount)
{
	DrawCircleLines(int(Center.x + BulletRadius), int(Center.y + BulletRadius), CircleRadius, WHITE);

	for (int i = 0; i < Amount; i++)
		DrawCircle(int(Points[i].x + BulletRadius), int(Points[i].y + BulletRadius), 2.0f, BLUE);
}

void BulletPatternGenerator::DrawDebugInfo()
{
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

void BulletPatternGenerator::AddDebugSwitchPatternCode()
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

void BulletPatternGenerator::CalculateDirection(const int i, const Vector2 Target)
{
	Bullet[i].Direction = Vector2Subtract(Center, Target);
	Bullet[i].Direction = Vector2Normalize(Bullet[i].Direction);
	Bullet[i].Direction = Vector2Negate(Bullet[i].Direction);
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

void BulletPatternGenerator::SetBulletPattern(const Pattern NewPattern)
{
	CurrentPattern = NewPattern;
}

void BulletPatternGenerator::SetDebug(const bool Condition)
{
	bDebug = Condition;
}
