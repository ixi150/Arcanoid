#include "Header.h"

//--------------------------------------------------------------------------------------//

void SetUpWindow(WindowPointers* SomeWindow){
	SDL_SetWindowTitle(SomeWindow->window, "Arcanoid the Game");

	SomeWindow->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) 
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	// tryb pe³noekranowy
	if (SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &SomeWindow->window, &SomeWindow->renderer) != 0) {
	//if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &SomeWindow->window, &SomeWindow->renderer) != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(SomeWindow->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(SomeWindow->renderer, 0, 0, 0, 255);

	


	SomeWindow->scrtex = SDL_CreateTexture(SomeWindow->renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	SomeWindow->charset = SDL_LoadBMP("./pics/fonts.bmp");
	if (SomeWindow->charset == NULL) {
		printf("SDL_LoadBMP(fonts.bmp) error: %s\n", SDL_GetError());
		DestroyScreen(*SomeWindow);
	};


	SDL_SetColorKey(SomeWindow->charset, true, 0x000000);

};

//--------------------------------------------------------------------------------------//

void DestroyScreen(WindowPointers SomeWindow){
	SDL_FreeSurface(SomeWindow.screen);
	SDL_DestroyTexture(SomeWindow.scrtex);
	SDL_DestroyWindow(SomeWindow.window);
	SDL_DestroyRenderer(SomeWindow.renderer);
	SDL_Quit();
	exit(EXIT_FAILURE);
};

//--------------------------------------------------------------------------------------//

void SetUpColors(GameInfo* Info){
	Info->COLOR.WHITE = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0xFF, 0xFF, 0xFF);
	Info->COLOR.LIGHTGREY = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0xB8, 0xB8, 0xB8);
	Info->COLOR.GREY = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x7A, 0x7A, 0x7A);
	Info->COLOR.DARKGREY = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x3B, 0x3B, 0x3B);
	Info->COLOR.BLACK = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x00, 0x00, 0x00);

	Info->COLOR.LIGHTGREEN = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x52, 0xFF, 0x52);
	Info->COLOR.GREEN = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x00, 0xD1, 0x00);
	Info->COLOR.DARKGREEN = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x00, 0x6B, 0x36);

	Info->COLOR.LIGHTRED = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0xFF, 0xE6, 0x61);
	Info->COLOR.RED = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0xE6, 0x00, 0x00);
	Info->COLOR.DARKRED = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x6B, 0x00, 0x00);

	Info->COLOR.LIGHTBLUE = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x00, 0xE6, 0xE6);
	Info->COLOR.BLUE = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x00, 0x73, 0xE6);
	Info->COLOR.DARKBLUE = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x00, 0x00, 0x70);

	Info->COLOR.PURPLE = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x66, 0x33, 0xFF);
	Info->COLOR.YELLOW = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0xFF, 0xFF, 0x29);
	Info->COLOR.ORANGE = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0xFF, 0x94, 0x29);
	Info->COLOR.BROWN = SDL_MapRGB(Info->ArcanoidWindow.screen->format, 0x8F, 0x47, 0x00);
};

//--------------------------------------------------------------------------------------//

void SetUpPictures(GameInfo* Info){
	Info->Pics.ball = SDL_LoadBMP("./pics/ball.bmp");
	Info->Pics.glue = SDL_LoadBMP("./pics/glue.bmp");
	Info->Pics.slow = SDL_LoadBMP("./pics/slow.bmp");
	Info->Pics.wide = SDL_LoadBMP("./pics/wide.bmp");
	Info->Pics.bullet = SDL_LoadBMP("./pics/bullet.bmp");
	Info->Pics.bullets = SDL_LoadBMP("./pics/bullets.bmp");
	Info->Pics.scores = SDL_LoadBMP("./pics/scores.bmp");

	if (Info->Pics.ball == NULL ||
		Info->Pics.glue == NULL ||
		Info->Pics.slow == NULL ||
		Info->Pics.wide == NULL ||
		Info->Pics.bullet == NULL ||
		Info->Pics.bullets == NULL ||
		Info->Pics.scores == NULL)
	{
		printf("Pictures load error: %s\n", SDL_GetError());
		DestroyScreen(Info->ArcanoidWindow);
	}
}

//--------------------------------------------------------------------------------------//

void SetUpTime(TimeInfo* Time){
	Time->t1 = SDL_GetTicks();
	Time->frames	= 0;
	Time->fpsTimer	= 0;
	Time->fps		= 0;
	Time->worldTime = 0;
}

//--------------------------------------------------------------------------------------//
