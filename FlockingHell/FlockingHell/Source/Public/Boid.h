#pragma once

#include "Enemy.h"

struct Boid : Enemy
{
	Boid();

	void Init() override;
	void Update() override;
	void Draw() override;

	void Flock(std::vector<Boid*> *Boids);

	void Seek(const Vector2& DestLocation);
	void Flee(const Vector2& DestLocation);

	Vector2 Align(std::vector<Boid*> *Boids);
	Vector2 Cohere(std::vector<Boid*> *Boids);
	Vector2 Separate(std::vector<Boid*> *Boids);

	bool IsAtLocation(const Vector2& GoalLocation) override;

	Vector2 Velocity{};
	Vector2 DesiredVelocity{};
	Vector2 Steering{}; // Force
	Vector2 Arrival{}; // Force
	Vector2 Acceleration{};

	float Mass{};

	bool bFlock{};

private:
	Vector2 Origin{};

	Rectangle BoidFrameRec{};
	Rectangle BoidDestFrameRec{};

	unsigned short BoidSpriteFramesCounter{};
	unsigned short BoidCurrentFrame{};
	unsigned short FramesSpeed{10};

	float AlignMaxForce{0.2f};
	float CohereMaxForce{0.1f};
	float SeparateMaxForce{0.2f};
	float MaxSpeed{2.0f};
	float MaxVelocity{};
	float MaxForce{};

	float CurrentRotation{0.0f};
	float Rotation{0};

	float TargetRadius{};

	void UpdateBoidAnimation();
};

