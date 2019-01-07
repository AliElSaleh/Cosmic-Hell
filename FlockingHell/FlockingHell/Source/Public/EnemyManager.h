#pragma once

#include <vector>

struct Enemy;

struct EnemyManager
{
	EnemyManager(); // Default
	~EnemyManager();
	EnemyManager (const EnemyManager &OldEnemyManager); // Copy constructor
	EnemyManager (EnemyManager &&OldEnemyManager ) noexcept; // Move constructor

	EnemyManager& operator=(const EnemyManager &OtherManager); // Copy assignment
	EnemyManager& operator=(EnemyManager &&OtherManager) noexcept; // Move assignment


	void Init();
	void CreateEnemy(Enemy* NewEnemy);
	void AddExistingEnemy(const Enemy& ExistingEnemy);

	void Update();
	void Draw();

	void RemoveEnemy(unsigned short Where);
	void Dispose();

	std::vector<struct Enemy> Enemies{};
};

