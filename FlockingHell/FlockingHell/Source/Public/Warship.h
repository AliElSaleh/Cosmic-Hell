#pragma once

#include "Enemy.h"
#include "LinearPattern.h"
#include "CirclePattern.h"
#include "SpiralOneWayPattern.h"
#include "SpiralMultiPattern.h"
#include "LinearMultiPattern.h"
#include "RandomPattern.h"

struct Warship final : Enemy
{
	Warship();

	void Init() override;
	void Update() override;
	void Draw() override;

protected:
	void InitBullet(BulletPatternGenerator& BulletPattern, const Vector2& SpawnLocation, float Delay, BulletPatternGenerator::Pattern Pattern) override;
	void UpdateBullet() override;
	void DrawBullet() override;

	void UpdateBulletComponents(BulletPatternGenerator& BulletPattern, const Vector2& Location, const Vector2& TargetLocation) const;

	void CheckCollisionWithPlayerBullets() override;

	void IncreasePlayerScore() override;

private:
	Rectangle Hitbox[2]{};

	struct LinearPattern LinearBullet[4]{};
	struct SpiralMultiWayPattern SpiralMultiWayBullet[3]{};
	struct LinearPattern LinearAimingBullet[9]{};
	struct CirclePattern CircleBullet[5];
	struct SpiralOneWayPattern SpiralOneWayBullet[2];
	struct SpiralMultiPattern SpiralMultiBullet[4];
	struct LinearMultiPattern LinearMultiBullet[4];
	struct RandomPattern RageBullet[4];

	Vector2 SpawnLocation[4]{};
	Vector2 CanonSpawnLocation{};
};

