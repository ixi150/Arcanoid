#include "Header.h"

///////////////////////////////////////////////////////////////////////////////////
//								KONSTRUKTORY									 //
///////////////////////////////////////////////////////////////////////////////////

GameObject::GameObject(){
	x = y = r = angle = speed = 0.0f;
	w = h = 0;
	alive = false;
	score = color = 0;
	pic = NULL;
}
GamePlayer::GamePlayer() :GameObject(){
	alive = true;
	w = PLAYER_WIDTH;
	h = PLAYER_HEIGHT;
	x = SCREEN_WIDTH / 2 - w / 2;
	y = WALLSY + WALLS_HEIGHT - h * 1.3;
	force_x1 = force_x2 = Reloading = 0;
	for (short i = 0; i < BONUSES; i++)
		ActiveBonus[i] = 0.0f;
}
GameBall::GameBall() :GameObject(){
	w = h = 16;
	r = w / 2;
	angle = (rand() % 140) - 70.0;
	speed = START_SPEED;
	glued = false;
	newAngle = 0.0f;
	pic = SDL_LoadBMP("./pics/ball.bmp");;
}
GameBrick::GameBrick() :GameObject(){
	w = WALLS_WIDTH / BRICKS_IN_WIDE;
	h = WALLS_HEIGHT / BRICKS_IN_TALL / 2;
	destroyable = true;
	/*pPlayerScore = pScore;
	pBonuses = pBonus;*/
}
GameBullet::GameBullet() : GameObject(){
	w = 6;
	h = 40;
	speed = BULLET_SPEED;
	pic = SDL_LoadBMP("./pics/bullet.bmp");
}
GameBlock::GameBlock() :GameObject(){
	w = WALLS_WIDTH / BRICKS_IN_WIDE / 2;
	h = WALLS_HEIGHT / BRICKS_IN_TALL / 4;
	angle = (rand() % 360);
	speed = START_SPEED;
}
GameBonus::GameBonus() : GameObject(){
	speed = START_SPEED;
}





void GamePlayer::Reset(){
	x = SCREEN_WIDTH / 2 - w/2;
	y = WALLSY + WALLS_HEIGHT - h * 1.3;
	force_x1 = force_x2 = Reloading = 0;
	for (short i = 0; i < BONUSES; i++)
		ActiveBonus[i] = 0.0f;
}
///////////////////////////////////////////////////////////////////////////////////
//								DESTROY											 //
///////////////////////////////////////////////////////////////////////////////////

void GameObject::Destroy(){
	alive = false;
}
void GameBrick::Destroy(){
	if (destroyable)
	{
		alive = false;
		*pPlayerScore += score;
		SpawnBonus();
	}
}
void GameBonus::Gather(){
	*pPlayerScore += score;
	*(pActiveBonus + effect) = duration;
}


///////////////////////////////////////////////////////////////////////////////////
//								BONUS EFFECTS									 //
///////////////////////////////////////////////////////////////////////////////////

void GameObjects::TrippleBalls(){
	short counter = 0;
	for (short i = 0; i < MAX_BALLS; i++) //szuka pierwszej ¿yj¹cej pi³ki aby j¹ roztroiæ
	{
		if (Ball[i].alive)
		{
			for (short j = 0; j < MAX_BALLS; j++) //pi³eczka w jedn¹ stronê
			{
				if (Ball[j].alive == false)
				{
					Ball[j].alive = true;
					Ball[j].x = Ball[i].x;
					Ball[j].y = Ball[i].y;
					Ball[j].speed = Ball[i].speed;
					Ball[j].angle = Ball[i].angle + 10;
					break;
				}
			}
			for (short j = 0; j < MAX_BALLS; j++) //pi³eczka w drug¹ stronê
			{
				if (Ball[j].alive == false)
				{
					Ball[j].alive = true;
					Ball[j].x = Ball[i].x;
					Ball[j].y = Ball[i].y;
					Ball[j].speed = Ball[i].speed;
					Ball[j].angle = Ball[i].angle - 10;
					break;
				}
			}
			return;
		}
	}
}

void GameBrick::SpawnBonus(){
	GameBonus* pBonus = pBonuses;
	for (short i = 0; i < MAX_BONUSES; i++, pBonus++)
	{
		if (pBonus->alive == false)
		{
			/*Czy nagroda siê pojawi*/
			int Luck = rand() % 100;
			if (Luck < BONUS_CHANCE)
			{
				pBonus->alive = true;
				pBonus->x = x;
				pBonus->y = y+10;
				pBonus->angle = 180;
				
				Luck = rand() % (BONUS_SLOW_CHANCE + BONUS_GLUE_CHANCE + BONUS_LIFE_CHANCE + BONUS_WIDE_CHANCE + BONUS_THREE_CHANCE + BONUS_SHOOTING_CHANCE);
				
				/*GLUE*/
				if (Luck < BONUS_GLUE_CHANCE)
				{
					pBonus->effect = GLUE;
					pBonus->pic = SDL_LoadBMP("./pics/glue.bmp");
					pBonus->w = 16;
					pBonus->h = 30;
					pBonus->score = 100;
					pBonus->duration = 5;
					return;
				}
				else
					Luck -= BONUS_GLUE_CHANCE;

				/*SLOW*/
				if (Luck < BONUS_SLOW_CHANCE)
				{
					pBonus->effect = SLOW;
					pBonus->pic = SDL_LoadBMP("./pics/slow.bmp");
					pBonus->w = 21;
					pBonus->h = 32;
					pBonus->score = 25;
					pBonus->duration = 20;
					return;
				}
				else
					Luck -= BONUS_SLOW_CHANCE;

				/*THREE BALLS*/
				if (Luck < BONUS_THREE_CHANCE)
				{
					pBonus->effect = THREE;
					pBonus->pic = SDL_LoadBMP("./pics/three.bmp");
					pBonus->w = 30;
					pBonus->h = 25;
					pBonus->score = 150;
					pBonus->duration = 1;
					return;
				}
				else
					Luck -= BONUS_THREE_CHANCE;

				/*WIDE PLAYER*/
				if (Luck < BONUS_WIDE_CHANCE)
				{
					pBonus->effect = WIDE;
					pBonus->pic = SDL_LoadBMP("./pics/wide.bmp");
					pBonus->w = 44;
					pBonus->h = 18;
					pBonus->score = 75;
					pBonus->duration = 20;
					return;
				}
				else
					Luck -= BONUS_WIDE_CHANCE;

				/*LIFE*/
				if (Luck < BONUS_LIFE_CHANCE)
				{
					pBonus->effect = LIFE;
					pBonus->pic = SDL_LoadBMP("./pics/life.bmp");
					pBonus->w = 37;
					pBonus->h = 32;
					pBonus->score = 50;
					pBonus->duration = 1;
					return;
				}
				else
					Luck -= BONUS_LIFE_CHANCE;

				/*SHOOTING*/
				//if (Luck < BONUS_SHOOTING_CHANCE)
				{
					pBonus->effect = SHOOTING;
					pBonus->pic = SDL_LoadBMP("./pics/bullets.bmp");
					pBonus->w = 43;
					pBonus->h = 37;
					pBonus->score = 5;
					pBonus->duration = 20;
					return;
				}
			}
			return;
		}
	}
}
