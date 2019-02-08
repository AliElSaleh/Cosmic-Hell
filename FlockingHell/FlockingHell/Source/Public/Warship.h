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
	void UpdateBullet() override;
	void DrawBullet() override;

	void CheckCollisionWithPlayerBullets() override;
private:
	Rectangle Hitbox[2]{};

	struct LinearPattern LinearBullet[4]{};
	struct SpiralMultiWayPattern SpiralMultiWayBullet[3]{};

	Vector2 SpawnLocation[4]{};
	Vector2 CanonSpawnLocation{};
};

