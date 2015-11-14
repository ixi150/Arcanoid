#include "Header.h"
//--------------------------------------------------------------------------------------//

void SetUpGameObjects(GameObjects* Objects, GameInfo Info){
	
	Objects->quit = Objects->Restart = false;
	Objects->Lifes = 3;
	Objects->Stage = 1;

	/*Set Up Player*/
	Objects->Player.color = Info.COLOR.PURPLE;
	Objects->Player.borderColor = Info.COLOR.ORANGE;

	/*Set Up Balls*/
	
	Objects->Ball[0].x = Objects->Player.x + Objects->Player.w / 2;
	Objects->Ball[0].y = Objects->Player.y - Objects->Ball[0].r;
	Objects->Ball[0].alive = Objects->Ball[0].glued = true;
	
	/*Set Up Bricks*/
	for (short i = 0; i < MAX_BRICKS; i++)
	{
		Objects->Brick[i].x = WALLSX + (i % BRICKS_IN_WIDE)*(Objects->Brick[i].w);
		Objects->Brick[i].y = WALLSY + (i / BRICKS_IN_WIDE)*Objects->Brick[i].h;
		Objects->Brick[i].pBonuses = Objects->Bonus;
		Objects->Brick[i].pPlayerScore = &Objects->Player.score;
	}
	
	/*Set Up Bonuses*/
	for (short i = 0; i < MAX_BONUSES; i++)
	{
		Objects->Bonus[i].pPlayerScore = &Objects->Player.score;
		Objects->Bonus[i].pActiveBonus = Objects->Player.ActiveBonus;
	}

	/*Set up Blocks*/
	for (short i = 0; i < MAX_BLOCKS; i++)
	{
		Objects->Block[i].color = Info.COLOR.WHITE;
	}
}

//--------------------------------------------------------------------------------------//

void UpdateTime(TimeInfo* Time){
	Time->t2 = SDL_GetTicks();
	Time->delta = (Time->t2 - Time->t1) * 0.001;
	Time->t1 = Time->t2;
	Time->worldTime += Time->delta;
	Time->fpsTimer += Time->delta;
	if (Time->fpsTimer > FPS_REFRESH_RATE)
	{
		Time->fps = Time->frames / FPS_REFRESH_RATE;
		Time->frames = 0;
		Time->fpsTimer -= FPS_REFRESH_RATE;
	}
	Time->frames++;
}

//--------------------------------------------------------------------------------------//

bool NoBalls(GameBall* Ball){
	for (short i = 0; i < MAX_BALLS; i++)
	{
		if (Ball[i].alive)
		return false;
	}
	return true;
}
bool NoBricks(GameBrick* Brick){
	for (short i = 0; i < MAX_BRICKS; i++)
	{
		if (Brick[i].alive && Brick[i].destroyable)
			return false;
	}
	return true;
}
void ClearStage(GameObjects* Objects){
	for (short i = 0; i < MAX_BONUSES; i++)
		Objects->Bonus[i].Destroy();
	for (short i = 0; i < MAX_BULLETS; i++)
		Objects->Bullet[i].Destroy();
	for (short i = 0; i < MAX_BLOCKS; i++)
		Objects->Block[i].Destroy();
	for (short i = 0; i < BONUSES; i++)
		Objects->Player.ActiveBonus[i] = 0;
	
	Objects->Player.Reset();
	Objects->Ball[0].x = Objects->Player.x + Objects->Player.w / 2;
	Objects->Ball[0].y = Objects->Player.y - Objects->Ball[0].r;
	Objects->Ball[0].alive = true;
	Objects->Ball[0].glued = true;
}

void WinLose(GameObjects* Objects, GameInfo Info){
	if (NoBalls(Objects->Ball))
	{
		ClearStage(Objects);
		Objects->Lifes--;
	}
	if (NoBricks(Objects->Brick))
	{
		for (short i = 0; i < MAX_BALLS; i++)
			Objects->Ball[i].Destroy();

		Objects->Player.score += 1000 * Objects->Stage;
		
		ClearStage(Objects);
		if (!LoadStage(++Objects->Stage, Objects, Info))
			Objects->Lifes=0;
	}
	/////////////////////////////////////////////////////////////////////////

	/*Prze³adowywanie broni*/
	if (Objects->Player.Reloading > 0)
		Objects->Player.Reloading -= Info.Time.delta;

	/*Zmniejszanie czasu trwania bonusów*/
	for (short i = 0; i < BONUSES; i++)
	{
		if (Objects->Player.ActiveBonus[i] > 0)
			Objects->Player.ActiveBonus[i] -= Info.Time.delta;
	}

	/*Obs³uga bonusu spowolnienia*/
	if (Objects->Player.ActiveBonus[SLOW] < 0.0f && Objects->Player.ActiveBonus[SLOW] > -1.0f)
	{
		Objects->Player.ActiveBonus[SLOW] = -2.0f;
		for (short i = 0; i < MAX_BALLS; i++)
		{
			Objects->Ball[i].speed = START_SPEED;
		}
	}
	/*Zmiana koloru paletki gdy klej¹ca*/
	if (Objects->Player.ActiveBonus[GLUE] > 0 && Objects->Player.color != Info.COLOR.ORANGE)
		Objects->Player.color = Info.COLOR.ORANGE;
	else if (Objects->Player.ActiveBonus[GLUE] <= 0 && Objects->Player.color != Info.COLOR.PURPLE)
		Objects->Player.color = Info.COLOR.PURPLE;

	/*Dodanie ¿ycia*/
	if (Objects->Player.ActiveBonus[LIFE] > 0)
	{
		Objects->Lifes++;
		Objects->Player.ActiveBonus[LIFE] = 0;
	}

	/*Animacje wyd³u¿ania i skracania paletki*/
	if (Objects->Player.ActiveBonus[WIDE] > 0 && Objects->Player.w < WIDE_BONUS)
	{
		Objects->Player.w += WIDE_BONUS_SPEED * Info.Time.delta;
		Objects->Player.x -= WIDE_BONUS_SPEED/2 * Info.Time.delta;
	}
	if (Objects->Player.ActiveBonus[WIDE] <= 0 && Objects->Player.w > PLAYER_WIDTH)
	{
		Objects->Player.w -= WIDE_BONUS_SPEED * Info.Time.delta;
		Objects->Player.x += WIDE_BONUS_SPEED / 2 * Info.Time.delta;
	}
	/*Roztrojenie pileczek*/
	if (Objects->Player.ActiveBonus[THREE] > 0)
	{
		Objects->TrippleBalls();
		Objects->Player.ActiveBonus[THREE] = 0;
	}
	/*Zmiana kolorów obudowania paletki gracza*/
	if (Objects->Player.ActiveBonus[SHOOTING] > 0.0 && Objects->Player.borderColor != Info.COLOR.RED)
		Objects->Player.borderColor = Info.COLOR.RED;
	if (Objects->Player.ActiveBonus[SHOOTING] <= 0.0 && Objects->Player.borderColor != Info.COLOR.ORANGE)
		Objects->Player.borderColor = Info.COLOR.ORANGE;

	//////////////////////////////// BLOCKI /////////////////////////////////////////////

	/*Pojawianie siê bloków*/
	static double Timer = 0;
	Timer += Info.Time.delta;
	static double MaxTime = (rand() % 500)/100.0;
	if (Timer > MaxTime)
	{
		Timer = 0.0f;
		MaxTime = (rand() % 1000) / 100.0 +1;
		
		for (short i = 0; i < MAX_BLOCKS; i++)
		{
			if (!Objects->Block[i].alive)
			{
				Objects->Block[i].alive = true;
				Objects->Block[i].x = WALLSX+20;
				Objects->Block[i].y = WALLSY + WALLS_HEIGHT/2;

				if (Objects->Block[i].CollidesWith(Objects->Block))
					Objects->Block[i].Destroy();

				return;
			}
		}
	}
}

//--------------------------------------------------------------------------------------//

void RestartGame(GameObjects* Objects, GameInfo Info){
	Objects->Restart = false;
	Objects->Player.score = 0;
	Objects->Player.Reset();
	ClearStage(Objects);
	Objects->Stage = 1;
	LoadStage(Objects->Stage, Objects, Info);
}
//--------------------------------------------------------------------------------------//
