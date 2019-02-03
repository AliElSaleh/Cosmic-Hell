#include "Bomb.h"


Bomb::Bomb()
{
	Init();
}

void Bomb::Init()
{

}

void Bomb::Explode()
{
	for (unsigned short i = 0; i < Bullets->size(); i++)
		for (unsigned short j = 0; j < Bullets[0].size(); j++)
		{
			Bullets[0][0].bActive = false;
			Bullets[0][0].bIsHit = true;

			if (!Bullets[0][0].bActive)
				Bullets[0].erase(Bullets[0].begin()+0);
		}
}
