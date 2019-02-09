#include "Globals.h"
#include "Player.h"
#include "Bullet.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

void Player::Init()
{
	Sprite = GetAsset(Player);

	Heart.clear();
	Bomb.clear();

	for (int i = 0; i < 6; i++)
		Heart.emplace_back(GetAsset(Heart));
	
	for (int i = 0; i < 2; i++)
		Bomb.emplace_back(GetAsset(Bomb));

	XOffset = 15;
	YOffset = 50;
	Location.x = float(GetScreenWidth()-PANEL_WIDTH) / 2 + float(XOffset);
	Location.y = float(GetScreenHeight()) - 100;
	BulletSpawnLocation.x = Location.x + float(Sprite.width)/4 - XOffset;
	BulletSpawnLocation.y = Location.y;
	BulletXOffset = 5;
	Hitbox.x = Location.x + float(Sprite.width)/4 + float(XOffset);
	Hitbox.y = Location.y + float(Sprite.height)/4;
	Spritebox = {Location.x, Location.y, float(Sprite.width)/4 - 20, float(Sprite.height)};
	Hitbox.width = 3;
	Hitbox.height = 3;
	Health = 6;
	BombsLeft = unsigned short(Bomb.size());
	Score = 0;
	GrazingScore = 0;
	Name = "Scarlet";
	
	PlayerFrameRec.x = 0.0f;
	PlayerFrameRec.y = 0.0f;
	PlayerFrameRec.width = float(Sprite.width)/4; // 4 frames
	PlayerFrameRec.height = float(Sprite.height);

	Center = {Location.x - float(Sprite.width)/12, Location.y + float(Sprite.height)/2};


	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		Bullet[i].Sprite = GetAsset(BlueBullet);
		Bullet[i].Frames = 4;
		Bullet[i].Location = Location;
		Bullet[i].Radius = 3.0f;
		Bullet[i].Speed = 500.0f;
		Bullet[i].Damage = GetRandomValue(20, 40);
		Bullet[i].bActive = false;

		Bullet[i].InitFrames();
	}

	BulletLevel = 1;
	BossKilled = 0;
	EnemiesKilled = 0;
	BulletDamage = GetRandomValue(10, 15);

	BombRegenTimer = 0;
	BombCooldownTimer = 0;

	bWasBombUsed = false;
	bCanUseBomb = true;
	bChangeMusic = false;
	bInvincible = false;
	bIsDead = false;
	bIsHit = false;
	bDebug = false;
}

void Player::Update()
{
	// Update location and animation if not dead
	if (!bIsDead)
	{
		PlayerSpriteFramesCounter++;
		BombRegenTimer++;

		// Update the player's location and its components
		Location.x = GetMousePosition().x - XOffset - 2;
		Location.y = GetMousePosition().y - YOffset;

		Hitbox.x = Location.x + float(Sprite.width)/4 - 45;
		Hitbox.y = Location.y + float(Sprite.height)/4 + 22;

		Spritebox = {Location.x, Location.y, float(Sprite.width)/4 - 20, float(Sprite.height)};

		Center = {Location.x + float(Sprite.width)/12 - 2.0f, Location.y + float(Sprite.height)/2 - 5.0f};

		BulletSpawnLocation.x = Location.x + float(Sprite.width)/4 - XOffset - 3;
		BulletSpawnLocation.y = Location.y;

		// Player's invincibility mechanic
		if (bIsHit)
		{
			PlayerHitFramesCounter++;
			Invincibility(true, 1.0f);
		}
		else
		{
			PlayerHitFramesCounter = 0;
			Invincibility(false, 0.0f);
		}

		// Bomb mechanic
		if (bCanUseBomb)
		{
			if (IsKeyPressed(KEY_B))
			{
				bWasBombUsed = true;
				bCanUseBomb = false;
				
				if (BombsLeft < 0)
					BombsLeft = -1;
				else
					BombsLeft--;

				if (!Bomb.empty())
					Bomb.pop_back();
			}
		}
		else
		{
			BombCooldownTimer++;
			bWasBombUsed = false;
			
			if (BombCooldownTimer/360%2)
			{
				BombCooldownTimer = 0;
				bWasBombUsed = false;
				bCanUseBomb = true;
			}
		}

		if (BombRegenTimer/2400%2)
		{
			if (Bomb.size() < 4)
				Bomb.emplace_back(GetAsset(Bomb));

			BombsLeft++;

			BombRegenTimer = 0;
		}

		// To prevent negative values
		if (Score < 0)
			Score = 0;

		UpdatePlayerAnimation();
	}

	// Player checks
	CheckCollisionWithWindow();
	CheckHealth();

	// Update bullets movement on key press [space]
	UpdateBullet();
	CheckBulletOutsideWindow();
	CheckBulletLevel();
}

void Player::Draw() const
{   
	// Player sprite
	if (bIsHit)
	{
		if (PlayerHitFramesCounter/10%2)
			DrawTextureRec(Sprite, PlayerFrameRec, Location, WHITE);  // Draw part of the player texture
	}
	else
		DrawTextureRec(Sprite, PlayerFrameRec, Location, WHITE);  // Draw part of the player texture

	if (bDebug)
	{
		DrawRectangleLines(int(Spritebox.x), int(Spritebox.y), int(Spritebox.width), int(Spritebox.height), WHITE); // Spritebox
		DrawRectangle(int(Hitbox.x), int(Hitbox.y), int(Hitbox.width), int(Hitbox.height), YELLOW); // Player hitbox
	
		for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
			DrawCircle(int(Bullet[i].Center.x), int(Bullet[i].Center.y), Bullet[i].Radius, RED); // Player Bullets hitbox
	
		DrawCircle(int(Center.x), int(Center.y), 3.0f, YELLOW);
	}
}

void Player::DrawBullets() const
{
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
		Bullet[i].Draw();
}

void Player::ResetBullet(const short Index)
{
	Bullet[Index].bActive = false;		
	Bullet[Index].bIsHit = true;
	Bullet[Index].Location = Location;
	Bullet[Index].Center = Bullet[Index].Location;
	ShootRate = 0;
}

void Player::InitBulletLevel(const signed short Level)
{
	switch (Level)
	{
		case 1:
			ShootRate += 2;

			// Center of gun
			for (int i = 0; i < MAX_PLAYER_BULLETS/2; i++)
			{
				if (!Bullet[i].bActive && ShootRate % 20 == 0)
				{
					Bullet[i].Location = BulletSpawnLocation;
					Bullet[i].Damage = GetRandomValue(20, 40);
					Bullet[i].bActive = true;
					break;
				}
			}
		break;

		case 2:
			ShootRate += 3;

			// Left of gun
			for (int i = 0; i < MAX_PLAYER_BULLETS/2; i++)
			{
				if (!Bullet[i].bActive && ShootRate % 24 == 0)
				{
					Bullet[i].Location.x = BulletSpawnLocation.x - BulletXOffset; // Offset to the left by 5 pixels to make room for other half of bullets
					Bullet[i].Location.y = BulletSpawnLocation.y;
					Bullet[i].Speed = 700.0f;
					Bullet[i].Damage = GetRandomValue(20, 40);
					Bullet[i].bActive = true;
					break;
				}
			}

			// Right of gun
			for (int i = 25; i < MAX_PLAYER_BULLETS; i++)
			{
				if (!Bullet[i].bActive && ShootRate % 24 == 0)
				{
					Bullet[i].Location.x = BulletSpawnLocation.x + BulletXOffset; // Offset to the right by 5 pixels to make room for other half of bullets
					Bullet[i].Location.y = BulletSpawnLocation.y;
					Bullet[i].Speed = 700.0f;
					Bullet[i].Damage = GetRandomValue(20, 40);
					Bullet[i].bActive = true;
					break;
				}
			}
		break;

		case 3:
		break;

		default:
		break;
	}
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

void Player::UpdateBullet()
{
	// Initialise bullets
	if (IsKeyDown(KEY_SPACE))
	{
		if (!bIsDead)
		{
			switch (BulletLevel)
			{
				case 1:
					InitBulletLevel(1);
				break;

				case 2:
					InitBulletLevel(2);
				break;

				case 3:
					InitBulletLevel(3);
				break;

				default:
					InitBulletLevel(1);
				break;
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

			// Move the center location with bullet movement
			Bullet[i].Center.x = Bullet[i].Location.x + float(Bullet[i].Sprite.width)/8;
			Bullet[i].Center.y = Bullet[i].Location.y + float(Bullet[i].Sprite.height);

			Bullet[i].UpdateAnimation();
		}
		else
			Bullet[i].Location = Location;
	}
}

void Player::CheckBulletOutsideWindow()
{
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
		if (Bullet[i].Location.y + Bullet[i].Sprite.height < 0)
			ResetBullet(i);
}

void Player::CheckCollisionWithWindow()
{
	if (Location.x + float(Sprite.width)/4 > GetScreenWidth()-PANEL_WIDTH)
	{
		Location.x = GetScreenWidth()-PANEL_WIDTH - float(Sprite.width)/4;
		Hitbox.x = GetScreenWidth()-PANEL_WIDTH -  float(Sprite.width)/6 - 3.0f;
		BulletSpawnLocation.x = GetScreenWidth()-PANEL_WIDTH - float(Sprite.width)/6 + 23.0f;
	}
	
	if (Location.y + float(Sprite.height)/2 > GetScreenHeight())
	{
		Location.y = float(GetScreenHeight()) - float(Sprite.height)/2;
		Hitbox.y =  float(GetScreenHeight() - 7.0f);
		BulletSpawnLocation.y = float(GetScreenHeight()) - float(Sprite.height)/2;
	}

	if (Location.x < 0.0f)
	{
		Location.x = 0.0f;
		Hitbox.x = float(Sprite.width)/12 - 3.0f;
		BulletSpawnLocation.x = float(Sprite.width)/6 + 3.0f;
	}
	
	if (Location.y < 0.0f)
	{
		Location.y = 0.0f;
		Hitbox.y =  float(Sprite.height)/2 - 7.0f;
	}
}

void Player::CheckHealth()
{
	if (Health <= 0)
	{
		// Prevent negative health values
		Health = 0;
		Heart.clear();
		bIsDead = true;
		
		*GameState = DEATH;
	}
}

void Player::CheckBulletLevel()
{
	if (BossKilled > 0)
	{
		BulletLevel = 2;
	}
}

void Player::Invincibility(const bool Mode, const float Seconds)
{
	FramesCounter++;

	if (Mode && Seconds > 0.0f)
	{
		bInvincible = true;

		if (fmodf(FramesCounter/(GetFPS()*Seconds), 2) == 1)
		{
			FramesCounter = 0;

			bInvincible = false;
			bIsHit = false;
		}
	}
	else if (Mode && Seconds <= 0.0f)
	{
		bInvincible = true;
		Health = 6;
	}

	else if (!Mode && Seconds > 0.0f)
	{
		bInvincible = false;
		
		if (fmodf(FramesCounter/(GetFPS()*Seconds), 2) == 1)
		{
			FramesCounter = 0;

			bInvincible = true;
			bIsHit = true;
		}
	}
	else if (!Mode && Seconds <= 0.0f)
		bInvincible = false;

	if (FramesCounter > 120*Seconds)
		FramesCounter = 0;
}
