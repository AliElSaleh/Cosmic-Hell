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

	Vector2 Location{};
	Vector2 Direction{};
	Vector2 Velocity{};
	Vector2 HitboxOffset{};
	Vector2 Destination{};
	Vector2 SpawnLocation{};

	Texture2D Sprite{};

	Rectangle Hitbox{};
	Rectangle SpriteBox{};
	Rectangle FrameRec{};
	Rectangle DestFrameRec{};

	float Speed{};
	float TargetRadius{};

	int FramesCounter{};
	signed short Health{};
	unsigned short Damage{};
	unsigned short ShootRate{};
	unsigned short SpriteFramesCounter{};
	unsigned short CurrentFrame{};
	unsigned short FramesSpeed{10};

	bool bIsDestinationSet{};
	bool bActive{};
	bool bIsDead{};

	// References to other classes
	struct Player* Player = nullptr;
	virtual bool IsAtLocation(const Vector2& GoalLocation);

protected:
	Vector2 Origin{};
	Vector2 DesiredVelocity{};
	Vector2 Steering{};

	// Flocking properties
	float AlignmentForce{0.3f};
	float CohesionForce{0.01f};
	float SeparationForce{0.05f};
	float GoalSeekForce{0.08f};

	float Mass{};
	float MaxForce{};
	float MaxVelocity{};

	unsigned short Frames{10};
	
	Vector2 Align(std::vector<Enemy*> *Boids) const;
	Vector2 Cohere(std::vector<Enemy*> *Boids);
	Vector2 Separate(std::vector<Enemy*> *Boids) const;

	virtual Vector2 Limit(Vector2 V, float Amount) const;
	virtual Vector2 Seek(const Vector2& DestLocation);

	virtual void UpdateAnimation();

	virtual void ApplyBehaviours(std::vector<Enemy*> *Enemies);

	virtual void CheckCollisionWithPlayer();
	virtual void CheckCollisionWithPlayerBullets();
	virtual void CheckHealth();

	void ApplyForce(Vector2 Force);
	void SetDestLocation(Vector2 DestLocation);
	void MoveToLocation(const Vector2& DestLocation);

	virtual bool IsBulletSequenceComplete(const BulletPatternGenerator &BulletPattern);
	virtual bool IsLowHealth() const;

	bool bIsAtLocation = false;
	bool bDebug = false;
};

