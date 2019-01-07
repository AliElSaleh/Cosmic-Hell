#include "EnemyManager.h"
#include "Enemy.h"
#include "Alien.h"

EnemyManager::EnemyManager()
{
	Init();
}

EnemyManager::~EnemyManager()
{
	Dispose();
}

EnemyManager::EnemyManager(const EnemyManager &OldEnemyManager)
{
	Enemies = OldEnemyManager.Enemies;
}

EnemyManager::EnemyManager(EnemyManager &&OldEnemyManager) noexcept
{
	Enemies = OldEnemyManager.Enemies;
}

EnemyManager &EnemyManager::operator=(const EnemyManager &OtherManager)
{
	if (this != &OtherManager)
    {
		Enemies.clear();

		Enemies = OtherManager.Enemies;
	}

	return *this;
}

EnemyManager & EnemyManager::operator=(EnemyManager &&OtherManager) noexcept
{
	if (this != &OtherManager)
	{
		Enemies.clear();

		Enemies = OtherManager.Enemies;
	}

	return *this;
}

void EnemyManager::Init()
{
	Enemies.emplace_back(Alien());
}

void EnemyManager::CreateEnemy(Enemy* NewEnemy)
{
	Enemies.reserve(5);
	Enemies.emplace_back(*NewEnemy);
}

void EnemyManager::AddExistingEnemy(const Enemy & ExistingEnemy)
{
	Enemies.push_back(ExistingEnemy);
}

void EnemyManager::Update()
{
	if (!Enemies.empty())
		for (unsigned int i = 0; i < Enemies.size(); i++)
			Enemies[i].Update();

	for (unsigned int i = 0; i < Enemies.size(); i++)
		if (Enemies[i].Health < 150)
			RemoveEnemy(i);
		
}

void EnemyManager::Draw()
{
	if (!Enemies.empty())
		for (unsigned int i = 0; i < Enemies.size(); i++)
			Enemies[i].Draw();
}

void EnemyManager::RemoveEnemy(const unsigned short Where)
{
	if (!Enemies.empty())
		Enemies.erase(Enemies.begin()+Where);
}

void EnemyManager::Dispose()
{
	Enemies.clear();
}
