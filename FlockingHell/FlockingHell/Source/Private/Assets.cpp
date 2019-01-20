#include "Assets.h"

Assets::Assets() = default;

Assets::~Assets() = default;

void Assets::LoadSprites()
{
	const auto Load = [&](const std::string Name, const char* FileName)
	{
		const Texture2D Sprite = LoadTexture(FileName);
		Sprites[Name] = Sprite;
	};

	Load("Player", "Sprites/Scarlet.png");
	Load("BlueBullet", "Sprites/BlueBullet.png");
	Load("RedBullet", "Sprites/RedBullet.png");
	Load("Demon", "Sprites/Demon.png");
	Load("Alien", "Sprites/Alien.png");
	Load("Warship", "Sprites/Warship.png");
	Load("Boid", "Sprites/Boid.png");
	Load("Panel", "Sprites/Panel.png");
	Load("Heart", "Sprites/Heart.png");
	Load("Bomb", "Sprites/Bomb.png");
}

void Assets::UnloadSprites()
{
	UnloadTexture(GetSprite("Player"));
	UnloadTexture(GetSprite("BlueBullet"));
	UnloadTexture(GetSprite("RedBullet"));
	UnloadTexture(GetSprite("Demon"));
	UnloadTexture(GetSprite("Alien"));
	UnloadTexture(GetSprite("Warship"));
	UnloadTexture(GetSprite("Boid"));
	UnloadTexture(GetSprite("Panel"));
	UnloadTexture(GetSprite("Heart"));
	UnloadTexture(GetSprite("Bomb"));
}
