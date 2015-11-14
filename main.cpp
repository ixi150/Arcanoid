#include "Header.h"
#ifdef __cplusplus
extern "C"
#endif


int main(int argc, char **argv) {
	GameInfo Info; 
	SetUpWindow	(&Info.ArcanoidWindow);
	SetUpColors	(&Info);
	SetUpTime	(&Info.Time);
	SetUpPictures(&Info);

	GameObjects Objects;
	SetUpGameObjects(&Objects, Info);
	if (!LoadStage(Objects.Stage, &Objects, Info))
		return 0;

	srand(SDL_GetTicks());
	/////////////////////////////////////////// G£ÓWNA PÊTLA GRY ////////////////////////////////////////////////////////////////////////////
	while (Objects.quit == false)
	{
		UpdateTime(&Info.Time);
		MoveGameObjects(&Objects, Info.Time.delta);
		DrawGame(Info, Objects);
		WinLose(&Objects, Info);
		GameControls(&Objects);

		if (Objects.Lifes <= 0)
			HighScoreScreen(&Objects, Info);
		if (Objects.Restart)
			RestartGame(&Objects, Info);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// zwolnienie powierzchni
	DestroyScreen(Info.ArcanoidWindow);
	return 0;
	};