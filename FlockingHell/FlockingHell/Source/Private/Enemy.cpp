#include "Globals.h"
#include "Enemy.h"
#include "Player.h"

#include <raymath.h>

void Enemy::Init()
{
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
}

void Enemy::Flock(std::vector<Enemy*>* Boids)
{
}

void Enemy::UpdateAnimation()
{
	SpriteFramesCounter++;

	if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
	{
		SpriteFramesCounter = 0;
		CurrentFrame++;
	
		if (CurrentFrame > Frames-1)
			CurrentFrame = 0;
	
		FrameRec.x = float(CurrentFrame)*float(Sprite.width)/Frames;
	}
}

void Enemy::ApplyBehaviours(std::vector<Enemy*>* Enemies)
{
}

void Enemy::CheckCollisionWithPlayer() const
{
	if (bActive && !bIsDead)
		if (CheckCollisionRecs(Player->Hitbox, SpriteBox))
		{			
			if (!Player->bIsDead)
				Player->Health -= 1;

			if (!Player->Heart.empty())
				Player->Heart.pop_back();
		}
}

void Enemy::CheckCollisionWithPlayerBullets()
{
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
		if (CheckCollisionCircleRec(Player->Bullet[i].Center, Player->Bullet[i].Radius, Hitbox))
			if (bActive && !bIsDead)
			{
				Player->ResetBullet(i);
				IncreasePlayerScore();

				Health -= Player->Bullet[0].Damage;
			}
}

void Enemy::CheckHealth()
{
	if (Health <= 0 && bActive && !bIsDead)
	{
		Health = 0;
		bActive = false;
		bIsDead = true;
	}
}

void Enemy::ApplyForce(const Vector2 Force)
{
	Velocity = Vector2Add(Velocity, Force);
	Location = Vector2Add(Location, Vector2Scale(Velocity, Speed * GetFrameTime()));
}

void Enemy::SetDestLocation(const Vector2 DestLocation)
{
	Destination = DestLocation;
}

void Enemy::MoveToLocation(const Vector2& DestLocation)
{
	// Calculate the direction to destination
	Direction = Vector2Subtract(DestLocation, Location);
	Direction = Vector2Normalize(Direction);

	// Move to destination
	Location.x += Direction.x * Speed * GetFrameTime();
	Location.y += Direction.y * Speed * GetFrameTime();
}

void Enemy::UpdateBullet()
{
}

void Enemy::DrawBullet()
{
}

void Enemy::IncreasePlayerScore()
{
}

Vector2 Enemy::Align(std::vector<Enemy*>* Boids) const
{
	Vector2 SumOfVectors = {0.0f, 0.0f};
	const float PerceptionRadius = 50.0f;
	unsigned short Neighbors = 0;

	for (auto Other : *Boids)
	{
		const float Distance = Vector2Distance(Location, Other->Location);

		if (Distance > 0 && Distance < PerceptionRadius)
		{
			SumOfVectors = Vector2Add(SumOfVectors, Other->Velocity);
			Neighbors++;
		}
	}

	if (Neighbors > 0)
	{
		SumOfVectors = Vector2Divide(SumOfVectors, Neighbors); // Get the average velocity vectors

		Vector2 Steer = Vector2Subtract(SumOfVectors, Velocity); // Reynold's steering formula
		Steer = Limit(Steer, MaxForce);

		return Steer;
	}

	return Vector2Zero();
}

Vector2 Enemy::Cohere(std::vector<Enemy*>* Boids)
{
	Vector2 SumOfVectors = {0.0f, 0.0f};
	const float PerceptionRadius = 60.0f;
	unsigned short Neighbors = 0;

	for (auto Other : *Boids)
	{
		const float Distance = Vector2Distance(Location, Other->Location);

		if (Distance > 0 && Distance < PerceptionRadius)
		{
			SumOfVectors = Vector2Add(SumOfVectors, Other->Location);
			Neighbors++;
		}
	}

	if (Neighbors > 0)
	{
		SumOfVectors = Vector2Divide(SumOfVectors, Neighbors); // Get the average velocity vectors

		return Seek(SumOfVectors);
	}

	return Vector2Zero();
}

Vector2 Enemy::Separate(std::vector<Enemy*>* Boids) const
{
	Vector2 SumOfVectors = {0.0f, 0.0f};
	const float DesiredSeparation = float(Sprite.width)/Frames + 10.0f; // 10 is an Arbitrary value
	unsigned short Neighbors = 0;

	for (auto Other : *Boids)
	{
		const float Distance = Vector2Distance(Location, Other->Location);

		if (Distance > 0 && Distance < DesiredSeparation)
		{
			Vector2 Difference = Vector2Subtract(Location, Other->Location);
			Difference = Vector2Normalize(Difference);

			SumOfVectors = Vector2Add(SumOfVectors, Difference);
			Neighbors++;
		}
	}

	if (Neighbors > 0)
	{
		SumOfVectors = Vector2Divide(SumOfVectors, Neighbors);

		Vector2 Steer = Vector2Subtract(SumOfVectors, Velocity);
		Steer = Limit(Steer, MaxForce);
		
		return Steer;
	}

	return Vector2Zero();
}

Vector2 Enemy::Limit(Vector2 V, const float Amount) const
{
	if (V.x > Amount)
		V.x = Amount;

	if (V.y > Amount)
		V.y = Amount;

	return V;
}

Vector2 Enemy::Seek(const Vector2 & DestLocation)
{
	// Arrival logic
	DesiredVelocity = Vector2Subtract(DestLocation, Location);
	const float Distance = Vector2Length(DesiredVelocity);

	if (Distance < TargetRadius)
	{
		// Inside the slowing area - Enemy will slow down
		DesiredVelocity = Vector2Scale(Vector2Normalize(Vector2Subtract(DestLocation, Location)), MaxVelocity * (Distance / TargetRadius));
	}
	else
	{
		// Outside the slowing area - Enemy will move at normal speed
		DesiredVelocity = Vector2Scale(Vector2Normalize(Vector2Subtract(DestLocation, Location)), MaxVelocity);
	}

	// Reynold's Steering formula
	Steering = Vector2Subtract(DesiredVelocity, Velocity);

	Direction = Velocity;

	Steering = Limit(Steering, MaxForce);
	Steering = Vector2Divide(Steering, Mass);

	return Steering;
}

void Enemy::DrawFlockingProperties() const
{
	RDrawText(FormatText("Alignment Force: %f", AlignmentForce), 10, 70, 18, WHITE);
	RDrawText(FormatText("Cohesion Force: %f", CohesionForce), 10, 90, 18, WHITE);
	RDrawText(FormatText("Separation Force: %f", SeparationForce), 10, 110, 18, WHITE);
	RDrawText(FormatText("Goal seeking Force: %f", GoalSeekForce), 10, 130, 18, WHITE);
}

bool Enemy::IsAtLocation(const Vector2& GoalLocation)
{
	DesiredVelocity = Vector2Subtract(GoalLocation, Location);
	const float Distance = Vector2Length(DesiredVelocity);
	
	return Distance < TargetRadius;
}

void Enemy::StartMoving()
{
	Speed = 100.0f;
}

void Enemy::StopMoving()
{
	Speed = 0.0f;
}

bool Enemy::IsBulletSequenceComplete(const BulletPatternGenerator &BulletPattern)
{
	if (BulletPattern.Bullet.empty())
	{
		if (Frames > 0)
			SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH - Sprite.width/Frames)), float(GetRandomValue(0, GetScreenHeight() - 750))});

		if (bDebug)
			printf("Dest Location: X: %f, Y: %f\n", Destination.x, Destination.y);

		StartMoving();

		return true;
	}

	return false;
}

bool Enemy::IsLowHealth() const
{
	return Health <= LowHealthThreshold;
}
