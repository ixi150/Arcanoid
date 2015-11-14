#include "Header.h"
#include<stdio.h>
using namespace std;

//----------------------------------------------------------------------------------------------------------------------------

int Length(char* word)
{
	int Length = 0;
	while (word[Length++] != 0);
	return Length;
}

void Rewrite(char* to, char* from){
	int Lengh = Length(from);
	for (int i = 0; i <= Lengh; i++)
	{
		to[i] = from[i];
	}
}
bool FindStage(int lvl, FILE* Stages){
	
	for (short i = 1; i <= lvl; i++)
	{
		char Character = 0;
		while (Character != 'e')
		{
			Character = fgetc(Stages);
			if (Character == 'x')
				return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------

bool LoadStage(int lvl, GameObjects* Objects, GameInfo Info){
	FILE *Stages;
	fopen_s(&Stages, "Stages.txt", "r");
	if (Stages == NULL) //sprawdzenie czy istnieje wskazany plik
	{
		return false;
	}

	if (!FindStage(lvl, Stages))
	{
		fclose(Stages);
		return false;
	}

	for (short i = 0; i < MAX_BRICKS; i++){
		char Character = 0;
		while ((Character < '0' || Character > '9') && Character != '-'){
			Character = fgetc(Stages);

		}
		Objects->Brick[i].score = (Character - '0') * 10;

		if (Character >= '1' && Character <= '9' ) 
			Objects->Brick[i].destroyable = true;
		if (Character >= '0' && Character <= '9')
			Objects->Brick[i].alive = true;
		else 
			Objects->Brick[i].alive = false;

		switch (Character){
		/*case '-':
			break;*/
		case '0':
			Objects->Brick[i].destroyable = false;
			Objects->Brick[i].color = Info.COLOR.GREY;
			break;
		case '1':
			Objects->Brick[i].color = Info.COLOR.LIGHTBLUE;
			break;
		case '2':
			Objects->Brick[i].color = Info.COLOR.BLUE;
			break;
		case '3':
			Objects->Brick[i].color = Info.COLOR.DARKBLUE;
			break;
		case '4':
			Objects->Brick[i].color = Info.COLOR.LIGHTGREEN;
			break;
		case '5':
			Objects->Brick[i].color = Info.COLOR.GREEN;
			break;
		case '6':
			Objects->Brick[i].color = Info.COLOR.DARKGREEN;
			break;
		case '7':
			Objects->Brick[i].color = Info.COLOR.LIGHTRED;
			break;
		case '8':
			Objects->Brick[i].color = Info.COLOR.RED;
			break;
		case '9':
			Objects->Brick[i].color = Info.COLOR.DARKRED;
			break;

		}
	}


	fclose(Stages);
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------

//HighScore* LoadHighScores(){
void HighScoreScreen(GameObjects* Objects, GameInfo Info){
	FILE *HighScores;
	fopen_s(&HighScores, "HighScores.txt", "r");

	/*Jeœli plik nie istnieje to jest tworzony przykladowy*/
	if (HighScores == NULL)
	{
		fopen_s(&HighScores, "HighScores.txt", "w");
		fprintf(HighScores, 
			"MIREK#100000\n"
			"MARCIN#50000\n"
			"MARCEL#10000\n"
			"MATEUSZ#500\n"
			"MICHAL#10\n");
		fclose(HighScores);
		fopen_s(&HighScores, "HighScores.txt", "r");
	}
	
	/*Przepisywanie danych z pliku .txt do pamiêci programu*/
	for (short i = 0, j=0; i < 5; i++, j=0)
	{
		char Character = 0;
		while (true) 
		{
			Character = fgetc(HighScores);

			if (Character == '#')
			{
				Objects->Record[i].name[j] = NULL;
				break;
			}
			else
				Objects->Record[i].name[j++] = Character;
		} 
		
		Objects->Record[i].score = 0;
		while (true)
		{
			Character = fgetc(HighScores);

			if (Character == '\n')
				break;
			else
				Objects->Record[i].score = Objects->Record[i].score * 10 + Character - '0';
		}
	}
	fclose(HighScores);

	//////////////////////////////////////////////////////////////////////////////////////

	Objects->PositionInHighScores = 5;
	char name[20] = "";
	for (short j = 0; j < 20; j++)
		name[j] = 0;

	for (short i = 4; i >= 0; i--)
	{
		if (Objects->Player.score > Objects->Record[i].score)
		{
			/*Zrzut o 1 pozycjê w dó³ */
			if (i < 4)
			{
				Objects->Record[i + 1].score = Objects->Record[i].score;
				Rewrite(Objects->Record[i + 1].name, Objects->Record[i].name);
			}

			/*Wpisanie wyniku i roboczego imienia u¿ytkownika*/
			Objects->Record[i].score = Objects->Player.score;

			for (short j = 0; j < 20; j++)
				Objects->Record[i].name[j] = 0;

			Rewrite(Objects->Record[i].name, name);

			Objects->PositionInHighScores = i;
		}
	}

	while (Objects->quit == false && Objects->Lifes <= 0)
	{
		HighScoresControls(Objects);
		DrawHighScores(Info, *Objects);
	}

	//////////////////////////////////////////////////////////////////////////////////////

	if (Objects->quit == false)
	{
		fopen_s(&HighScores, "HighScores.txt", "w");
		for (short i = 0; i < 5; i++)
		{
			fprintf(HighScores, Objects->Record[i].name);
			fprintf(HighScores, "#%d\n", Objects->Record[i].score);
		}
		fclose(HighScores);
	}
}