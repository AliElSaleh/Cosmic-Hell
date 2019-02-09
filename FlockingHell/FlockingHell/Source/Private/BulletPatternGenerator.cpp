#include "Globals.h"
#include "BulletPatternGenerator.h"
#include "Bullet.h"
#include "Enemy.h"

#include <raymath.h>
#include <algorithm>
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

void BulletPatternGenerator::Delay(const float Seconds)
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
		if (AmountOfSpirals < 3)
			AmountOfSpirals = 3;
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
		if (AmountOfSpirals < 1 || AmountOfSpirals > 1)
			AmountOfSpirals = 1;

		NumOfBullets = AmountOfBullets;
		NumOfSpiral = AmountOfSpirals;
		NumOfWay = AmountOfWays;
		RotationSpeed = RotSpeed;
		BulletSpeed = Speed;
		CircleRadius = Radius;
		Angle = 0.0f;

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

void BulletPatternGenerator::CreateSpreadPattern(const unsigned short AmountOfBullets, const unsigned short AmountOfWays, const float Speed, const float Radius)
{
	NumOfBullets = AmountOfBullets;
	NumOfWay = AmountOfWays;
	BulletSpeed = Speed;
	CircleRadius = Radius;
	Angle = 45.0f;

	Bullet.reserve(NumOfBullets);
	Points.reserve(NumOfWay);
	Angles.reserve(NumOfWay);

	for (int i = 0; i < NumOfBullets; i++)
		Bullet.emplace_back(::Bullet());

	Offset = 90.0f/NumOfWay;

	for (int i = 0; i < NumOfWay; i++)
	{
		Angles.emplace_back(Angle+=Offset);
		Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));
	}

	BulletRadius = Bullet[0].Radius;
}

void BulletPatternGenerator::CreateCirclePattern(const bool Hole, const unsigned short AmountOfBullets, const float Speed, const float Radius)
{
	NumOfBullets = AmountOfBullets;
	BulletSpeed = Speed;
	CircleRadius = Radius;

	Bullet.reserve(NumOfBullets);
	Points.reserve(NumOfBullets);
	Angles.reserve(NumOfBullets);
	Angle = 90.0f;

	for (int i = 0; i < NumOfBullets; i++)
		Bullet.emplace_back(::Bullet());

	if (Hole)
		Offset = 360.0f/NumOfBullets - 0.1f;
	else
		Offset = 360.0f/NumOfBullets;

	for (int i = 0; i < NumOfBullets; i++)
	{
		Angles.emplace_back(Angle+=Offset);
		Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));
	}

	BulletRadius = Bullet[0].Radius;
}

void BulletPatternGenerator::CreateRandomPattern(const bool Spiral, const unsigned short AmountOfBullets, const unsigned short AmountOfSpirals, const float Speed, const float RotSpeed, const float Radius)
{
	NumOfBullets = AmountOfBullets;
	NumOfSpiral = AmountOfSpirals;
	BulletSpeed = Speed;
	RotationSpeed = RotSpeed;
	CircleRadius = Radius;

	Bullet.reserve(NumOfBullets);

	if (Spiral)
	{
		Points.reserve(NumOfSpiral);
		Angles.reserve(NumOfSpiral);
	}

	for (int i = 0; i < NumOfBullets; i++)
		Bullet.emplace_back(::Bullet());

	if (Spiral)
	{
		if (NumOfSpiral > 2)
			Offset = 360.0f/NumOfSpiral;

		for (int i = 0; i < NumOfSpiral; i++)
		{
			Angles.emplace_back(Angle+=Offset);
			Points.emplace_back(Vector2({Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)}));
		}
	}

	BulletRadius = Bullet[0].Radius;
}

void BulletPatternGenerator::UpdateLinearBullet(const bool LockOn)
{
	if (LockOn)
	{
		if(!bRelease)
		{
			for (int i = 0; i < Bullet.size(); i++)
			{
				// Update bullet location to spawn location
				Center = {Location.x - Bullet[i].Radius, Location.y - Bullet[i].Radius};
				Bullet[i].Location = Center;
				
				// Calculate direction to target
				CalculateDirection(i, TargetLocation);
			}
		}
		else
		{
			Delay(DelayAmount);
		}
	}
	else
	{
		if (!bRelease)
			for (int i = 0; i < Bullet.size(); i++)
			{
				// Update bullet location to spawn location
				Center = { Location.x - Bullet[i].Radius, Location.y - Bullet[i].Radius };
				Bullet[i].Location = Center;
				CalculateDirection(i, TargetLocation);
			}
		else
		{
			Delay(DelayAmount);
		}

		// Calculate direction to target
		//for (int i = 0; i < NumOfBullets; i++)
		//	CalculateDirection(i, TargetLocation);
	}
}

void BulletPatternGenerator::UpdateSpiralBullet(const bool Double)
{
	if (Double)
	{
		if(!bRelease)
		{
			for (int i = 0; i < 2; i++)
			{
				Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
				Bullet[i].Location = {Points[i].x - BulletRadius, Points[i].y - BulletRadius};
			}

			for (int i = 0; i < Bullet.size(); i++)
			{
				Center = {Location.x - Bullet[i].Radius, Location.y - Bullet[i].Radius};
			}				
		}
		else
		{
			Delay(DelayAmount);
		}
	}
	else
	{
		if (!bRelease)
			for (int i = 0; i < Bullet.size(); i++)
			{
				Center = { Location.x - Bullet[i].Radius, Location.y - Bullet[i].Radius };
				Points[0] = { Center.x + CircleRadius * cosf(Angle*DEG2RAD), Center.y + CircleRadius * sinf(Angle*DEG2RAD) };

				Bullet[i].Location = { Points[0].x - BulletRadius, Points[0].y - BulletRadius };
			}
		else
		{
			Delay(DelayAmount);
		}
	}
}

void BulletPatternGenerator::UpdateSpiralMultiBullet()
{
	if(!bRelease)
	{
		int k = 0;
		for (int i = 0; i < NumOfSpiral; i++)
		{
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};

			for (int j = k; j < Bullet.size()/NumOfSpiral+k; j++)
			{
				Center = {Location.x - Bullet[j].Radius, Location.y - Bullet[j].Radius}; // Spawn point

				Bullet[j].Location = {Points[i].x - BulletRadius, Points[i].y - BulletRadius};
			}

			k += Bullet.size()/NumOfSpiral;
		}
	}
	else
	{
		Delay(DelayAmount);
	}
}

void BulletPatternGenerator::UpdateLinearMultiBullet(const bool Aiming)
{
	if(!bRelease)
	{
		if (NumOfWay == 5)
			Angle = Vector2Angle(Center, TargetLocation) - 45.0f; // Offset 90 degress minus the custom angle 45 for 5 way
		if (NumOfWay == 6)
			Angle = Vector2Angle(Center, TargetLocation) - 55.0f; // Offset 90 degress minus the custom angle 55 for 6 way
		else if (NumOfWay == 8)
			Angle = Vector2Angle(Center, TargetLocation) - 55.0f; // Offset 90 degress minus the custom angle 55 for 8 way
		else
			Angle = Vector2Angle(Center, TargetLocation) - 75.0f; // Offset 90 degress minus the default angle 15 for 11 way aiming

		int k = 0;
		for (int i = 0; i < NumOfWay; i++)
		{
			if (Aiming)
				Angles[i] = Angle += Offset;
		
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
			
			for (int j = k; j < Bullet.size()/NumOfWay+k; j++)
			{
				// Update spawn point on circle
				Center = {Location.x - Bullet[j].Radius, Location.y - Bullet[j].Radius};
				Bullet[j].Location = Points[i];
			}
			
			k += Bullet.size()/NumOfWay;
		}
	}
	else
	{
		Delay(DelayAmount);
	}
}

void BulletPatternGenerator::UpdateSpiralMultiBullet(const bool MultiWay)
{
	if(!bRelease)
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
					
					for (int k = l; k < Bullet.size()/NumOfSpiral/NumOfWay+l; k++)
					{
						Center = {Location.x - Bullet[k].Radius, Location.y - Bullet[k].Radius};
						Bullet[k].Location = Points[j];
					}
					
					l += Bullet.size()/NumOfSpiral/NumOfWay;
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
				
				for (int k = l; k < Bullet.size()/NumOfWay+l; k++)
				{
					Center = {Location.x - Bullet[k].Radius, Location.y - Bullet[k].Radius};
					Bullet[k].Location = Points[j];
				}
				
				l += Bullet.size()/NumOfWay;
			}
		}
	}
	else
	{
		Delay(DelayAmount);
	}
}

void BulletPatternGenerator::UpdateSpreadBullet(const bool LockOn)
{
	if(!bRelease)
	{
		if (LockOn)
			Angle = Vector2Angle(Center, TargetLocation) - 45.0f; // Offset 90 degress minus the starting angle 45

		int k = 0;
		for (int i = 0; i < NumOfWay; i++)
		{		
			if (LockOn)
				Angles[i] = Angle += Offset;

			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
			
			for (int j = k; j < Bullet.size()/NumOfWay+k; j++)
			{
				// Update spawn point on circle
				Center = {Location.x - Bullet[j].Radius, Location.y - Bullet[j].Radius};
				Bullet[j].Location = Points[i];
			}
			
			k += Bullet.size()/NumOfWay;
		}
	}
	else
	{
		Delay(DelayAmount);
	}
}

void BulletPatternGenerator::UpdateCircleBullet(const bool LockOn)
{
	if(!bRelease)
	{
		if (LockOn)
			Angle = Vector2Angle(Center, TargetLocation);

		for (int j = 0; j < Bullet.size(); j++)
		{
			Center = {Location.x - Bullet[j].Radius, Location.y - Bullet[j].Radius};

			if (LockOn)
				Angles[j] = Angle += Offset;

			// Update spawn point on circle
			Points[j] = {Center.x + CircleRadius * cosf(Angles[j]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[j]*DEG2RAD)};
			Bullet[j].Location = Points[j];
		}
	}
	else
	{
		Delay(DelayAmount);
	}
}

void BulletPatternGenerator::UpdateRandomBullet(const bool Spiral)
{
	if (!bRelease)
	{
		if (NumOfSpiral > 2)
		{
			int k = 0;
			for (int i = 0; i < NumOfSpiral; i++)
			{
				Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};

				for (int j = k; j < Bullet.size()/NumOfSpiral+k; j++)
				{
					Center = {Location.x - Bullet[j].Radius, Location.y - Bullet[j].Radius}; // Spawn point
					Bullet[j].Location = {Points[i].x, Points[i].y};
				}

				k += Bullet.size()/NumOfSpiral;
			}
		}
		else
		{
			for (int i = 0; i < Bullet.size(); i++)
			{
				// Update bullet location to spawn location
				Center = {Location.x - Bullet[i].Radius, Location.y - Bullet[i].Radius};

				if (Spiral)
				{
					for (int j = 0; j < NumOfSpiral; j++)
					{
						Points[j] = {Center.x + CircleRadius * cosf(Angles[j]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[j]*DEG2RAD)};
						Bullet[i].Location = {Points[j].x, Points[j].y};
					}	
				}
				else
					Bullet[i].Location = Center;
			}
		}
	}
	else
	{
		Delay(DelayAmount);
	}
}

void BulletPatternGenerator::DestroyActiveBullets()
{
	const auto Predicate = [](const ::Bullet &b) { return b.bActive; };
	
	for (int i = 0; i < Bullet.size(); i++)
		if (Bullet[i].bActive)
		{
			Bullet.erase(std::remove_if(Bullet.begin(), Bullet.end(), Predicate), Bullet.end());
			NumOfBullets = unsigned short(Bullet.size());
		}
}

void BulletPatternGenerator::UpdateAnimation()
{
	if (!Bullet.empty())
		for (int i = 0; i < Bullet.size(); i++)
			Bullet[i].UpdateAnimation();
}

void BulletPatternGenerator::StartShotRoutine()
{
	switch (CurrentPattern)
	{
		case LINEAR:
			UpdateLinearPattern();
		break;

		case LINEAR_LOCK_ON:
			UpdateLinearTargetPattern(false);
		break;

		case LINEAR_AIMING:
			UpdateLinearTargetPattern(true);
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

		case SPIRAL_FIVE_WAY:
			UpdateSpiralMultiPattern(false);
		break;

		case SPIRAL_SIX_WAY:
			UpdateSpiralMultiPattern(false);
		break;

		case SPIRAL_SEVEN_WAY:
			UpdateSpiralMultiPattern(false);
		break;

		case SPIRAL_EIGHT_WAY:
			UpdateSpiralMultiPattern(false);
		break;

		case SPIRAL_MULTI_THREE_WAY:
			UpdateSpiralMultiPattern(true);
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
			UpdateSpreadPattern();
		break;

		case SPREAD_EIGHT_WAY_LOCK_ON:
			UpdateSpreadPattern();
		break;

		case CIRCLE:
			UpdateCirclePattern();
		break;

		case CIRCLE_HOLE:
			UpdateCirclePattern();
		break;

		case CIRCLE_HOLE_LOCK_ON:
			UpdateCirclePattern();
		break;

		case RANDOM:
			UpdateRandomPattern(false, false, false);
		break;

		case RANDOM_ALL_RANGE:
			UpdateRandomPattern(true, false, false);
		break;

		case RANDOM_AIMING:
			UpdateRandomPattern(false, true, false);
		break;

		case RANDOM_SPIRAL:
			UpdateRandomPattern(false, false, true);
		break;

		case RANDOM_SPIRAL_MULTI:
			UpdateRandomPattern(false, false, true);
		break;

		default:
		break;
	}
}

void BulletPatternGenerator::UpdateLinearPattern()
{
	ShootRate += 2;

	for (int i = 0; i < Bullet.size(); i++)
	{
		if (!Bullet[i].bActive && ShootRate % 20 == 0)
		{
			Bullet[i].Location = Center;
			Bullet[i].Damage = GetRandomValue(20, 40);
			Bullet[i].bActive = true;
			break;
		}
	}

	for (int i = 0; i < Bullet.size(); i++)
	{
		if (Bullet[i].bActive)
		{
			// Bullet movement
			Bullet[i].Location.y += BulletSpeed * GetFrameTime();
		}
	}
}

void BulletPatternGenerator::UpdateLinearTargetPattern(const bool Aiming)
{
	ShootRate += 2;

	for (int i = 0; i < Bullet.size(); i++)
	{
		if (!Bullet[i].bActive && ShootRate % 20 == 0)
		{
			Bullet[i].Location = Center;
			Bullet[i].Damage = GetRandomValue(20, 40);
			Bullet[i].bActive = true;

			if (Aiming)
				CalculateDirection(i, TargetLocation);

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

		for (int i = 0; i < Bullet.size()/2; i++)
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

		for (int i = Bullet.size()/2; i < Bullet.size(); i++)
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

		for (int i = 0; i < Bullet.size(); i++)
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

			for (int j = 0; j < Bullet.size()/2; j++)
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

			for (int j = Bullet.size()/2; j < Bullet.size(); j++)
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

			for (int j = 0; j < Bullet.size(); j++)
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

	Angle = Vector2Angle(Center, TargetLocation) - 75.0f; // Offset 90 degress minus the default angle 15

	for (int i = 0; i < NumOfWay; i++)
	{
		for (int j = 0; j < Bullet.size(); j++)
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
				for (int k = h; k < Bullet.size(); k++)
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

				h += Bullet.size()/NumOfSpiral/NumOfWay;
			}

			l += NumOfWay;
		}
	}
	else
	{
		for (int i = 0; i < NumOfWay; i++)
		{
			for (int j = 0; j < Bullet.size(); j++)
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

void BulletPatternGenerator::UpdateSpreadPattern()
{
	ShootRate += 2;

	for (int i = 0; i < NumOfWay; i++)
	{
		for (int j = 0; j < Bullet.size(); j++)
		{
			if (!Bullet[j].bActive && ShootRate % 20 == 0)
			{
				Bullet[j].Location = Points[i];
				Bullet[j].Damage = GetRandomValue(20, 40);
				BulletSpeed += 1.5f;
				Bullet[j].bActive = true;
				CalculateDirection(j, Points[i]);
				break;
			}
		}
	}

	ApplyBulletMovement();
}

void BulletPatternGenerator::UpdateCirclePattern()
{
	for (int i = 0; i < Bullet.size(); i++)
	{
		if (!Bullet[i].bActive)
		{
			CalculateDirection(i, Points[i]);
			Bullet[i].bActive = true;
		}
	}

	ApplyBulletMovement();
}

void BulletPatternGenerator::UpdateRandomPattern(const bool AllRange, const bool Aiming, const bool Spiral)
{
	ShootRate += 3;

	unsigned short Deviation;

	if (Spiral)
	{
		if (NumOfSpiral > 2)
			Deviation = 4;
		else
			Deviation = 8;

		for (int i = 0; i < NumOfSpiral; i++)
		{
			// Rotate the spawn point on circle clockwise
			Points[i] = {Center.x + CircleRadius * cosf(Angles[i]*DEG2RAD), Center.y + CircleRadius * sinf(Angles[i]*DEG2RAD)};
			Angles[i] += RotationSpeed * GetFrameTime();

			for (int j = 0; j < Bullet.size(); j++)
			{
				if (!Bullet[j].bActive && ShootRate % 24 == 0)
				{
					Bullet[j].Location = Points[i];
					Bullet[j].Damage = GetRandomValue(20, 40);
					Bullet[j].bActive = true;

					if(Angles[i] > 90 && Angles[i] < 270)
						CalculateDirection(j, Vector2({float(GetRandomValue(int(Points[i].x), int(Points[i].x) - Deviation)), float(GetRandomValue(int(Points[i].y), int(Points[i].y) - Deviation))}));
					else 
						CalculateDirection(j, Vector2({float(GetRandomValue(int(Points[i].x), int(Points[i].x) + Deviation)), float(GetRandomValue(int(Points[i].y), int(Points[i].y) + Deviation))}));	

					break;
				}
			
			}
		}
	}
	else
	{
		Deviation = 200;

		for (int i = 0; i < Bullet.size(); i++)
		{
			if (!Bullet[i].bActive && ShootRate % 24 == 0)
			{
				Bullet[i].Location = Center;
				Bullet[i].Damage = GetRandomValue(20, 40);
				Bullet[i].bActive = true;

				if (AllRange)
					CalculateDirection(i, Vector2({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, GetScreenHeight()))}));
				else if (Aiming)
					CalculateDirection(i, Vector2({float(GetRandomValue(int(TargetLocation.x) - Deviation, int(TargetLocation.x) + DummySprite.width + Deviation)), float(GetRandomValue(int(TargetLocation.y), int(TargetLocation.y) + DummySprite.height))}));
				else
					CalculateDirection(i, Vector2({float(GetRandomValue(int(Bullet[i].Location.x) - Deviation, int(Bullet[i].Location.x) + Deviation)), float(GetRandomValue(int(Bullet[i].Location.y), GetScreenHeight()))}));
			
				break;
			}
		}
	}

	ApplyBulletMovement();
}

void BulletPatternGenerator::DrawDummy() const
{
	DrawTexture(DummySprite, int(TargetLocation.x), int(TargetLocation.y), WHITE);
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
		TargetLocation = {500.0f, 600.0f};
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
			TargetLocation.x -= DummySpeed * GetFrameTime();

		if (IsKeyDown(KEY_D))
			TargetLocation.x += DummySpeed * GetFrameTime();

		if (IsKeyDown(KEY_S))
			TargetLocation.y += DummySpeed * GetFrameTime();

		if (IsKeyDown(KEY_W))
			TargetLocation.y -= DummySpeed * GetFrameTime();
	}
}

void BulletPatternGenerator::AddDebugSwitchPatternCode()
{
	if (!Bullet.empty() && bRelease)
		bIsInProgress = true;
	else
		bIsInProgress = false;

	if (!bIsInProgress)
	{
		if (IsKeyPressed(KEY_LEFT))
		{
			if (static_cast<Pattern>(int(CurrentPattern-1)) < 0)
				SetBulletPattern(static_cast<Pattern>(RANDOM_SPIRAL_MULTI));
			else
				SetBulletPattern(static_cast<Pattern>(int(CurrentPattern-1)));

			Init();
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			if (static_cast<Pattern>(int(CurrentPattern+1)) > RANDOM_SPIRAL_MULTI)
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
	bRelease = false;

	for (int i = 0; i < Bullet.size(); i++)
	{
		if (!Bullet[i].bActive)
		{
			// Set inital bullet location to be the center location
			Center = {Location.x - Bullet[i].Radius, Location.y - Bullet[i].Radius};
			Bullet[i].Location = Center;

			// Calculate direction to target
			CalculateDirection(i, TargetLocation);
		}
	}
}

void BulletPatternGenerator::ApplyBulletMovement()
{
	for (int i = 0; i < Bullet.size(); i++)
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
	if (Bullet.size() > 0)
		for (int i = 0; i < Bullet.size(); i++)
		{
			if (Bullet[i].Location.x > GetScreenWidth()-PANEL_WIDTH || 
				Bullet[i].Location.x + Bullet[i].Radius*2 < 0 ||
				Bullet[i].Location.y - Bullet[i].Radius > GetScreenHeight() ||
				Bullet[i].Location.y + Bullet[i].Radius*2 < 0)
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

void BulletPatternGenerator::SetDelayAmount(const float Seconds)
{
	DelayAmount = Seconds;
}

void BulletPatternGenerator::SetDebug(const bool Condition)
{
	bDebug = Condition;
}
