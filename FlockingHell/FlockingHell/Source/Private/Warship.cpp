#include "Globals.h"
#include "Warship.h"
#include "Player.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Warship::Warship()
{
	Warship::Init();
}

void Warship::Init()
{
	Sprite = GetAsset(Warship);
	SpriteBox = {Location.x, Location.y, float(Sprite.width), float(Sprite.height)};
	Hitbox = {Location.x + 96.0f, Location.y + 50.0f, 90.0f, 80.0f};

	Location = {300.0f, 200.0f};

	Health = 2000;
	Speed = 100.0f;
	Damage = GetRandomValue(20, 30);
	ShootRate = 5;
	bActive = true;
	bIsDead = false;
	bDebug = false;

	SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH-PANEL_WIDTH - Sprite.width)), float(GetRandomValue(0, GetScreenHeight() - 700))});
}

void Warship::Update()
{
	if (bActive && !bIsDead)
	{
		if (!IsLowHealth())
			MoveToLocation(Destination);
		//else
			// TODO: Implement final bullet wave

		// Update sprite box location
		SpriteBox.x = Location.x;
		SpriteBox.y = Location.y;

		// Update hitbox location
		Hitbox.x = Location.x + 96.0f;
		Hitbox.y = Location.y + 50.0f;
	}

	CheckCollisionWithPlayer();

	IsAtLocation(Destination);
}

void Warship::Draw()
{
	if (bDebug && bActive && !bIsDead)
	{
		DrawRectangle(int(SpriteBox.x), int(SpriteBox.y), int(SpriteBox.width), int(SpriteBox.height), WHITE); // A rectangle that its width/height is the same as the sprite's width/height
		DrawRectangle(int(Hitbox.x), int(Hitbox.y), int(Hitbox.width), int(Hitbox.height), GRAY); // Hitbox
		DrawText(FormatText("Warship Health: %02i", Health), 10, 60, 20, RED); // Warship health
	}

	// Draw the warship sprite
	if (bActive && !bIsDead)
		DrawTexture(Sprite, int(Location.x), int(Location.y), WHITE);
}

void Warship::CheckCollisionWithPlayer()
{
	if (bActive && !bIsDead)
		if (CheckCollisionRecs(Player->Hitbox, SpriteBox))
			Player->Health -= Damage;
}

bool Warship::IsAtLocation(const Vector2& GoalLocation)
{
	return false;
}

bool Warship::IsLowHealth() const
{
	bool bLowHealth;

	Health <= 350 ? bLowHealth = true : bLowHealth = false;

	return bLowHealth;
}
