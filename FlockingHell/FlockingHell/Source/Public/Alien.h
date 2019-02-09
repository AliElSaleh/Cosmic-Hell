#pragma once

#include "Enemy.h"
#include "RandomPattern.h"
#include "LinearPattern.h"
#include "LinearMultiPattern.h"
#include "SpreadPattern.h"

struct Alien final : Enemy
{
	Alien();

	void Init() override;
	void Update() override;
	void Draw() override;

protected:

	void InitBullet(BulletPatternGenerator& BulletPattern, BulletPatternGenerator::Pattern Pattern) override;
	void UpdateBullet() override;
	void DrawBullet() override;

	void IncreasePlayerScore() override;

private:

	struct LinearPattern LinearBullet;
	struct LinearMultiPattern LinearMultiBullet[2];
	struct SpiralMultiPattern SpiralBullet[2];
	struct SpiralMultiWayPattern SpiralMultiBullet;
	struct SpreadPattern SpreadBullet;
	struct RandomPattern RageBullet[2];
};

