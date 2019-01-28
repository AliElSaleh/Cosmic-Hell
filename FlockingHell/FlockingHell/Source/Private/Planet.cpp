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

void Planet::UpdateY()
{
	Location.y += Speed * GetFrameTime();

	CheckPlanetWindowCollision();
}

void Planet::Draw() const
{
	DrawTexture(Sprite, int(Location.x), int(Location.y), WHITE);
}

void Planet::CheckPlanetWindowCollision()
{
	// X
	if (Location.x > GetScreenWidth() + ArbitraryAmount)
		Location.x = 0.0f - float(Sprite.width);
	else if (Location.x < 0.0f - float(Sprite.width) - ArbitraryAmount)
		Location.x = float(GetScreenWidth());

	// Y
	if (Location.y > GetScreenHeight() + ArbitraryAmount)
		Location.y = 0.0f - float(Sprite.height);
	else if (Location.y < 0.0f - float(Sprite.height) - ArbitraryAmount)
		Location.y = float(GetScreenHeight());
}
