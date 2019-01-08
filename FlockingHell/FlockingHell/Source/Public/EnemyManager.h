#pragma once
#include "Enemy.h"

#include <vector>

struct EnemyManager
{
	EnemyManager();
	~EnemyManager();

	void AddEnemy(Enemy* NewEnemyType);
	void Update();
	void Draw();

	std::vector<Enemy*> Enemies{};

private:
	void Init();

	void RemoveEnemy(unsigned short Where);

	bool bIsEnemyDead{};
};

