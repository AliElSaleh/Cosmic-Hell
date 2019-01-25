#include "HealthPickup.h"
#include "Globals.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

HealthPickup::HealthPickup()
{
	Init();
}

void HealthPickup::Init()
{
	Location = {300.0f, 300.0f};
	Direction = {float(GetRandomValue(-1, 1)), float(GetRandomValue(-1, 1))};
	Speed = 30.0f;

	Sprite = GetAsset(Heart);
}

void HealthPickup::Update()
{
	Location.x += Direction.x * Speed * GetFrameTime();
	Location.y += Direction.y * Speed * GetFrameTime();
}

void HealthPickup::Draw() const
{
	DrawTexture(Sprite, Location.x, Location.y, WHITE);
}

bool HealthPickup::IsOutsideWindow() const
{
	return Location.x + Sprite.width < 0 ||
		   Location.x > GetScreenWidth() - PANEL_WIDTH ||
		   Location.y + Sprite.height < 0 ||
	       Location.y > GetScreenHeight();
}

