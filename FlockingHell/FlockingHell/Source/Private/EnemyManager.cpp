#include "EnemyManager.h"
#include "Demon.h"
#include "Alien.h"
#include "Flock.h"

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
	if (!Enemies.empty())
		if (!bIsEnemyDead)
			for (unsigned short i = 1; i < Enemies.size(); i++)
				Enemies[i]->bActive = false;

	// Make all flock enemies inactive if not in the first element of vector
	if (!FlockOfEnemies.empty())
		for (unsigned short i = 1; i < FlockOfEnemies.size(); i++)
			for (unsigned short j = 0; j < FlockOfEnemies[i]->Boids.size(); j++)
				FlockOfEnemies[i]->Boids[j]->bActive = false;

	// Remove flock from vector when boids are empty
	if (!FlockOfEnemies.empty())
		if (FlockOfEnemies[0]->Boids.empty() && Enemies[0]->FinalBullets->empty())
			RemoveFlock(0);

	// Remove enemy from vector on death
	if (!Enemies.empty())
		if (Enemies[0]->bIsDead)
		{
			if (Enemies[0]->FinalBullets->empty())
			{
				RemoveEnemy(0);

				if (FlockOfEnemies[0]->Boids.empty())
					RemoveFlock(0);

				bIsEnemyDead = true;

				if (!Enemies.empty())
				{
					Enemies[0]->Init();
					Enemies[0]->bActive = true;
				}

				bIsEnemyDead = false;
			}
		}

	// Remove flock enemy from vector on death
	if (!FlockOfEnemies.empty())
		if (!FlockOfEnemies[0]->Boids.empty())
			for (unsigned short i = 0; i < FlockOfEnemies[0]->Boids.size(); i++)
				if (FlockOfEnemies[0]->Boids[i]->bIsDead)
					if (FlockOfEnemies[0]->Boids[i]->FinalBullets->empty())
						RemoveEnemyFromFlock(i);

	// Update the currently active flock
	if (!FlockOfEnemies.empty())
		if (!FlockOfEnemies[0]->Boids.empty())
			FlockOfEnemies[0]->Update();

	// Update the currently active enemy
	if (!Enemies.empty())
		for (unsigned short i = 0; i < Enemies.size(); i++)
			Enemies[i]->Update();
}

void EnemyManager::Draw()
{
	if (!Enemies.empty())
		if (!bIsEnemyDead)
			Enemies[0]->Draw();

	// Draw the currently active flock
	if (!FlockOfEnemies.empty())
		if (!FlockOfEnemies[0]->Boids.empty())
			FlockOfEnemies[0]->Draw();

	// Draw the currently active enemy
	if (!Enemies.empty())
		for (unsigned short i = 0; i < Enemies.size(); i++)
			Enemies[i]->Draw();
}

void EnemyManager::Reset()
{
	Enemies.clear();
	FlockOfEnemies.clear();
}

void EnemyManager::Init()
{
	Enemies.reserve(3);
	FlockOfEnemies.reserve(3);

	FlockOfEnemies.emplace_back(new Flock(false, 20));
	FlockOfEnemies.emplace_back(new Flock(true, 40));
	FlockOfEnemies.emplace_back(new Flock(true, 40));

	Enemies.emplace_back(new Demon());
	Enemies.emplace_back(new Alien());
	Enemies.emplace_back(new Alien());

	bIsEnemyDead = false;
}

void EnemyManager::RemoveEnemy(const unsigned short Where)
{
	if (!Enemies.empty())
		Enemies.erase(Enemies.begin()+Where);
}

void EnemyManager::RemoveFlock(const unsigned short Where)
{
	if (!FlockOfEnemies.empty())
		FlockOfEnemies.erase(FlockOfEnemies.begin()+Where);
}

void EnemyManager::RemoveEnemyFromFlock(const unsigned short Where)
{
	if (!FlockOfEnemies[0]->Boids.empty())
		FlockOfEnemies[0]->Boids.erase(FlockOfEnemies[0]->Boids.begin()+Where);
}
