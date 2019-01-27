#pragma once

#include "Enemy.h"
#include "RandomPattern.h"

struct Alien : Enemy
{
	Alien();

	void Init() override;
	void Update() override;
	void Draw() override;

private:
	struct RandomPattern RageBullet;
};

