#include "Bullet.h"
#include "Player.h"

void Bullet::Init()
{
	Location.x = 0.0f;
	Location.y = -20.0f;
	Direction = {0.0f, 0.0f};
	Center.x = Location.x + float(Sprite.width)/8;
	Center.y = Location.y + float(Sprite.height);
	Speed = 200.0f;
	Radius = 6;
	Damage = GetRandomValue(10, 15);
	bActive = true;
	bIsHit = false;
	CollisionOffset.x = Location.x + Radius;
	CollisionOffset.y = Location.y + Radius;
	bDebug = false;
}

void Bullet::Update()
{
	// Movement
	Location.y += Speed * GetFrameTime();
	Center.x = Location.x + float(Sprite.width)/8;
	Center.y = Location.y + float(Sprite.height);

	// Collision checks
	CollisionOffset.x = Location.x + Radius;
	CollisionOffset.y = Location.y + Radius;
	CheckCollisionWithPlayer();
}

void Bullet::Draw() const
{
	if (bActive && !bIsHit)
		DrawTexture(Sprite, int(Location.x), int(Location.y), WHITE);

	if (bDebug)
		DrawCircle(int(Location.x) + int(Sprite.width)/2, int(Location.y) + int(Sprite.height)/2, Radius, WHITE); // Enemy red bullets

}

bool Bullet::IsLocationYGreaterThan(const float Y) const
{
	bool bGreaterThanY = false;

	if (Location.y - Radius > Y)
		bGreaterThanY = true;

	return bGreaterThanY;
}

void Bullet::CheckCollisionWithPlayer()
{
	// Enemy bullet collision with player
	if (CheckCollisionCircleRec(CollisionOffset, Radius, Player->Hitbox) && !Player->bIsDead)
	{
		if (!bIsHit)
		{
			// Apply damage to player
			if (bActive)
			{
				Player->Health -= Damage;
				Player->bIsHit = true;
				bIsHit = true;
				bActive = false;
			}
		}
	}
}

void Bullet::Clear()
{
	Location.x = 0.0f;
	Location.y = 0.0f;
	Center.x = 0;
	Center.y = 0;
	Speed = 200.0f;
	Radius = 6;
	bActive = true;
	bIsHit = false;
	CollisionOffset.x = Location.x + Radius;
	CollisionOffset.y = Location.y + Radius;
}
