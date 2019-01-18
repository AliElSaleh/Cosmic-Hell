#pragma once
#include <vector>
#include "Boid.h"
#include "Ship.h"

struct Flock
{
	Flock();

	void Init();
	void Update();
	void Draw();

	std::vector<Boid*> Boids{};
	std::vector<Ship*> Ships{};

private:
	void CheckBoidsWindowEdges();
	void SetNewDirection();
};

