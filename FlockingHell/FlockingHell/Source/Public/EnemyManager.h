#pragma once
#include "Enemy.h"

#include <vector>
#include "Flock.h"

struct EnemyManager
{
	EnemyManager();
	~EnemyManager();

	void Init();
	void AddEnemy(Enemy* NewEnemyType);
	void Update();
	void Draw();

	std::vector<Enemy*> Enemies{};
	std::vector<Flock<Enemy>*> FlockOfEnemies{};

	void Reset();

	bool bAllEnemiesDefeated{false};

private:
	void RemoveEnemy(unsigned short Where);
	void RemoveFlock(unsigned short Where);
	void RemoveEnemyFromFlock(unsigned short Where);

	bool bIsEnemyDead{};
};

