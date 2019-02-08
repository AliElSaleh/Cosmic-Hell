#pragma once

#include <raylib.h>
#include <vector>
#include "Explosion.h"
#include "SpiralMultiPattern.h"

struct BulletPatternGenerator;

struct Enemy
{
	enum WAVE
	{
		FIRST,
		FIRST_A,
		FIRST_B,
		FIRST_C,
		SECOND,
		SECOND_A,
		SECOND_B,
		SECOND_C,
		THIRD,
		THIRD_A,
		THIRD_B,
		THIRD_C,
		RAGE
	};

	WAVE BulletWave{};

	virtual ~Enemy() = default;
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	virtual void Flock(std::vector<Enemy*> *Boids);

	void StartMoving();
	void StopMoving();

	Vector2 SpawnLocation{}; // Bullet spawning
	Vector2 Location{};
	Vector2 Direction{};
	Vector2 Velocity{};
	Vector2 Destination{};

	Vector2 HitboxOffset{};

	Texture2D Sprite{};

	Rectangle Hitbox{}; // A box on an enemy that can be damaged by player
	Rectangle SpriteBox{};

	float TargetRadius{};

	// Each inherited class will set the amount of frames of the sprite sheet to properly separate when flocking with local flockmates
	unsigned short Frames{10};

	bool bActive{};
	bool bIsDead{};

	std::vector<Bullet> *FinalBullets;

	// References to player to apply damage and recieve damage
	struct Player* Player = nullptr;

protected:
	Explosion DeathExplosion[20]{};

	// Animation
	Rectangle FrameRec{};
	Rectangle DestFrameRec{};
	unsigned short SpriteFramesCounter{};
	unsigned short CurrentFrame{};
	unsigned short FramesSpeed{10};

	// Movement
	Vector2 Origin{};
	Vector2 DesiredVelocity{};
	Vector2 Steering{};

	// Flocking properties
	float AlignmentForce{0.3f};
	float CohesionForce{0.01f};
	float SeparationForce{0.05f};
	float GoalSeekForce{0.08f};

	// Properties of Enemy
	signed short Health{};
	unsigned short LowHealthThreshold{};
	unsigned short Damage{};
	unsigned short ShootRate{};
	unsigned short Explosions{};

	float Speed{};
	float Mass{};
	float MaxForce{};
	float MaxVelocity{};

	virtual void UpdateBullet();
	virtual void DrawBullet();

	// Flocking functions
	Vector2 Align(std::vector<Enemy*> *Boids) const;
	Vector2 Cohere(std::vector<Enemy*> *Boids);
	Vector2 Separate(std::vector<Enemy*> *Boids) const;

	virtual Vector2 Limit(Vector2 V, float Amount) const;
	virtual Vector2 Seek(const Vector2& DestLocation);

	void DrawFlockingProperties() const;

	// AI Logic functions
	void ApplyForce(Vector2 Force);
	void SetDestLocation(Vector2 DestLocation);
	void MoveToLocation(const Vector2& DestLocation);
	virtual void ApplyBehaviours(std::vector<Enemy*> *Enemies);
	
	// Animation functions
	virtual void UpdateAnimation();

	// Collision check functions
	void CheckCollisionWithPlayer() const;
	virtual void CheckCollisionWithPlayerBullets();
	void CheckHealth();

	// Enemy checks
	bool IsAtLocation(const Vector2& GoalLocation);
	bool IsBulletSequenceComplete(const BulletPatternGenerator &BulletPattern);
	bool IsLowHealth() const;

	// Timing
	unsigned short FramesCounter{};

	bool bDebug{};
};

