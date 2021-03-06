#include "Globals.h"
#include "Star.h"

void Star::Init()
{
	Location.x = float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH));
	Location.y = float(GetRandomValue(0, GetScreenHeight()));
	Radius = 1.0f;
	Speed = 100.0f;
}

void Star::Update()
{
	// Movement
	Location.y += Speed * GetFrameTime();

	// Check stars outside window
	if (Location.y - Radius > GetScreenHeight())
		Location.y = float(GetRandomValue(-100, 0));
}

void Star::Draw() const
{
	DrawCircle(int(Location.x), int(Location.y), Radius, LIGHTGRAY);
}
