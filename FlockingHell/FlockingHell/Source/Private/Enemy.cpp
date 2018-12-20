#include "Enemy.h"


Enemy::Enemy()
	: Location(), Sprite(), Hitbox(), Health(100), Damage(GetRandomValue(20, 30))
{
}

void Enemy::Init()
{
	Location = {0.0f, 0.0f};
	Hitbox = {0.0f, 0.0f, 0.0f, 0.0f};
	Health = 100;
	Damage = GetRandomValue(20, 30);
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
}
