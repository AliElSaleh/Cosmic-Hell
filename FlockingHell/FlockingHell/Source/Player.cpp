#include "Player.h"
#include "Bullet.h"

Player::Player()
{
	XOffset = 15;
	YOffset = 50;
	Location.x = float(GetScreenWidth()) / 2 + float(XOffset);
	Location.y = float(GetScreenHeight()) - 100;
	BulletSpawnLocation = {0.0f, 0.0f};
	Rotation = {0.0f, 0.0f};
	Hitbox.width = 6;
	Hitbox.height = 6;
	Health = 100;
	Name = "Scarlet";
	*Bullet = {};
	Texture = {};
	
	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;

	bFirstLaunch = true;
	bIsDead = false;
	bIsHit = false;
}

void Player::Init()
{
	if (bFirstLaunch) // If we are opening the application the first time and to prevent loading the same texture every time we die or go to main menu
	{
		Texture = LoadTexture("Sprites/Scarlet.png");
		bFirstLaunch = false;
	}

	XOffset = 15;
	YOffset = 50;
	Location.x = float(GetScreenWidth()) / 2 + float(XOffset);
	Location.y = float(GetScreenHeight()) - 100;
	BulletSpawnLocation.x = Location.x + float(Texture.width/4 )- XOffset;
	BulletSpawnLocation.y = Location.y;
	Rotation = {0.0f, 0.0f};
	Hitbox.x = Location.x + float(Texture.width/4) + float(XOffset);
	Hitbox.y = Location.y + float(Texture.height/4);
	Hitbox.width = 6;
	Hitbox.height = 6;
	Health = 100;
	Name = "Scarlet";
	
	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = float(Texture.width)/4;
	FrameRec.height = float(Texture.height);

	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		Bullet[i].Location = Location;
		Bullet[i].Radius = 3.0f;
		Bullet[i].Speed = 500.0f;
		Bullet[i].Damage = GetRandomValue(20, 40);
		Bullet[i].bActive = false;
	}

	bIsDead = false;
	bIsHit = false;
}

void Player::Update()
{
	/// Player sprite
	if (!bIsDead)
	{
		SpriteFramesCounter++;

		Location.x = GetMousePosition().x - XOffset;
		Location.y = GetMousePosition().y - YOffset;

		Hitbox.x = GetMousePosition().x - Hitbox.width/4;
		Hitbox.y = GetMousePosition().y - Hitbox.height/4 - 5;

		BulletSpawnLocation.x = Location.x + float(Texture.width)/4 - XOffset;
		BulletSpawnLocation.y = Location.y;

		if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
	    {
			SpriteFramesCounter = 0;
			CurrentFrame++;
	        
			if (CurrentFrame > 4)
				CurrentFrame = 0;

			FrameRec.x = float(CurrentFrame)*float(Texture.width)/4;
		}
	}

	/// Player's bullets
	// Initialise bullets
	if (IsKeyDown(KEY_SPACE))
	{
		if (!bIsDead)
		{
			ShootRate += 2;

	        for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	        {
	            if (!Bullet[i].bActive && ShootRate % 20 == 0)
	            {
	                Bullet[i].Location = BulletSpawnLocation;
					Bullet[i].Damage = GetRandomValue(20, 40);
	                Bullet[i].bActive = true;
	                break;
	            }
			}
		}
	}

	// Apply movement to bullets when they become active
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
    {
        if (Bullet[i].bActive)
        {
            // Bullet movement
            Bullet[i].Location.y -= Bullet[i].Speed * GetFrameTime();
		}
	}

	/// Check bullets outside window
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (Bullet[i].Location.y - Bullet[i].Radius < 0)
		{
			Bullet[i].bActive = false;
			Bullet[i].Location = Location;
			ShootRate = 0;
		}
	}

	/// Player Collision check
	if (Location.x + Texture.width/4 > GetScreenWidth())
		Location.x = GetScreenWidth() - Texture.width/4;
	
	if (Location.y + Texture.height > GetScreenHeight())
		Location.y = GetScreenHeight() - Texture.height;

	if (Location.x - Texture.width/4 < 0)
		Location.x = Texture.width/4;
	
	if (Location.y - Texture.height < 0)
		Location.y = Texture.height;

	/// Player health check
	if (Health <= 0)
	{
		// Prevent negative health values
		Health = 0;
		bIsDead = true;
		*GameState = DEATH;
	}
}

void Player::Draw()
{
	// Player bullets
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
		if (Bullet[i].bActive)
			DrawCircleGradient(int(Bullet[i].Location.x), int(Bullet[i].Location.y), Bullet[i].Radius, VIOLET, RED);
   
	// Player sprite
    DrawTextureRec(Texture, FrameRec, Location, WHITE);  // Draw part of the texture
}

Player& Player::GetCurrentPlayer()
{
	return *this;
}
