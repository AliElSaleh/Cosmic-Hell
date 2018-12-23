#pragma once
#include <raylib.h>
#include "Bullet.h" // <--- Forward declares player struct

#define MAX_ENEMY_BULLETS 50
constexpr auto TOLERANCE = 1.0f;

struct Enemy
{
	virtual ~Enemy() = default;
	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	struct Bullet Bullet[MAX_ENEMY_BULLETS];

	Vector2 Location{};
	Vector2 HitboxOffset{};
	Vector2 Destination{};
	Texture2D Sprite{};
	Rectangle Hitbox{};
	Rectangle SpriteBox{};

	float Speed{};

	signed short Health{};
	unsigned short Damage{};

	bool bIsDestinationSet{};
	bool bActive{};
	bool bIsDead{};

	// References to other classes
	struct Player* Player = nullptr;

protected:
	virtual void CheckCollisionWithPlayer();
	virtual void CheckCollisionWithPlayerBullets();
	virtual void CheckHealth();

	void SetDestLocation(Vector2 DestLocation);
	void MoveToLocation(Vector2 DestLocation);
	void IsAtLocation(Vector2 GoalLocation);

	virtual bool IsLowHealth() const;

	bool bDebug = false;
};

