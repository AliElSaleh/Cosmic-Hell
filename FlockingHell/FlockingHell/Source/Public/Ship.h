#pragma once
#include "Enemy.h"


struct Ship : Enemy
{
	Ship();

	void Init() override;
	void Update() override;
	void Draw() override;

	void Flock(std::vector<Enemy*> *Boids) override;

	Vector2 Seek(const Vector2& DestLocation);

	Vector2 Align(std::vector<Enemy*> *Boids) const;
	Vector2 Cohere(std::vector<Enemy*> *Boids);
	Vector2 Separate(std::vector<Enemy*> *Boids) const;

	Vector2 DesiredVelocity{};
	Vector2 Steering{}; // Force

	float Mass{};

private:
	float AlignmentForce{0.3f};
	float CohesionForce{0.01f};
	float SeparationForce{0.05f};
	float GoalSeekForce{0.08f};
	float MaxVelocity{};
	float MaxForce{};
	
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
	void ApplyBehaviours(std::vector<Enemy*> *Boids);

	void UpdateShipAnimation();
};

