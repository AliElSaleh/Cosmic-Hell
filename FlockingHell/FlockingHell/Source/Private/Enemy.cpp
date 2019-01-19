#include "Globals.h"
#include "Enemy.h"

#include <raymath.h>

void Enemy::Init()
{
	Location = {300.0f, -300.0f};
	HitboxOffset = {50.0f, 105.0f};
	Hitbox = {};
	Health = 500;
	Speed = 100.0f;
	Damage = GetRandomValue(20, 30);
	ShootRate = 5;
	bActive = true;
	bIsDead = false;
	bDebug = false;

	SetDestLocation({float(GetRandomValue(0 + Sprite.width, GetScreenWidth()-PANEL_WIDTH - Sprite.width)), float(GetRandomValue(0 + Sprite.height/2, GetScreenHeight() - 650))});
}

void Enemy::Update()
{
	if (bActive && !bIsDead)
	{
		if (!IsLowHealth())
			MoveToLocation(Destination);
		else
			MoveToLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, 150))});

		Hitbox.x = Location.x + HitboxOffset.x;
		Hitbox.y = Location.y + HitboxOffset.y;
	}

	CheckCollisionWithPlayerBullets();
	CheckHealth();
}

void Enemy::Draw()
{
	if (bActive && !bIsDead)
		DrawTexture(Sprite, int(Location.x), int(Location.y), WHITE);

	if (bDebug && bActive && !bIsDead)
	{
		DrawRectangle(int(Hitbox.x), int(Hitbox.y), int(Hitbox.width), int(Hitbox.height), WHITE);
		DrawText(FormatText("Demon Health: %02i", Health), 10, 60, 20, RED);
	}
}

void Enemy::Flock(std::vector<Enemy*>* Boids)
{
}

void Enemy::CheckCollisionWithPlayer()
{

}

void Enemy::CheckCollisionWithPlayerBullets()
{

}

void Enemy::CheckHealth()
{

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

bool Enemy::IsAtLocation(const Vector2& GoalLocation)
{
	if (Location.x > GoalLocation.x - TOLERANCE && Location.y > GoalLocation.y - TOLERANCE) // Is at the goal location?
	{
		bIsDestinationSet = false;

		if (!bIsDestinationSet)
		{
			SetDestLocation({float(GetRandomValue(0 + Sprite.width, GetScreenWidth()-PANEL_WIDTH - Sprite.width)), float(GetRandomValue(0 - Sprite.height/4, GetScreenHeight() - 650))});
			bIsDestinationSet = true;
		}
	}
	else
		bIsDestinationSet = false;

	return bIsDestinationSet;
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
	return false;
}

bool Enemy::IsLowHealth() const
{
	return false;
}
