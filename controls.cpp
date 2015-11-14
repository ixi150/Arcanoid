#include "Header.h"

//--------------------------------------------------------------------------------------//


void GameControls(GameObjects* Objects){
	static SDL_Event event;

	// obs³uga zdarzeñ (o ile jakieœ zasz³y)
	while (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){

				/*PROGRAMISTYCZNE*/
//#define SPEED_UP 0.6f
//			case SDLK_w:
//				Objects->Ball[0].speed += SPEED_UP;
//				break;
//			case SDLK_s:
//				Objects->Ball[0].speed -= SPEED_UP;
//				break;
//			case SDLK_a:
//				Objects->Ball[0].angle -= 10;
//				break;
//			case SDLK_d:
//				Objects->Ball[0].angle += 10;
//				break;
//
//				/*CHEATS*/
//			case SDLK_KP_0:
//				Objects->Ball[0].x = Objects->Ball[0].y = SCREEN_HEIGHT / 2;
//				break;
//			case SDLK_r:
//				Objects->Ball[0].speed = Objects->Ball[0].angle = 0;
//				break;
//			case SDLK_KP_1:
//				Objects->Ball[0].speed /= 2;
//				break;
//			case SDLK_KP_2:
//				Objects->Ball[0].speed *= 2;
//				break;
//			case SDLK_KP_4:
//				Objects->Lifes++;
//				break;
//			case SDLK_KP_5:
//				Objects->Lifes--;
//				break;
//			case SDLK_KP_7:
//				for (short i = 0; i < MAX_BRICKS; i++){
//					Objects->Brick[i].Destroy();
//				}
//				break;
//			case SDLK_KP_3: //roztrojenie
//				//Objects->Ball[0].reset();
//				for (short i = 1; i < MAX_BALLS; i++)
//				{
//					Objects->Ball[i].alive = true;
//					Objects->Ball[i].angle = (rand() % 360);
//					Objects->Ball[i].x = Objects->Ball[0].x;
//					Objects->Ball[i].y = Objects->Ball[0].y;
//					Objects->Ball[i].speed = Objects->Ball[0].speed;
//				}
//				break;
//



				/*Do gry arcanoid*/
			case SDLK_LEFT:
				Objects->Player.force_x2 = PLAYER_SPEED;
				break;
			case SDLK_RIGHT:
				Objects->Player.force_x1 = PLAYER_SPEED;
				break;

			//case SDLK_s:
			case SDLK_SPACE: //case SDLK_s:
				if (Objects->Lifes > 0)
				{
					for (short i = 0; i < MAX_BALLS; i++)
					{
						if (Objects->Ball[i].glued)
						{
							Objects->Ball[i].glued = false; //poŸniej ze S i ¿e tylko odkleja WSZYSTKIE

							Objects->Ball[i].y -= 1;
						}
					}
				}

				break;

			case SDLK_f:
				for (int i = 0; i < MAX_BULLETS; i++)
				{
					if (Objects->Player.Reloading <= 0 && Objects->Bullet[i].alive == false && Objects->Player.ActiveBonus[SHOOTING] > 0.0)
					{
						Objects->Bullet[i].alive = true;
						Objects->Bullet[i].x = Objects->Player.x + Objects->Player.w / 2;
						Objects->Bullet[i].y = Objects->Player.y - Objects->Bullet[i].h;
						Objects->Bullet[i].angle = 0;
						Objects->Player.Reloading = .5;
						break;
					}
				}
				break;

			case SDLK_ESCAPE:
				Objects->quit = true;
				break;
			}

			break;


		case SDL_KEYUP:
			switch (event.key.keysym.sym){
			case SDLK_LEFT:
				Objects->Player.force_x2 = 0;
				break;
			case SDLK_RIGHT:
				Objects->Player.force_x1 = 0;
				break;
			}
			break;
	


		case SDL_QUIT:
			Objects->quit = true;
			break;
		};
	}
}





void HighScoresControls(GameObjects* Objects){
	static SDL_Event event;
	static int CursorPosition = 0;

	// obs³uga zdarzeñ (o ile jakieœ zasz³y)
	while (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){

			case SDLK_ESCAPE:
				Objects->quit = true;
				break;
				
			case SDLK_RETURN:
				if (Objects->Record[Objects->PositionInHighScores].name[0] != 0)
				{
					Objects->Lifes = 3;
					Objects->Restart = true;
				}
				break;

			default:
				if (Objects->PositionInHighScores < 5)
				{
					if ((event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z) ||
						(event.key.keysym.sym == SDLK_SPACE && CursorPosition > 0))
					{
						Objects->Record[Objects->PositionInHighScores].name[CursorPosition] = event.key.keysym.sym;
						if (event.key.keysym.sym != SDLK_SPACE)
							Objects->Record[Objects->PositionInHighScores].name[CursorPosition] += ('A' - 'a');

						if (CursorPosition < 17)
							CursorPosition++;
					}

					if (event.key.keysym.sym == SDLK_BACKSPACE)
					{
						if (CursorPosition > 0)
							CursorPosition--;

						Objects->Record[Objects->PositionInHighScores].name[CursorPosition] = 0;
					}
				}
			}

			break;
		



		case SDL_KEYUP:
			switch (event.key.keysym.sym){
			case SDLK_LEFT:
				Objects->Player.force_x2 = 0;
				break;
			case SDLK_RIGHT:
				Objects->Player.force_x1 = 0;
				break;
			}
			break;



		case SDL_QUIT:
			Objects->quit = true;
			break;
		};
	}
}