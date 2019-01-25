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
	FrameRec.height = Sprite.height;
}

void Bullet::Update()
{
	CollisionOffset.x = Location.x + Radius;
	CollisionOffset.y = Location.y + Radius;

	CheckCollisionWithPlayer();
	CheckCollisionWithPlayerHitbox();
}

void Bullet::Draw() const
{
	if (bActive)
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);

	if (bDebug)
		DrawCircle(CollisionOffset.x, CollisionOffset.y, 3.0f, WHITE);
}

void Bullet::UpdateAnimation(const TYPE BulletType)
{
	switch (BulletType)
	{
	case PLAYER:
		SpriteFramesCounter++;

		if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
		{
			SpriteFramesCounter = 0;
			CurrentFrame++;
		
			if (CurrentFrame > Frames-1)
				CurrentFrame = 0;
		
			FrameRec.x = float(CurrentFrame)*float(Sprite.width)/Frames;
		}
		break;

	case FIRE:
		SpriteFramesCounter++;

		if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
		{
			SpriteFramesCounter = 0;
			CurrentFrame++;
		
			if (CurrentFrame > Frames-1)
				CurrentFrame = 0;
		
			FrameRec.x = float(CurrentFrame)*float(Sprite.width)/Frames;
		}
		break;

	case LASER:
		break;

	default:
		break;
	}
}

void Bullet::CheckCollisionWithPlayerHitbox()
{
	// Enemy bullet collision with player hitbox
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
			Player->GrazingScore += GetRandomValue(130, 150) * GetFrameTime();
	}

	Player->bIsHit = false;
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
