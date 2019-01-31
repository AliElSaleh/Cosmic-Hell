 #pragma once

#include "Enemy.h"
#include "LinearPattern.h"

struct Ship : Enemy
{
	Ship();

	void Init() override;
	void Update() override;
	void Draw() override;

	void Flock(std::vector<Enemy*> *Boids) override;

protected:
	void ApplyBehaviours(std::vector<Enemy*> *Enemies) override;

	void UpdateAnimation() override;

private:
	struct LinearPattern LinearBullet{};

	float Rotation{0.0f};

	unsigned short FramesCounter{};

	void UpdateBullet();
	void DrawBullet();
};

