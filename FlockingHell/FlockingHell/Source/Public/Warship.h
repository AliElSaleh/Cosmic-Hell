#pragma once

#include "Enemy.h"

struct Warship : Enemy
{
	Warship();

	void Init() override;
	void Update() override;
	void Draw() override;
};

