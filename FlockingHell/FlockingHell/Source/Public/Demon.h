#pragma once

#include "Enemy.h"
#include "LinearMultiPattern.h"
#include "CirclePattern.h"
#include "SpiralPattern.h"
#include "SpiralMultiPattern.h"

struct Demon : Enemy
{
	Demon();

	void Init() override;
	void Update() override;
	void Draw() override;

protected:
	void InitBullet(BulletPatternGenerator& BulletPattern, float Delay, BulletPatternGenerator::Pattern Pattern) override;
	void UpdateBullet() override;
	void DrawBullet() override;

	void UpdateAnimation() override;

	void IncreasePlayerScore() override;

private:
	struct CirclePattern CircleBullet[20];
	struct LinearMultiPattern LinearMultiBullet[4];
	struct SpiralPattern SpiralBullet[4];
	struct SpiralMultiPattern RageBullet;
};

