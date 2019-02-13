#include "Bullet.h"
#include "Player.h"


void Bullet::Init()
{
	Speed = 200.0f;
	Radius = 10;
	Damage = GetRandomValue(10, 15);

	bActive = false;
	bIsHit = false;
	bDebug = false;
}

void Bullet::InitFrames()
{
	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = float(Sprite.width)/Frames;
	FrameRec.height = float(Sprite.height);
}

void Bullet::Update()
{
	CollisionOffset.x = Location.x + Radius;
	CollisionOffset.y = Location.y + Radius;

	CheckCollisionWithPlayer();
	CheckCollisionWithPlayerHitbox();

	if (Speed <= 0.0f && bActive)
		bActive = false;
}

void Bullet::Draw() const
{
	if (bActive)
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);

	if (bDebug)
	{
		DrawCircle(int(CollisionOffset.x), int(CollisionOffset.y), float(Sprite.width)/Frames/2, WHITE);
		DrawCircle(int(CollisionOffset.x), int(CollisionOffset.y), 3.0f, YELLOW);
	}
}

void Bullet::UpdateAnimation()
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

void Bullet::CheckCollisionWithPlayerHitbox()
{
	// Enemy bullet collision with player hitbox
	if (CheckCollisionCircleRec(CollisionOffset, float(Sprite.width)/Frames/2, Player->Hitbox) && !Player->bIsDead && !Player->bInvincible)
	{
		if (!bIsHit)
		{
			// Apply damage to player
			if (bActive)
			{
				Player->Health -= 1;
				Player->Score -= Player->Score/100 * 5; // Take off 5 percent of the player's current score

				if (!Player->Heart.empty())
					Player->Heart.pop_back();

				Player->bIsHit = true;
				Player->Hits++;
				bIsHit = true;
			}
		}
	}
}

void Bullet::CheckCollisionWithPlayer() const
{
	// Enemy bullet collision with player sprite
	if (CheckCollisionCircleRec(CollisionOffset, Radius, Player->Spritebox) && !Player->bIsDead)
	{
		// Increase grazing score
		if (bActive && !Player->bIsHit)
			Player->GrazingScore += unsigned short(GetRandomValue(200, 230) * GetFrameTime());
	}
}
