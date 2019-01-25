#pragma once

#include "Enemy.h"
#include "LinearPattern.h"
#include "LinearMultiPattern.h"
#include "CirclePattern.h"
#include "SpiralPattern.h"
#include "SpiralMultiPattern.h"
#include "Explosion.h"

struct Demon : Enemy
{
	Demon();

	enum WAVE
	{
		FIRST,
		FIRST_A,
		FIRST_B,
		FIRST_C,
		SECOND,
		SECOND_A,
		SECOND_B,
		SECOND_C,
		THIRD,
		THIRD_A,
		THIRD_B,
		THIRD_C,
		RAGE
	};

	void Init() override;
	void Update() override;
	void Draw() override;

	bool IsBulletSequenceComplete(const BulletPatternGenerator &BulletPattern) override;

	struct SpiralPattern SpiralBullet[4];
	struct SpiralMultiPattern RageBullet;
	struct LinearMultiPattern LinearMultiBullet[4];
	struct CirclePattern CircleBullet[20];

	WAVE BulletWave{};

protected:
	Explosion DeathExplosion[20]{};

	void UpdateBullet();
	void UpdateAnimation() override;
	void DrawBullet();

	void CheckCollisionWithPlayer() override;
	void CheckCollisionWithPlayerBullets() override;
	void CheckHealth() override;

	bool IsAtLocation(const Vector2& GoalLocation) override;

	bool IsLowHealth() const override;
};

