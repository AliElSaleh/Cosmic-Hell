#pragma once
#include <raylib.h>

struct Planet
{
	Planet();

	void Init();
	void Update();
	void UpdateY();
	void Draw() const;

	Vector2 Location{};
	Vector2 Direction{};

	Texture2D Sprite{};

	float Speed{};

private:
	void CheckPlanetWindowCollision();

	float ArbitraryAmount{};
};

