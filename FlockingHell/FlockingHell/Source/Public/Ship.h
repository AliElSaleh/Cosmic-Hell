#pragma once
#include "Enemy.h"


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
	float CurrentRotation{0.0f};
	float Rotation{0.0f};
};

