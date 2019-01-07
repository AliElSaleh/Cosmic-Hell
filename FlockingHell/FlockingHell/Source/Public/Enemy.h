#pragma once
#include <raylib.h>
#include <vector>
#include "Bullet.h" // <--- Forward declares player struct

#define MAX_ENEMY_BULLETS 50
constexpr auto TOLERANCE = 1.0f;

struct Enemy
{
	virtual ~Enemy() = default;
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	struct Bullet Bullet[MAX_ENEMY_BULLETS];

	Vector2 Location{};
	Vector2 Direction{};
	Vector2 HitboxOffset{};
	Vector2 Destination{};
	Texture2D Sprite{};
	Rectangle Hitbox{};
	Rectangle SpriteBox{};

	std::vector<struct Enemy> Enemies;

	float Speed{};

	signed short Health{};
	unsigned short Damage{};
	unsigned short ShootRate{};
	int FramesCounter{};

	bool bFirstLaunch{};
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
	void MoveToLocation(const Vector2& DestLocation);
	virtual bool IsAtLocation(const Vector2& GoalLocation);

	void StartMoving();
	void StopMoving();

	virtual bool IsLowHealth() const;

	bool bDebug = false;
};

