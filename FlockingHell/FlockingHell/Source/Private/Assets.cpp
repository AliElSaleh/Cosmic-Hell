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
	Load("GreenBullet", "Sprites/GreenBullet.png");
	Load("FireBullet", "Sprites/FireBullet.png");
	Load("Demon", "Sprites/Demon.png");
	Load("Alien", "Sprites/Alien.png");
	Load("Warship", "Sprites/Warship.png");
	Load("Boid", "Sprites/Boid.png");
	Load("Panel", "Sprites/Panel.png");
	Load("Heart", "Sprites/Heart.png");
	Load("Bomb", "Sprites/Bomb.png");
	Load("Divider", "Sprites/Divider.png");
	Load("Explosion", "Sprites/Explosion.png");
	Load("ArchDemon", "Sprites/ArchDemon.png");
	Load("Boid v2", "Sprites/Boidv2.png");
	Load("MainMenu", "Sprites/Background.png");
	Load("OptionsMenu", "Sprites/OptionsMenu.png");
	Load("GameBackground", "Sprites/GameBackground.png");
	Load("Planet", "Sprites/Planet.png");
	Load("RingPlanet", "Sprites/RingPlanet.png");
	Load("CosmicHellTitle", "Sprites/CosmicHellTitle.png");
	Load("PressAnyKey", "Sprites/PressAnyKey.png");
	Load("DeathState", "Sprites/DeathState.png");
	Load("PauseState", "Sprites/PauseState.png");
	Load("WinState", "Sprites/WinState.png");
	Load("LeaderboardScreen", "Sprites/LeaderboardScreen.png");
	Load("StatsScreen", "Sprites/StatsState.png");
	Load("Selector", "Sprites/Selector.png");
	Load("StartGame", "Sprites/StartGame.png");
	Load("Leaderboard", "Sprites/Leaderboard.png");
	Load("Stats", "Sprites/Stats.png");
	Load("Credits", "Sprites/Credits.png");
	Load("Quit", "Sprites/Quit.png");
}

void Assets::UnloadSprites()
{
	UnloadTexture(GetSprite("Player"));
	UnloadTexture(GetSprite("BlueBullet"));
	UnloadTexture(GetSprite("RedBullet"));
	UnloadTexture(GetSprite("PurpleBullet"));
	UnloadTexture(GetSprite("GreenBullet"));
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
	UnloadTexture(GetSprite("MainMenu"));
	UnloadTexture(GetSprite("OptionsMenu"));
	UnloadTexture(GetSprite("Planet"));
	UnloadTexture(GetSprite("RingPlanet"));
	UnloadTexture(GetSprite("CosmicHellTitle"));
	UnloadTexture(GetSprite("PressAnyKey"));
	UnloadTexture(GetSprite("DeathState"));
	UnloadTexture(GetSprite("PauseState"));
	UnloadTexture(GetSprite("WinState"));
	UnloadTexture(GetSprite("LeaderboardScreen"));
	UnloadTexture(GetSprite("StatsScreen"));
	UnloadTexture(GetSprite("Selector"));
	UnloadTexture(GetSprite("StartGame"));
	UnloadTexture(GetSprite("Leaderboard"));
	UnloadTexture(GetSprite("Stats"));
	UnloadTexture(GetSprite("Credits"));
	UnloadTexture(GetSprite("Quit"));
}

void Assets::LoadImages()
{
	const auto Load = [&](const std::string Name, const char* FileName)
	{
		const Image Image = LoadImage(FileName);
		Images[Name] = Image;
	};

	Load("Icon", "Images/Cosmic-Hell_Logo.png");
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

void Assets::LoadSounds()
{
	const auto Load = [&](const std::string Name, const char* FileName)
	{
		const Sound Sound = LoadSound(FileName);
		Sounds[Name] = Sound;
	};

	Load("Fire", "Sounds/BulletFire.wav");
	Load("MenuSelect", "Sounds/complete.ogg");
	Load("Switch", "Sounds/switch13.wav");
	Load("Select", "Sounds/Click.wav");
}

void Assets::UnloadSounds()
{
	UnloadSound(GetSound("Fire"));
	UnloadSound(GetSound("MenuSelect"));
	UnloadSound(GetSound("Switch"));
	UnloadSound(GetSound("Select"));
}

void Assets::LoadMusic()
{
	const auto Load = [&](const std::string Name, const char* FileName)
	{
		const Music Music = LoadMusicStream(FileName);
		PiecesOfMusic[Name] = Music;
	};

	Load("Menu", "Music/Carpenter_Brut-Hang'Em_All.ogg");
	Load("Credits", "Music/Carpenter_Brut-Sexkiller_On_The_Loose.ogg");
	Load("DemonStage", "Music/Carpenter_Brut-Turbo_Killer.ogg");
	Load("AlienStage", "Music/Carpenter_Brut-Meet_Matt_Stryker.ogg");
	Load("WarshipStage", "Music/Carpenter_Brut-Division_Ruine.ogg");
}

void Assets::UnloadMusic()
{
	UnloadMusicStream(GetMusic("Menu"));
	UnloadMusicStream(GetMusic("Credits"));
	UnloadMusicStream(GetMusic("DemonStage"));
	UnloadMusicStream(GetMusic("AlienStage"));
	UnloadMusicStream(GetMusic("WarshipStage"));
}
