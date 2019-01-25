#include "EnemyManager.h"
#include "Demon.h"
#include "Alien.h"

EnemyManager::EnemyManager()
{
	Init();
}

EnemyManager::~EnemyManager() = default;

void EnemyManager::AddEnemy(Enemy *NewEnemyType)
{
	Enemies.emplace_back(NewEnemyType);
}

void EnemyManager::Update()
{
	// Make all enemies inactive if not in the first element of vector
	if (!bIsEnemyDead)
		for (unsigned short i = 1; i < Enemies.size(); i++)
			Enemies[i]->bActive = false;

	// Remove enemy from vector on death
	if (Enemies[0]->bIsDead)
	{
		if (dynamic_cast<Demon*>(Enemies[0])->RageBullet.Bullet.empty())
		{
			RemoveEnemy(0);
			bIsEnemyDead = true;

			Enemies[0]->bActive = true;
			bIsEnemyDead = false;
		}

		if (!Heart.empty())
			Heart[0]->Update();
	}

	// Update the currently active enemy
	if (!Enemies.empty())
		for (unsigned short i = 0; i < Enemies.size(); i++)
		{
			Enemies[i]->Update();
			Heart[0]->Location = Enemies[0]->Location;
		}

	// Remove heart when outside the window
	if (!Heart.empty())
		if (Heart[0]->IsOutsideWindow())
			Heart.erase(Heart.begin());
}

void EnemyManager::Draw()
{
	if (!bIsEnemyDead)
		Enemies[0]->Draw();

	if (!Heart.empty())
		if (Enemies[0]->bIsDead)
			Heart[0]->Draw();

	// Draw the currently active enemy
	if (!Enemies.empty())
		for (unsigned short i = 0; i < Enemies.size(); i++)
			Enemies[i]->Draw();
}

void EnemyManager::Init()
{
	Enemies.reserve(3);
	Enemies.emplace_back(new Demon());
	Enemies.emplace_back(new Alien());
	Enemies.emplace_back(new Alien());

	Heart.emplace_back(new HealthPickup());

	Heart[0]->Init();

	bIsEnemyDead = false;
}

void EnemyManager::RemoveEnemy(const unsigned short Where)
{
	if (!Enemies.empty())
		Enemies.erase(Enemies.begin()+Where);
}
