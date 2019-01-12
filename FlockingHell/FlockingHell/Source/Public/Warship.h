#pragma once

#include "Enemy.h"

struct Warship : Enemy
{
	Warship();

	void Init() override;
	void Update() override;
	void Draw() override;

private:
	void CheckCollisionWithPlayer() override;

	bool IsAtLocation(const Vector2& GoalLocation) override;
	bool IsLowHealth() const override;
};

