#pragma once

#include "Enemy.h"
#include "LinearPattern.h"
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

	bool IsBulletSequenceComplete(const BulletPatternGenerator &BulletPattern) override;

	struct CirclePattern CircleBullet[20];
	struct LinearMultiPattern LinearMultiBullet[4];
	struct SpiralPattern SpiralBullet[4];
	struct SpiralMultiPattern RageBullet;
	
	WAVE BulletWave{};

protected:
	CirclePattern C;
	void UpdateBullet();
	void UpdateAnimation() override;
	void DrawBullet();

	void CheckCollisionWithPlayer() override;
	void CheckCollisionWithPlayerBullets() override;
	void CheckHealth() override;

	bool IsAtLocation(const Vector2& GoalLocation) override;

	bool IsLowHealth() const override;
};

