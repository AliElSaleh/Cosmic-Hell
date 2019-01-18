#pragma once
#include "Bullet.h" // <--- Forward declares player struct

#include <raylib.h>
#include <vector>

#define MAX_ENEMY_BULLETS 50

struct BulletPatternGenerator;
constexpr auto TOLERANCE = 1.0f;

struct Enemy
{
	virtual ~Enemy() = default;
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	virtual void Flock(std::vector<Enemy*> *Boids);

	void StartMoving();
	void StopMoving();

	struct Bullet Bullet[MAX_ENEMY_BULLETS];

	Vector2 Location{};
	Vector2 Direction{};
	Vector2 Velocity{};
	Vector2 HitboxOffset{};
	Vector2 Destination{};
	Vector2 SpawnLocation{};
	Texture2D Sprite{};
	Rectangle Hitbox{};
	Rectangle SpriteBox{};

	float Speed{};
	float TargetRadius{};

	signed short Health{};
	unsigned short Damage{};
	unsigned short ShootRate{};
	int FramesCounter{};

	bool bIsDestinationSet{};
	bool bActive{};
	bool bIsDead{};

	// References to other classes
	struct Player* Player = nullptr;
	virtual bool IsAtLocation(const Vector2& GoalLocation);

protected:
	virtual void CheckCollisionWithPlayer();
	virtual void CheckCollisionWithPlayerBullets();
	virtual void CheckHealth();

	void SetDestLocation(Vector2 DestLocation);
	void MoveToLocation(const Vector2& DestLocation);

	virtual bool IsBulletSequenceComplete(const BulletPatternGenerator &BulletPattern);
	virtual bool IsLowHealth() const;

	bool bIsAtLocation = false;
	bool bDebug = false;
};

