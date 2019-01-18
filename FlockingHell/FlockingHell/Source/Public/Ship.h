#pragma once
#include "Enemy.h"


struct Ship : Enemy
{
	Ship();

	void Init() override;
	void Update() override;
	void Draw() override;

	void Flock(std::vector<Ship*> *Boids);

	Vector2 Seek(const Vector2& DestLocation);
	void Flee(const Vector2& DestLocation);

	Vector2 Align(std::vector<Ship*> *Boids);
	Vector2 Cohere(std::vector<Ship*> *Boids);
	Vector2 Separate(std::vector<Ship*> *Boids);

	Vector2 Velocity{};
	Vector2 DesiredVelocity{};
	Vector2 Steering{}; // Force
	Vector2 Arrival{}; // Force
	Vector2 Acceleration{};

	float Mass{};

	bool bFlock{};

private:
	float AlignmentForce{0.5f};
	float CohesionForce{0.05f};
	float SeparationForce{0.2f};
	float GoalSeekForce{0.2f};
	float MaxSpeed{2.0f};
	float MaxVelocity{};
	float MaxForce{};

	float TargetRadius{};
	
	float CurrentRotation{0.0f};
	float Rotation{0.0f};

	Vector2 Origin{};

	Rectangle BoidFrameRec{};
	Rectangle BoidDestFrameRec{};

	unsigned short BoidSpriteFramesCounter{};
	unsigned short BoidCurrentFrame{};
	unsigned short FramesSpeed{10};

	Vector2 Limit(Vector2 V, float Amount) const;

	void ApplyForce(Vector2 Force);
	void ApplyBehaviours(std::vector<Ship*> *Boids);

	void UpdateShipAnimation();
};

