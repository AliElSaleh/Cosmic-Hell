#include "Alien.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Alien::Alien()
{
	Alien::Init();
}

void Alien::Init()
{
	Location = {300.0f, 300.0f};
	Health = 120;
	Speed = 100.0f;
	Damage = GetRandomValue(20, 30);
	ShootRate = 5;
	bActive = true;
	bIsDead = false;
	bDebug = false;
}

void Alien::Update()
{
	Location = {300.0f, 300.0f};
	
}

void Alien::Draw()
{
	if (!bIsDead && bActive)
		DrawTexture(GetAsset(Alien), Location.x, Location.y, WHITE);
}

