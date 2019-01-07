#pragma once

#include "Enemy.h"

struct Alien : Enemy
{
	Alien();

	void Init() override;
	void Update() override;
	void Draw() override;
};

