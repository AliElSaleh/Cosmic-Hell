#include "Bullet.h"
#include "Player.h"
#include "Assets.h"

void Bullet::Init()
{
	Sprite = GetAsset(FireBullet);

	Speed = 200.0f;
	Radius = 10;
	Damage = GetRandomValue(10, 15);
	bActive = false;
	bIsHit = false;
	CollisionOffset.x = Location.x + Radius;
	CollisionOffset.y = Location.y + Radius;
	bDebug = false;

	BulletFrameRec.x = 0.0f;
	BulletFrameRec.y = 0.0f;
	BulletFrameRec.width = float(Sprite.width)/6;
	BulletFrameRec.height = Sprite.height;
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
		DrawTextureRec(Sprite, BulletFrameRec, Location, WHITE);

	if (bDebug)
		DrawCircle(CollisionOffset.x, CollisionOffset.y, 3.0f, WHITE);
}

void Bullet::UpdateBulletAnimation(const TYPE BulletType)
{
	switch (BulletType)
	{
	case NORMAL:
		break;

	case FIRE:
		BulletSpriteFramesCounter++;

		if (BulletSpriteFramesCounter >= (GetFPS()/FramesSpeed))
		{
			BulletSpriteFramesCounter = 0;
			BulletCurrentFrame++;
		
			if (BulletCurrentFrame > 5)
				BulletCurrentFrame = 0;
		
			BulletFrameRec.x = float(BulletCurrentFrame)*float(Sprite.width)/6;
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
				//bActive = false;
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
		if (bActive)
			Player->GrazingScore += GetRandomValue(130, 150) * GetFrameTime();
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
