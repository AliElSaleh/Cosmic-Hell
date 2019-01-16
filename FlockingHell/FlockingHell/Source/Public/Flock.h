#pragma once
#include <vector>
#include "Boid.h"

struct Flock
{
	Flock();

	void Init();
	void Update();
	void Draw();

	std::vector<Boid*> Boids{};

private:
	void CheckBoidsWindowEdges();
	void SetNewDirection();
};

