#pragma once

#include "Enemy.h"

struct Boid : Enemy
{
	Boid();

	void Init() override;
	void Update() override;
	void Draw() override;

	bool IsAtLocation(const Vector2& GoalLocation) override;

private:
	Vector2 Origin{};

	Rectangle BoidFrameRec{};
	Rectangle BoidDestFrameRec{};

	unsigned short BoidSpriteFramesCounter{};
	unsigned short BoidCurrentFrame{};
	unsigned short FramesSpeed{10};

	float CurrentRotation{0.0f};
	float Rotation{0};

	void UpdateBoidAnimation();

	float Round(float Number);
};

