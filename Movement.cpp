#include "Header.h"

void GameObject::MoveByAngle(double delta, double Multiplier){
	x += Multiplier * speed * sin(angle * 3.14 / 180) * delta;
	y -= Multiplier * speed * cos(angle * 3.14 / 180) * delta;
}

void GamePlayer::Move(double delta, GameObjects* Objects){
	x -= force_x2 * delta;
	x += force_x1 * delta;
	if (OutOfMap())
	{
	}
	else
	{
		for (short i = 0; i < MAX_BALLS; i++)
		{
			if (Objects->Ball[i].glued)
			{
				Objects->Ball[i].x -= force_x2 * delta;
				Objects->Ball[i].x += force_x1 * delta;
			}
		}
	}
}

void GameBall::Move(double delta, GameObjects* Objects){
	if (alive)
	{
		double Multiplier = 1;
		if (glued)
			Multiplier = 0;
		if (Objects->Player.ActiveBonus[SLOW] > 0)
			Multiplier /= 2;

		MoveByAngle(delta, Multiplier);

		int Resoult = NONE;

		Resoult += CollidesWith(Objects->Player);
		if (Resoult % 10 == LEFT)
			x = Objects->Player.x + Objects->Player.w + r;
		if (Resoult % 10 == RIGHT)
			x = Objects->Player.x - r;
		if (Resoult == CENTER)
			y = Objects->Player.y - r;

		Resoult += OutOfMap();
		Resoult += CollidesWith(Objects->Brick);
		CollidesWith(Objects->Block);

		if (Resoult)
		{
			MoveByAngle(delta, -Multiplier);

			if (Resoult % 10 > 0)
				BounceVertical();
			if (Resoult / 10 > 0)
				BounceHorizontal();
			if (newAngle)
			{
				angle = newAngle;
				newAngle = 0.0f;
			}
		}
	}
}

void GameBonus::Move(double delta, GameObjects* Objects){
	if (alive)
	{
		double Multiplier = 1;
		if (Objects->Player.ActiveBonus[SLOW] > 0)
			Multiplier /= 2;

		MoveByAngle(delta, Multiplier);

		CollidesWith(Objects->Player);
		if (OutOfMap() == DOWN)
			Destroy();
	}
}
void GameBullet::Move(double delta, GameObjects* Objects){
	if (alive)
	{
		MoveByAngle(delta, 1.0f);

		CollidesWith(Objects->Brick);
		OutOfMap();
	}
}
void GameBlock::Move(double delta, GameObjects* Objects){
	if (alive)
	{
		angle += (rand()%20 - 10);
		double Multiplier = 1;
		MoveByAngle(delta, Multiplier);

		int Resoult = NONE;

		Resoult += CollidesWith(Objects->Player);
		if (Resoult % 10 == LEFT)
			x = Objects->Player.x + Objects->Player.w + r;
		if (Resoult % 10 == RIGHT)
			x = Objects->Player.x - r;

		Resoult += OutOfMap();
		Resoult += CollidesWith(Objects->Brick);
		Resoult += CollidesWith(Objects->Block);

		if (Resoult)
		{
			MoveByAngle(delta, -Multiplier);

			if (Resoult % 10 > 0)
				BounceVertical();
			if (Resoult / 10 > 0)
				BounceHorizontal();
		}
	}
}

void MoveGameObjects(GameObjects* Objects, double delta){
	Objects->Player.Move(delta, Objects);
	for (short i = 0; i < MAX_BALLS; i++)
		Objects->Ball[i].Move(delta, Objects);
	for (short i = 0; i < MAX_BONUSES; i++)
		Objects->Bonus[i].Move(delta, Objects);
	for (short i = 0; i < MAX_BULLETS; i++)
		Objects->Bullet[i].Move(delta, Objects);
	for (short i = 0; i < MAX_BLOCKS; i++)
		Objects->Block[i].Move(delta, Objects);
}