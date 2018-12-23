#pragma once

#include "Enemy.h"
#include "Bullet.h"

#define MAX_DEMON_BULLETS 50

struct Demon : Enemy
{
	void Init() override;
	void Update() override;
	void Draw() const override;

	struct Bullet Bullet[MAX_DEMON_BULLETS];

	struct Player* Player = nullptr;

protected:
	void CheckCollisionWithPlayer() override;
	void CheckCollisionWithPlayerBullets() override;
	void CheckHealth() override;

	bool IsLowHealth() const override;
};

