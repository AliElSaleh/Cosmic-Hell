#include "Bullet.h"
#include "Player.h"


void Bullet::Init()
{
	Spacing = 35;
	Location.x = 0.0f;
	Location.y = -20.0f;
	Center.x = Location.x + float(Sprite.width)/8;
	Center.y = Location.y + float(Sprite.height);
	Speed = 200.0f;
	Radius = float(Sprite.width)/2 - 4;
	Damage = GetRandomValue(10, 15);
	bActive = true;
	bIsHit = false;
	FramesCounter = 0;
	CollisionOffset.x = Location.x + Radius;
	CollisionOffset.y = Location.y + Radius;
	bDebug = false;
}

void Bullet::InitArray(const int i)
{
	Init();

	Location.x = 650 - float(i) * float(Spacing) + float(Player->Sprite.width)/12;
}

void Bullet::InitWave(const int Wave)
{
	switch (Wave)
	{
		case 1:
			Speed = 200.0f;
		break;

		case 2:
			Spacing = 20;
			Speed = 300.0f;
			Damage = GetRandomValue(15, 25);
		break;

		case 3:
		break;

		case 4:
		break;

		case 5:
		break;

		default:
		break;
	}
}

void Bullet::Update()
{
	FramesCounter++;

	// Movement
	Location.y += Speed * GetFrameTime();
	Center.x = Location.x + float(Sprite.width)/8;
	Center.y = Location.y + float(Sprite.height);

	// Collision checks
	CollisionOffset.x = Location.x + Radius;
	CollisionOffset.y = Location.y + Radius;
	CheckCollisionWithPlayer();

	FramesCounter = 0;
}

void Bullet::Draw() const
{
	if (bActive && !bIsHit)
		DrawTexture(Sprite, int(Location.x), int(Location.y), WHITE);

	if (bDebug)
		DrawCircle(Location.x + float(Sprite.width)/2, Location.y + float(Sprite.height)/2, Radius, WHITE); // Enemy red bullets

}

bool Bullet::IsOutsideWindow() const
{
	bool bOutsideWindow = false;

	// Bullet collision with bottom of window 
	if (Location.y - Radius > GetScreenHeight())
		bOutsideWindow = true;

	return bOutsideWindow;
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

void Bullet::ResetBullet()
{
	Location.y = 0.0f;
	bActive = true;
	bIsHit = false;
}
