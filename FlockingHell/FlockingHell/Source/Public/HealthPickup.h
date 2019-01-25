#pragma once

#include <raylib.h>

struct HealthPickup
{
	HealthPickup();

	void Init();
	void Update();
	void Draw() const;

	Vector2 Location{};
	Vector2 Direction{};

	Texture2D Sprite{};

	bool IsOutsideWindow() const;

private:
	float Speed{};
};

