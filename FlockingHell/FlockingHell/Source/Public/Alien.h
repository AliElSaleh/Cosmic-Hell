#pragma once

#include "Enemy.h"
#include "RandomPattern.h"
#include "LinearPattern.h"

struct Alien : Enemy
{
	Alien();

	void Init() override;
	void Update() override;
	void Draw() override;

private:
	struct LinearPattern LinearBullet;
	struct SpiralPattern SpiralBullet;
	struct RandomPattern RageBullet;

	void UpdateBullet();
	void DrawBullet();
};

