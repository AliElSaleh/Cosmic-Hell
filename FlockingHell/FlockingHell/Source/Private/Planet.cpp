#include "Planet.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Planet::Planet()
{
	Init();
}

void Planet::Init()
{
	Direction = {float(GetRandomValue(-1, 1)), float(GetRandomValue(-1, 1))};
	Speed = 20.0f;
	ArbitraryAmount = 300.0f;

	if (Direction.x == 0)
		Direction.x = 1;
	else if (Direction.y == 0)
		Direction.y = -1;
}

void Planet::Update()
{
	Location.x += Direction.x * Speed * GetFrameTime();
	Location.y += Direction.y * Speed * GetFrameTime();

	CheckPlanetWindowCollision();
}

void Planet::Draw() const
{
	DrawTexture(Sprite, Location.x, Location.y, WHITE);
}

void Planet::CheckPlanetWindowCollision()
{
	// X
	if (Location.x > GetScreenWidth() + ArbitraryAmount)
		Location.x = 0 - Sprite.width;
	else if (Location.x < 0 - Sprite.width - ArbitraryAmount)
		Location.x = GetScreenWidth();

	// Y
	if (Location.y > GetScreenHeight() + ArbitraryAmount)
		Location.y = 0 - Sprite.height;
	else if (Location.y < 0 - Sprite.height - ArbitraryAmount)
		Location.y = GetScreenHeight();
}
