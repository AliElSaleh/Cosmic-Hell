#pragma once
#include <raylib.h>
#include "Bullet.h" // <--- Forward declares player struct

#define MAX_ENEMY_BULLETS 50
constexpr auto TOLERANCE = 1.0f;

struct Enemy
{
	Enemy();

	void Init();
	void Update();
	void Draw() const;

	Bullet Bullet[MAX_ENEMY_BULLETS];

	Vector2 Location;
	Vector2 HitboxOffset;
	Vector2 Destination;
	Texture2D Sprite;
	Rectangle Hitbox;

	float Speed;

	signed short Health;
	unsigned short Damage;

	bool bIsDestinationSet;
	bool bActive;
	bool bIsDead;

	// References to other classes
	Player* Player = nullptr;

protected:
	void CheckCollisionWithPlayerBullets();
	void CheckHealth();

	void SetDestLocation(Vector2 DestLocation);
	void MoveToLocation(Vector2 DestLocation);
	void IsAtLocation(Vector2 DesiredLocation);

	bool IsLowHealth();

private:
	bool bDebug;
};

