#include "Bullet.h"
#include "Player.h"

Bullet::Bullet()
{
	Spacing = 35;
	Location.x = 0.0f;
	Location.y = 0.0f;
	Speed = 200.0f;
	Radius = 5.0f;
	Damage = GetRandomValue(10, 15);
	bActive = true;
	bIsHit = false;
	Player = nullptr;
	FramesCounter = 0;
}

void Bullet::Init()
{
	Spacing = 35;
	Location.x = float(Spacing) + float(Player->PlayerSprite.width)/12;
	Location.y = 0;
	Speed = 200.0f;
	Radius = 5.0f;
	Damage = GetRandomValue(10, 15);
	bActive = true;
	bIsHit = false;
	FramesCounter = 0;
}

void Bullet::InitArray(const int i)
{
	Spacing = 35;
	Location.x = float(i) * float(Spacing) + float(Player->PlayerSprite.width)/12;
	Location.y = 0;
	Speed = 200.0f;
	Radius = 5.0f;
	Damage = GetRandomValue(10, 15);
	bActive = true;
	bIsHit = false;
	FramesCounter = 0;
}

void Bullet::Update()
{
	FramesCounter++;

	// Movement
	Location.y += Speed * GetFrameTime();

	/// Collision
	// Player bullet collision with enemy bullets
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (CheckCollisionCircles(Player->Bullet[i].Location, Player->Bullet[i].Radius, Location, Radius))
			if (Player->Bullet[i].bActive && !bIsHit)
				bActive = false;
	}

	// Enemy bullet collision with player
	if (CheckCollisionCircleRec(Location, Radius, Player->Hitbox) && !Player->bIsDead)
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

	// Bullet collision with bottom of window
	if (((FramesCounter/60)%2) == 1)
    {   
		if (Location.y - Radius > GetScreenHeight())
		{
			Location.y = 0;
			bIsHit = false;
			bActive = true;
		}

		FramesCounter = 0;
    }
}

void Bullet::Draw()
{
	if (bActive && !bIsHit)
		DrawCircleGradient(int(Location.x), int(Location.y), Radius, WHITE, RED);
}
