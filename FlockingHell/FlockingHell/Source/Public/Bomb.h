#pragma once

#include <vector>

#include "Bullet.h"

struct Bomb
{
	Bomb();

	void Init();
	void Explode();

	std::vector<Bullet> *Bullets;
};

