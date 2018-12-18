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

void Bullet::InitWave(int Wave)
{
	switch (Wave)
	{
		case 1:

		break;

		case 2:
			Speed = 300.0f;
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

	// Collision checks
	CheckCollisionWithPlayerBullets();
	CheckCollisionWithPlayer();

	if (IsOutsideWindow())
	{
		// Do stuff...
	}

	FramesCounter = 0;
}

void Bullet::Draw() const
{
	if (bActive && !bIsHit)
		DrawCircleGradient(int(Location.x), int(Location.y), Radius, WHITE, RED);
}

bool Bullet::IsOutsideWindow() const
{
	bool bOutsideWindow = false;

	// Bullet collision with bottom of window 
	if (Location.y - Radius > GetScreenHeight())
	{
		bOutsideWindow = true;
	}

	return bOutsideWindow;
}

void Bullet::CheckCollisionWithPlayerBullets()
{
	// Player bullet collision with this bullet
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (CheckCollisionCircles(Player->Bullet[i].Location, Player->Bullet[i].Radius, Location, Radius))
			if (Player->Bullet[i].bActive && !bIsHit)
				bActive = false;
	}
}

void Bullet::CheckCollisionWithPlayer()
{
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
}
