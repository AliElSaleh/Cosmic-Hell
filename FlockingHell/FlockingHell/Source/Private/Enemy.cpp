#include "Enemy.h"
#include "Player.h"
#include <raymath.h>

Enemy::Enemy()
	: Location(),
	  HitboxOffset(),
	  Destination(),
	  Sprite(),
	  Hitbox(),
	  Speed(100.0f),
	  Health(800),
	  Damage(GetRandomValue(20, 30)),
	  bIsDestinationSet(false),
	  bActive(true),
	  bIsDead(false),
	  bDebug(false)
{
}

void Enemy::Init()
{
	Location = {300.0f, 100.0f};
	HitboxOffset = {50.0f, 105.0f};
	Hitbox = {Location.x + HitboxOffset.x, Location.y + HitboxOffset.y, float(Sprite.width)/11, float(Sprite.height)/4};
	Health = 100;
	Speed = 120.0f;
	Damage = GetRandomValue(20, 30);
	bActive = true;
	bIsDead = false;
	bDebug = false;

	SetDestLocation({float(GetRandomValue(0 + Sprite.width, GetScreenWidth() - Sprite.width)), float(GetRandomValue(0 + Sprite.height/2, 200))});
}

void Enemy::Update()
{
	if (bActive && !bIsDead)
	{
		if (!IsLowHealth())
			MoveToLocation(Destination);
		else
			MoveToLocation({float(GetRandomValue(0, GetScreenWidth())), float(GetRandomValue(0, 200))});

		Hitbox.x = Location.x + HitboxOffset.x;
		Hitbox.y = Location.y + HitboxOffset.y;
	}

	CheckCollisionWithPlayerBullets();
	CheckHealth();

	IsAtLocation(Destination);
}

void Enemy::Draw() const
{
	if (bActive && !bIsDead)
		DrawTexture(Sprite, int(Location.x), int(Location.y), WHITE);

	if (bDebug && bActive && !bIsDead)
	{
		DrawRectangle(Hitbox.x, Hitbox.y, Hitbox.width, Hitbox.height, WHITE);
		DrawText(FormatText("Demon Health: %02i", Health), 10, 60, 20, RED);
	}
}

void Enemy::CheckCollisionWithPlayerBullets()
{
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
		if (CheckCollisionCircleRec(Player->Bullet[i].Center, Player->Bullet[i].Radius, Hitbox))
			if (bActive && !bIsDead)
			{
				Player->ResetBullet(i);

				Health -= Player->Damage;
			}
}

void Enemy::CheckHealth()
{
	if (Health <= 0 && bActive && !bIsDead)
	{
		Health = 0;
		Player->EnemiesKilled++;
		bActive = false;
		bIsDead = true;
	}
}

void Enemy::SetDestLocation(const Vector2 DestLocation)
{
	Destination = DestLocation;
}

void Enemy::MoveToLocation(Vector2 DestLocation)
{
	// Calculate the direction to destination
	Vector2 Direction = Vector2Subtract(DestLocation, Location);
	Direction = Vector2Normalize(Direction);

	// Move to destination
	Location.x += Direction.x * Speed * GetFrameTime();
	Location.y += Direction.y * Speed * GetFrameTime();
}

void Enemy::IsAtLocation(Vector2 DesiredLocation)
{
	if (Location.x > DesiredLocation.x - TOLERANCE && Location.y > DesiredLocation.y - TOLERANCE)
	{
		bIsDestinationSet = false;

		if (!bIsDestinationSet)
		{
			SetDestLocation({float(GetRandomValue(0 + Sprite.width, GetScreenWidth() - Sprite.width)), float(GetRandomValue(0 - Sprite.height/4, 200))});
			bIsDestinationSet = true;
		}
	}
}

bool Enemy::IsLowHealth()
{
	bool bLowHealth = false;
	
	if (Health <= 50)
	{
		bLowHealth = true;
	}

	return bLowHealth;
}
