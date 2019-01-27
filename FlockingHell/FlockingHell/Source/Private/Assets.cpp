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
	Load("PurpleBullet", "Sprites/PurpleBullet.png");
	Load("Demon", "Sprites/Demon.png");
	Load("Alien", "Sprites/Alien.png");
	Load("Warship", "Sprites/Warship.png");
	Load("Boid", "Sprites/Boid.png");
	Load("Panel", "Sprites/Panel.png");
	Load("Heart", "Sprites/Heart.png");
	Load("Bomb", "Sprites/Bomb.png");
	Load("FireBullet", "Sprites/FireBullet.png");
	Load("Divider", "Sprites/Divider.png");
	Load("Explosion", "Sprites/Explosion.png");
	Load("ArchDemon", "Sprites/ArchDemon.png");
	Load("Boid v2", "Sprites/Boidv2.png");
	Load("Background", "Sprites/Background.png");
}

void Assets::UnloadSprites()
{
	UnloadTexture(GetSprite("Player"));
	UnloadTexture(GetSprite("BlueBullet"));
	UnloadTexture(GetSprite("RedBullet"));
	UnloadTexture(GetSprite("PurpleBullet"));
	UnloadTexture(GetSprite("Demon"));
	UnloadTexture(GetSprite("Alien"));
	UnloadTexture(GetSprite("Warship"));
	UnloadTexture(GetSprite("Boid"));
	UnloadTexture(GetSprite("Panel"));
	UnloadTexture(GetSprite("Heart"));
	UnloadTexture(GetSprite("Bomb"));
	UnloadTexture(GetSprite("FireBullet"));
	UnloadTexture(GetSprite("Divider"));
	UnloadTexture(GetSprite("Explosion"));
	UnloadTexture(GetSprite("ArchDemon"));
	UnloadTexture(GetSprite("Boid v2"));
	UnloadTexture(GetSprite("Background"));
}

void Assets::LoadFonts()
{
	const auto Load = [&](const std::string Name, const char* FileName)
	{
		const Font Font = LoadFont(FileName);
		Fonts[Name] = Font;
	};

	Load("Locust", "Fonts/CGF Locust Resistance.ttf");
	Load("VCR", "Fonts/VCR_OSD_MONO_1.001.ttf");
}

void Assets::UnloadFonts()
{
	UnloadFont(GetFont("Locust"));
	UnloadFont(GetFont("VCR"));
}
