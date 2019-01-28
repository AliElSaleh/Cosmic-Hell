#pragma once
#include <raylib.h>

struct Planet
{
	Planet();

	void Init();
	void Update();
	void Draw() const;

	Vector2 Location{};
	Vector2 Direction{};

	Texture2D Sprite{};


private:
	void CheckPlanetWindowCollision();

	float Speed{};
	float ArbitraryAmount{};
};

