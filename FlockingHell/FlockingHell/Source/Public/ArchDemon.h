#pragma once
#include "Enemy.h"

struct ArchDemon : Enemy
{
	ArchDemon();

	void Init() override;
	void Update() override;
	void Draw() override;

	void Flock(std::vector<Enemy*>* Boids) override;
	void ApplyBehaviours(std::vector<Enemy*> *Enemies) override;

	//bool IsAtLocation(const Vector2& GoalLocation) override;
	//bool IsBulletSequenceComplete(const BulletPatternGenerator& BulletPattern) override;
	
	//void CheckCollisionWithPlayer() override;
	//void CheckCollisionWithPlayerBullets() override;
	//void CheckHealth() override;

protected:
	void UpdateAnimation() override;
};

