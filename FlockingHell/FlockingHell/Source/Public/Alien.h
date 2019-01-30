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
	struct SpiralMultiPattern SpiralBullet[2];
	struct SpiralMultiWayPattern SpiralMultiBullet;
	struct RandomPattern RageBullet[2];

	void UpdateAnimation() override;

	void UpdateBullet();
	void DrawBullet();
};

