#pragma once

#include <vector>

struct EnemyManager
{
	void Init();
	void Update();
	void Draw();

	void RemoveEnemy(unsigned short Where);

	std::vector<struct Enemy> Enemies{};
};

