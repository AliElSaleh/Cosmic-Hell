#pragma once
#include "Enemy.h"
#include "LinearPattern.h"

struct ArchDemon : Enemy
{
	ArchDemon();

	void Init() override;
	void Update() override;
	void Draw() override;

	void Flock(std::vector<Enemy*>* Boids) override;
	void ApplyBehaviours(std::vector<Enemy*> *Enemies) override;

protected:
	void UpdateAnimation() override;

	void IncreasePlayerScore() override;

private:
	void UpdateBullet() override;
	void DrawBullet() override;

	struct LinearPattern LinearBullet{};
};

