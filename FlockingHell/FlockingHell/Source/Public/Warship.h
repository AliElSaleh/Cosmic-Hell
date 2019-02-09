#pragma once

#include "Enemy.h"
#include "LinearPattern.h"

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

	void CheckCollisionWithPlayerBullets() override;

	void IncreasePlayerScore() override;

private:
	Rectangle Hitbox[2]{};

	struct LinearPattern LinearBullet[4]{};
	struct SpiralMultiWayPattern SpiralMultiWayBullet[3]{};

	Vector2 SpawnLocation[4]{};
	Vector2 CanonSpawnLocation{};
};

