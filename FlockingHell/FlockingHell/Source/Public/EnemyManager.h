#pragma once
#include "Enemy.h"
#include "HealthPickup.h"

#include <vector>

struct EnemyManager
{
	EnemyManager();
	~EnemyManager();

	void Init();
	void AddEnemy(Enemy* NewEnemyType);
	void Update();
	void Draw();

	std::vector<Enemy*> Enemies{};

	void Reset();
private:
	std::vector<HealthPickup*> Heart{};

	void RemoveEnemy(unsigned short Where);

	bool bIsEnemyDead{};
};

