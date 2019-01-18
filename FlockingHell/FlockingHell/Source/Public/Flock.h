#pragma once

#include "Ship.h"

#include <vector>

struct Flock
{
	Flock();

	void Init();
	void Update();
	void Draw();

	std::vector<Enemy*> Boids{};

private:
	Vector2 TargetLocation{};

	void CheckBoidsWindowEdges();
	void IsAtLocation(Vector2 Location);

	void SetGoalLocation(Vector2 GoalLocation);
};

