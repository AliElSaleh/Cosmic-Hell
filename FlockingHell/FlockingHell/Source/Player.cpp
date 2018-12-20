#include "Player.h"
#include "Bullet.h"

Player::Player()
{
	XOffset = 15;
	YOffset = 50;
	Location.x = float(GetScreenWidth()) / 2 + float(XOffset);
	Location.y = float(GetScreenHeight()) - 100;
	BulletSpawnLocation = Location;
	Rotation = {0.0f, 0.0f};
	Hitbox.width = 6;
	Hitbox.height = 6;
	Health = 100;
	Name = "Scarlet";
	*Bullet = {};
	Sprite = {};
	BulletSprite = {};
	
	PlayerFrameRec = {0.0f, 0.0f, 0.0f, 0.0f};
	BulletFrameRec = {0.0f, 0.0f, 0.0f, 0.0f};

	bFirstLaunch = true;
	bIsDead = false;
	bIsHit = false;
	bDebug = false;
}

void Player::Init()
{
	if (bFirstLaunch) // If we are opening the application the first time and to prevent loading the same texture every time we die or go to main menu
	{
		Sprite = LoadTexture("Sprites/Scarlet.png");
		BulletSprite = LoadTexture("Sprites/BlueBullet.png");
		bFirstLaunch = false;
	}

	XOffset = 15;
	YOffset = 50;
	Location.x = float(GetScreenWidth()) / 2 + float(XOffset);
	Location.y = float(GetScreenHeight()) - 100;
	BulletSpawnLocation.x = Location.x + float(Sprite.width)/4 - XOffset;
	BulletSpawnLocation.y = Location.y;
	Rotation = {0.0f, 0.0f};
	Hitbox.x = Location.x + float(Sprite.width)/4 + float(XOffset);
	Hitbox.y = Location.y + float(Sprite.height)/4;
	Hitbox.width = 4;
	Hitbox.height = 4;
	Health = 100;
	Name = "Scarlet";
	
	PlayerFrameRec.x = 0.0f;
	PlayerFrameRec.y = 0.0f;
	PlayerFrameRec.width = float(Sprite.width)/4; // 4 frames
	PlayerFrameRec.height = float(Sprite.height);

	BulletFrameRec.x = 0.0f;
	BulletFrameRec.y = 0.0f;
	BulletFrameRec.width = float(BulletSprite.width)/4; // 4 frames
	BulletFrameRec.height = float(BulletSprite.height);

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
	// Update Player location and animation if player is still alive
	if (!bIsDead)
	{
		PlayerSpriteFramesCounter++;
		BulletSpriteFramesCounter++;

		Location.x = GetMousePosition().x - XOffset - 2;
		Location.y = GetMousePosition().y - YOffset;

		Hitbox.x = Location.x + float(Sprite.width/4 - 45);
		Hitbox.y = Location.y + float(Sprite.height/4 + 22);

		BulletSpawnLocation.x = Location.x + float(Sprite.width)/4 - XOffset - 3;
		BulletSpawnLocation.y = Location.y;

		UpdatePlayerAnimation();

		UpdateBulletAnimation();
	}

	CheckCollisionWithWindow();
	CheckPlayerHealth();

	// Bullet
	UpdateBullet();
	CheckBulletOutsideWindow();
}

void Player::UpdatePlayerAnimation()
{
	// Player sprite animation
	if (PlayerSpriteFramesCounter >= (GetFPS()/FramesSpeed))
	{
		PlayerSpriteFramesCounter = 0;
		PlayerCurrentFrame++;
	        
		if (PlayerCurrentFrame > 4)
			PlayerCurrentFrame = 0;

		PlayerFrameRec.x = float(PlayerCurrentFrame)*float(Sprite.width)/4;
	}
}

void Player::UpdateBulletAnimation()
{
	// Bullet sprite animation
	if (BulletSpriteFramesCounter >= (GetFPS()/FramesSpeed))
	{
		BulletSpriteFramesCounter = 0;
		BulletCurrentFrame++;

		if (BulletCurrentFrame > 4)
			BulletCurrentFrame = 0;

		BulletFrameRec.x = float(BulletCurrentFrame)*float(BulletSprite.width)/4;
	}
}

void Player::UpdateBullet()
{
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
}

void Player::CheckBulletOutsideWindow()
{
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (Bullet[i].Location.y + BulletSprite.height < 0)
		{
			Bullet[i].bActive = false;
			Bullet[i].Location = Location;
			ShootRate = 0;
		}
	}
}

void Player::CheckCollisionWithWindow()
{
	if (Location.x + float(Sprite.width)/4 > GetScreenWidth())
		Location.x = GetScreenWidth() - float(Sprite.width)/4;
	
	if (Location.y + Sprite.height > GetScreenHeight())
		Location.y = GetScreenHeight() - float(Sprite.height);

	if (Location.x < 0)
		Location.x = 0;
	
	if (Location.y < 0)
		Location.y = 0;
}

void Player::CheckPlayerHealth()
{
	if (Health <= 0)
	{
		// Prevent negative health values
		Health = 0;
		bIsDead = true;
		
		*GameState = DEATH;
	}
}

void Player::Draw() const
{
	// Player bullets
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
		if (Bullet[i].bActive)
			DrawTextureRec(BulletSprite, BulletFrameRec, Bullet[i].Location, WHITE); // Draw part of the bullet texture
   
	// Player sprite
    DrawTextureRec(Sprite, PlayerFrameRec, Location, WHITE);  // Draw part of the player texture

	if (bDebug)
		DrawRectangle(Hitbox.x, Hitbox.y, Hitbox.width, Hitbox.height, WHITE);
}
