#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include<math.h>
#include<stdio.h>
#include<string.h>
#include<iostream>

extern "C" {
	//#ifdef BIT64
	//#include"./sdl64/include/SDL.h"
	//#include"./sdl64/include/SDL_main.h"
	//#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
	//#endif
}

///////////////////////////////////////////////////////////////////////////////////
//								DEFINICJE										 //
///////////////////////////////////////////////////////////////////////////////////

//#define SCREEN_WIDTH	640
//#define SCREEN_HEIGHT	480
#define SCREEN_WIDTH	1000
#define SCREEN_HEIGHT	800
#define FPS_REFRESH_RATE 0.5

#define WALLS_WIDTH	950
#define WALLS_HEIGHT 700
#define WALLSX  SCREEN_WIDTH / 2 - WALLS_WIDTH / 2
#define WALLSY  SCREEN_HEIGHT / 2 - WALLS_HEIGHT / 2 + 10


#define PLAYER_HEIGHT 40
#define PLAYER_WIDTH 200
#define WIDE_BONUS 400
#define WIDE_BONUS_SPEED 200

#define BRICKS_IN_WIDE 15
#define BRICKS_IN_TALL 12
#define BRICK_WIDTH	WALLS_WIDTH/BRICKS_IN_WIDE
#define BRICK_HEIGHT WALLS_HEIGHT/BRICKS_IN_TALL/2
#define MAX_BRICKS BRICKS_IN_WIDE * BRICKS_IN_TALL

#define MAX_BALLS 10
#define MAX_BULLETS 10
#define MAX_BONUSES 10
#define MAX_BLOCKS 4
#define BONUSES 6
enum GameEffect{
	SLOW, GLUE, LIFE, WIDE, THREE, SHOOTING
};

#define BONUS_CHANCE 40 //musi byc wartoœæ procentów (od 1 do 100)
#define BONUS_SLOW_CHANCE 10
#define BONUS_GLUE_CHANCE 10
#define BONUS_LIFE_CHANCE 10
#define BONUS_WIDE_CHANCE 10
#define BONUS_THREE_CHANCE 10
#define BONUS_SHOOTING_CHANCE 10

enum Direction{
	NONE, UP=10, DOWN=20, LEFT=1, RIGHT=2,
	UPLEFT=11, UPRIGHT=12, DOWNLEFT=21, DOWNRIGHT=22,
	CENTER=30
};


#define BULLET_SPEED 800
#define START_SPEED 250
#define MAX_SPEED 500
#define PLAYER_SPEED 600

///////////////////////////////////////////////////////////////////////////////////
//								STRUKTURY										 //
///////////////////////////////////////////////////////////////////////////////////
struct point{
	double x, y;
};
struct WindowPointers{
	SDL_Surface *charset;
	SDL_Surface *screen;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
};

struct Colors{
	int WHITE, LIGHTGREY, GREY, DARKGREY, BLACK, 
		LIGHTGREEN, GREEN, DARKGREEN,
		LIGHTRED, RED, DARKRED,
		LIGHTBLUE, BLUE, DARKBLUE,
		PURPLE, YELLOW, ORANGE, BROWN;
};

struct TimeInfo{
	int t1, t2, frames;
	double delta, worldTime, fpsTimer, fps;
};

struct Pictures{
	SDL_Surface
		*ball,
		*glue,
		*slow,
		*life,
		*wide,
		*three,
		*bullet,
		*bullets,
		*scores;
};

struct GameInfo{
	WindowPointers ArcanoidWindow;
	Colors COLOR;
	TimeInfo Time;
	Pictures Pics;
};

///////////////////////////////////////////////////////////////////////////////////
//								KLASY											 //
///////////////////////////////////////////////////////////////////////////////////
class GameObjects;
class GameObject;
class GamePlayer;
class GameBall;
class GameBrick;
class GameBullet;
class GameBonus;
class GameBlock;


class Point{
public:
	int x, y;
}; 
class Rectangle{
public:
	int x, y;
	int w, h;
};
class Circle: public Point{
public:
	int r;
};
	

class GameObject{
public:
	SDL_Surface* pic; //obrazek
	double x, y;	//wspó³rzêdne lewego górnego naro¿nika prostok¹ta lub œrodka ko³a
	double w, h;	//szerokoœæ i wysokoœæ
	double r;		//promieñ
	double angle; //k¹t lotu obiektu mierzony wg wskazówek zegara od godziny 12
	double speed; //szybkosæ lotu k¹towego
	bool alive;
	int score, color;
	//GameEffect effect;

	/*Methods*/
	GameObject();
	void Destroy();
	Direction OutOfMap();
	void BounceVertical();
	void BounceHorizontal();
	void MoveByAngle(double, double);
	void MoveByForce(double, GameObjects*);

protected:
	Direction CollisionRecRec(Rectangle);
	bool CollisionCirPoint(Point);
	Direction CollisionCirRec(Rectangle);
};


class GamePlayer : public GameObject{
public:
	double force_x1, force_x2; //dwie poziome si³y na potrzeby poruszania gracza
	double ActiveBonus[BONUSES], Reloading;
	int borderColor;

	/*Methods*/
	GamePlayer();
	void Move(double, GameObjects*);
	void Reset();
	//Direction CollidesWith(GameBall*);
};
class GameBall : public GameObject{
public:
	bool glued;
	double newAngle;
	/*Methods*/
	GameBall();
	
	void Move(double, GameObjects*);
	
	Direction OutOfMap();
	Direction CollidesWith(GamePlayer);
	Direction CollidesWith(GameBrick*);
	bool CollidesWith(GameBlock*);
};
class GameBonus : public GameObject{
public:
	GameEffect effect;
	int* pPlayerScore;
	double* pActiveBonus;
	int duration;

	/*Methods*/
	GameBonus();
	void Gather();
	void Move(double, GameObjects*);
	bool CollidesWith(GamePlayer);
};
class GameBrick : public GameObject{
public:
	int* pPlayerScore;
	GameBonus* pBonuses;
	bool destroyable;

	/*Methods*/
	GameBrick();
	void Destroy();
	void SpawnBonus();
};
class GameBullet : public GameObject{
public:

	/*Methods*/
	GameBullet();
	bool OutOfMap();
	void Move(double, GameObjects*);
	bool CollidesWith(GameBrick*);
};
class GameBlock : public GameObject{
public:

	/*Methods*/
	GameBlock();
	void Move(double, GameObjects*);
	bool CollidesWith(GamePlayer);
	Direction CollidesWith(GameBrick*);
	Direction CollidesWith(GameBlock*);
};

class GameObjects{
public:
	GamePlayer Player;
	GameBall Ball[MAX_BALLS];
	GameBrick Brick[MAX_BRICKS];
	GameBullet Bullet[MAX_BULLETS];
	GameBonus Bonus[MAX_BONUSES];
	GameBlock Block[MAX_BLOCKS];
	
	int Lifes, Stage;
	void TrippleBalls();
	
	short PositionInHighScores;
	class HighScore{
	public:
		int score;
		char name[20];
	}Record[5];

	bool quit, Restart;
};

//--------------------------------------------------------------------------------------//



///////////////////////////////////////////////////////////////////////////////////
//						DEKLARACJE FUNKCJI ZEWNETRZNYCH							 //
///////////////////////////////////////////////////////////////////////////////////

void DrawGame(GameInfo, GameObjects);

void DestroyScreen	(WindowPointers);
void SetUpWindow	(WindowPointers*);
void SetUpColors	(GameInfo*);
void SetUpPictures	(GameInfo*);
void SetUpTime		(TimeInfo*);
void SetUpGameObjects(GameObjects*, GameInfo);

void UpdateTime	(TimeInfo*);
void GameControls(GameObjects*);
void MoveGameObjects(GameObjects*, double);
bool LoadStage(int lvl, GameObjects*, GameInfo);
void WinLose(GameObjects*, GameInfo);

//HighScore* LoadHighScores();
void HighScoreScreen(GameObjects*, GameInfo);
void DrawHighScores(GameInfo , GameObjects );
void HighScoresControls(GameObjects* );

void RestartGame(GameObjects*, GameInfo);








