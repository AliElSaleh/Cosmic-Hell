#include "raylib.h"

// Function Declarations
//-----------------------------------------
void Draw();
void Update();
//-----------------------------------------

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		Update();

		Draw();
	}

	CloseWindow();

    return 0;
}

// Function Definitions
//-----------------------------------------
void Update()
{
	
}

void Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

	EndDrawing();
}
//-----------------------------------------