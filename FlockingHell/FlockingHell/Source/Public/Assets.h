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

	void LoadSprites();
	void UnloadSprites();

private:
	Assets();
	~Assets();

	std::map<std::string, Texture2D> Sprites;
};

