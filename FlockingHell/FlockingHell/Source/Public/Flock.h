#pragma once

#include "RocketShip.h"

#include <vector>

template <class T>
struct Flock
{
	Flock();
	explicit Flock(unsigned short AmountOfBoids);

	void Init();
	void Update();
	void Draw();

	std::vector<Enemy*> Boids{};

	struct Player* Player{};
private:
	Vector2 TargetLocation{};

	void CheckBoidsWindowEdges();
	void IsAtLocation(Vector2 Location);

	void SetGoalLocation(Vector2 GoalLocation);
};

#include "../Private/Flock.cpp"