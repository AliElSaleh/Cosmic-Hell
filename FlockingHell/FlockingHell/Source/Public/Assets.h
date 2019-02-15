#pragma once
#include <map>
#include <raylib.h>

// Singleton class
struct Assets
{
	static Assets& Get()
	{
		static Assets me;
		return me;
	}

	Assets(Assets const&) = delete; // Get rid of default copy constructor
	void operator=(Assets const&) = delete; // Get rid of assignment operator

	Texture2D GetSprite(const std::string& Name)
	{
		return Sprites[Name];
	}

	Image GetImage(const std::string& Name)
	{
		return Images[Name];
	}

	Font GetFont(const std::string& Name)
	{
		return Fonts[Name];
	}

	Sound GetSound(const std::string& Name)
	{
		return Sounds[Name];
	}

	Music GetMusic(const std::string& Name)
	{
		return PiecesOfMusic[Name];
	}

	void LoadSprites();
	void UnloadSprites();

	void LoadImages();
	
	void LoadFonts();
	void UnloadFonts();

	void LoadSounds();
	void UnloadSounds();

	void LoadMusic();
	void UnloadMusic();

private:
	Assets();
	~Assets();

	std::map<std::string, Texture2D> Sprites;
	std::map<std::string, Font> Fonts;
	std::map<std::string, Sound> Sounds;
	std::map<std::string, Music> PiecesOfMusic;
	std::map<std::string, Image> Images;
};

