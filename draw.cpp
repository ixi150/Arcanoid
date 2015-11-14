#include "Header.h"

// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
	SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i/*, j*/;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

//--------------------------------------------------------------------------------------//
void RefreshRenders(WindowPointers ArcanoidWindow){
	SDL_UpdateTexture(ArcanoidWindow.scrtex, NULL, ArcanoidWindow.screen->pixels, ArcanoidWindow.screen->pitch);
	SDL_RenderClear(ArcanoidWindow.renderer);
	SDL_RenderCopy(ArcanoidWindow.renderer, ArcanoidWindow.scrtex, NULL, NULL);
	SDL_RenderPresent(ArcanoidWindow.renderer);
}
//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//



///////////////////////////////////////////////////////////////////////////////////
//								FUNKCJE											 //
///////////////////////////////////////////////////////////////////////////////////

void DrawGame(GameInfo Info, GameObjects Objects){
	//SDL_FillRect(ArcanoidWindow.screen, NULL, COLOR.BLACK); 
	DrawRectangle(Info.ArcanoidWindow.screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Info.COLOR.RED, Info.COLOR.BLACK);
	static char text[128];
	
	DrawRectangle(Info.ArcanoidWindow.screen, 4, 4, SCREEN_WIDTH - 8, 36, Info.COLOR.RED, Info.COLOR.BLUE);
	sprintf(text, "You're playing Arcanoid for %.1lf seconds now. FPS: %.0lf", Info.Time.worldTime, Info.Time.fps);
	DrawString(Info.ArcanoidWindow.screen, Info.ArcanoidWindow.screen->w / 2 - strlen(text) * 8 / 2, 10, text, Info.ArcanoidWindow.charset);
	sprintf(text, "\032 / \033 - moving, S / SPACE - release the balls, F - shoot, ESCAPE - quit the game");
	DrawString(Info.ArcanoidWindow.screen, Info.ArcanoidWindow.screen->w / 2 - strlen(text) * 8 / 2, 26, text, Info.ArcanoidWindow.charset);

	DrawRectangle(Info.ArcanoidWindow.screen,
		WALLSX, WALLSY, WALLS_WIDTH, WALLS_HEIGHT, Info.COLOR.BLUE, Info.COLOR.BLACK);

	/*DRAWING GAME OBJECTS*/
	
	for (short i = 0; i < MAX_BONUSES; i++)
		if (Objects.Bonus[i].alive)
			DrawSurface(Info.ArcanoidWindow.screen, Objects.Bonus[i].pic, 
			Objects.Bonus[i].x + Objects.Bonus[i].w / 2, Objects.Bonus[i].y + Objects.Bonus[i].h / 2);
	for (short i = 0; i < MAX_BALLS; i++)
		if (Objects.Ball[i].alive)
			DrawSurface(Info.ArcanoidWindow.screen, Objects.Ball[i].pic, Objects.Ball[i].x, Objects.Ball[i].y);
	for (short i = 0; i < MAX_BULLETS; i++)
		if (Objects.Bullet[i].alive)
			DrawSurface(Info.ArcanoidWindow.screen, Objects.Bullet[i].pic, 
			Objects.Bullet[i].x + Objects.Bullet[i].w / 2, Objects.Bullet[i].y + Objects.Bullet[i].h / 2);
	for (short j = 0; j < MAX_BRICKS; j++)
	{
		if (Objects.Brick[j].alive)
			DrawRectangle(Info.ArcanoidWindow.screen,
				Objects.Brick[j].x, Objects.Brick[j].y, Objects.Brick[j].w, Objects.Brick[j].h,
				Info.COLOR.DARKGREY, Objects.Brick[j].color);
	}
	for (short j = 0; j < MAX_BLOCKS; j++)
	{
		if (Objects.Block[j].alive)
			DrawRectangle(Info.ArcanoidWindow.screen,
			Objects.Block[j].x, Objects.Block[j].y, Objects.Block[j].w, Objects.Block[j].h,
			Info.COLOR.DARKGREY, Objects.Block[j].color);
	}

	/*DRAWING PLAYER*/
	DrawRectangle(Info.ArcanoidWindow.screen,
		Objects.Player.x, Objects.Player.y, 
		Objects.Player.w, Objects.Player.h,
		Objects.Player.borderColor, Objects.Player.color);
	sprintf(text, "Player Score: %5d", Objects.Player.score);
	DrawString(Info.ArcanoidWindow.screen, 
		Objects.Player.x + Objects.Player.w/2 - strlen(text) * 4,
		Objects.Player.y + Objects.Player.h/2 - 4 -5,
		text, Info.ArcanoidWindow.charset);
	sprintf(text, "Stage:        %5d", Objects.Stage);
	DrawString(Info.ArcanoidWindow.screen,
		Objects.Player.x + Objects.Player.w / 2 - strlen(text) * 4,
		Objects.Player.y + Objects.Player.h / 2 - 4 +5,
		text, Info.ArcanoidWindow.charset);

	/*DRAWING HUD*/
	for (short i = 0; i < Objects.Lifes; i++)
	{
		DrawSurface(Info.ArcanoidWindow.screen, Info.Pics.ball, 10 + 20 * i - (i / 40) * 800, SCREEN_HEIGHT - 10 - (i / 40) * 20);
	}
	if (Objects.Player.ActiveBonus[GLUE] > 0) DrawSurface(Info.ArcanoidWindow.screen, Info.Pics.glue, SCREEN_WIDTH - 30, SCREEN_HEIGHT - 20);
	if (Objects.Player.ActiveBonus[SLOW] > 0) DrawSurface(Info.ArcanoidWindow.screen, Info.Pics.slow, SCREEN_WIDTH - 60, SCREEN_HEIGHT - 20);
	if (Objects.Player.ActiveBonus[SHOOTING] > 0) DrawSurface(Info.ArcanoidWindow.screen, Info.Pics.bullets, SCREEN_WIDTH - 90, SCREEN_HEIGHT - 20);
	
	//DrawCommunicate(0);
	RefreshRenders(Info.ArcanoidWindow);
}

//--------------------------------------------------------------------------------------//

void DrawHighScores(GameInfo Info, GameObjects Objects){
	DrawRectangle(Info.ArcanoidWindow.screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Info.COLOR.RED, Info.COLOR.BLACK);
	static char text[128];

	DrawSurface(Info.ArcanoidWindow.screen, Info.Pics.scores, Info.ArcanoidWindow.screen->w / 2, Info.ArcanoidWindow.screen->h / 4);
	
	for (int i = 0; i < 5; i++)
	{
		if (Objects.PositionInHighScores == i)
			DrawRectangle(Info.ArcanoidWindow.screen,
			Info.ArcanoidWindow.screen->w / 2 - 200,	250 + i * 20 - 5,
			400,										20,
			Info.COLOR.YELLOW, Info.COLOR.DARKBLUE);

		DrawString(Info.ArcanoidWindow.screen, 
			Info.ArcanoidWindow.screen->w / 2 - 150,
			250 + i * 20, 
			Objects.Record[i].name, Info.ArcanoidWindow.charset);

		sprintf(text, "%12d", Objects.Record[i].score);

		DrawString(Info.ArcanoidWindow.screen, 
			Info.ArcanoidWindow.screen->w / 2 +50,
			250 + i * 20, 
			text, Info.ArcanoidWindow.charset);
	}

	/*Komunikaty ponizej*/
	

	if (Objects.PositionInHighScores < 5)
	{
		DrawRectangle(Info.ArcanoidWindow.screen,
			Info.ArcanoidWindow.screen->w / 2 - 250, 500,
			500, 70,
			Info.COLOR.YELLOW, Info.COLOR.DARKGREEN);

		sprintf(text, "Congratulations! You got yourself into the High Scores!");
		DrawString(Info.ArcanoidWindow.screen,
			Info.ArcanoidWindow.screen->w / 2 - strlen(text) * 8 / 2, 510,
			text, Info.ArcanoidWindow.charset);

		sprintf(text, "Please write your name. Click ENTER to confirm and");
		DrawString(Info.ArcanoidWindow.screen,
			Info.ArcanoidWindow.screen->w / 2 - strlen(text) * 8 / 2, 530,
			text, Info.ArcanoidWindow.charset);

		sprintf(text, "play again! Click ESCAPE to shut down the game.");
		DrawString(Info.ArcanoidWindow.screen,
			Info.ArcanoidWindow.screen->w / 2 - strlen(text) * 8 / 2, 550,
			text, Info.ArcanoidWindow.charset);
	}
	else
	{
		DrawRectangle(Info.ArcanoidWindow.screen,
			Info.ArcanoidWindow.screen->w / 2 - 250, 500,
			500, 70,
			Info.COLOR.YELLOW, Info.COLOR.DARKRED);

		sprintf(text, "Your Score is: %5d", Objects.Player.score);
		DrawString(Info.ArcanoidWindow.screen,
			Info.ArcanoidWindow.screen->w / 2 - strlen(text) * 8 / 2, 520,
			text, Info.ArcanoidWindow.charset);

		sprintf(text, "You lost. Click ENTER to play again, ESCAPE to run away.");
		DrawString(Info.ArcanoidWindow.screen,
			Info.ArcanoidWindow.screen->w / 2 - strlen(text) * 8 / 2, 540,
			text, Info.ArcanoidWindow.charset);
	}

	RefreshRenders(Info.ArcanoidWindow);
}

