#include "Player.h"

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
	if (Location.x + Size > GetScreenWidth())
		Location.x = GetScreenWidth() - Size;
	
	if (Location.y + Size > GetScreenHeight())
		Location.y = GetScreenHeight() - Size;

	if (Location.x - Size < 0)
		Location.x = Size;
	
	if (Location.y - Size < 0)
		Location.y = Size;

	/// Player health check
	if (Health <= 0)
	{
		// Prevent negative health values
		Health = 0;
		bIsDead = true;
		*GameState = DEATH;
	}
}
