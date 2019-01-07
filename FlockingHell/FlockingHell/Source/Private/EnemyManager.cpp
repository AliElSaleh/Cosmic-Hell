#include "EnemyManager.h"
#include "Enemy.h"

void EnemyManager::Init()
{
	Enemies.reserve(5);
	//Enemies.emplace_back(Enemy());
}

void EnemyManager::Update()
{
}

void EnemyManager::Draw()
{
}

void EnemyManager::RemoveEnemy(const unsigned short Where)
{
	if (!Enemies.empty())
		Enemies.erase(Enemies.begin()+Where);
}
