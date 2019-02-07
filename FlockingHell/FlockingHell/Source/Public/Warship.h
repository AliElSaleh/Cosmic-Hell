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

private:
	struct LinearPattern LinearBullet[4];

	Vector2 SpawnLocation[4]{};
	Vector2 CanonSpawnLocation{};
};

