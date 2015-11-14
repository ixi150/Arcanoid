#include "Header.h"


Direction GameObject::CollisionRecRec(Rectangle rect){
	int top1 = y,
		bottom1 = y + h,
		left1 = x,
		right1 = x + w,

		top2 = rect.y,
		bottom2 = rect.y + rect.h,
		left2 = rect.x,
		right2 = rect.x + rect.w;

	if (top1 > bottom2 || bottom1 < top2 ||
		left1 > right2 || right1 < left2) return NONE;
	else 
	{
		int direction = NONE;
		if (top1 < top2)
		{
			direction += DOWN;
		}
		if (right1 > right2)
		{
			direction += LEFT;
		}
		if (left1 < left2)
		{
			direction += RIGHT;
		}
		if (bottom1 > bottom2)
		{
			direction += UP;
		}

		return static_cast<Direction>(direction);
	}

return NONE;
}
bool GameObject::CollisionCirPoint(Point point){
	int delta_x = x - point.x;
	int delta_y = y - point.y;
	if (sqrt(delta_x*delta_x + delta_y*delta_y) < r) return true;
	else return false;
}
Direction GameObject::CollisionCirRec(Rectangle rect){
	Point P;
	int direction = NONE;

	if (x < rect.x)
	{
		P.x = rect.x;
		direction += RIGHT;
	}
	else if (x > rect.x + rect.w)
	{
		P.x = rect.x + rect.w;
		direction += LEFT;
	}
	else
	{
		P.x = x;
	}

	if (y < rect.y)
	{
		P.y = rect.y;
		direction += DOWN;
	}
	else if (y > rect.y + rect.h)
	{
		P.y = rect.y + rect.h;
		direction += UP;
	}
	else
	{
		P.y = y;
		//if (direction == NONE) direction = CENTER;
	}
	
	if (CollisionCirPoint(P))
		return static_cast<Direction>(direction);
	else
		return NONE;
}


Direction GameObject::OutOfMap(){
	if (alive)
	{
		/*Lewa krawêdŸ*/
		if (x < WALLSX)
		{
			x = WALLSX;
			return LEFT;
		}
		/*Prawa krawêdŸ*/
		if (x+w > WALLSX + WALLS_WIDTH)
		{
			x = WALLSX + WALLS_WIDTH - w;
			return RIGHT;
		}
		/*Górna krawêdŸ*/
		if (y < WALLSY)
		{
			y = WALLSY;
			return UP;
		}
		/*Dolna krawêdŸ*/
		if (y+h > WALLSY + WALLS_HEIGHT)
		{
			y = WALLSY + WALLS_HEIGHT - h;
			return DOWN;
		}
	}
	return NONE;
}

bool GameBullet::OutOfMap(){
	if (alive)
	{
		/*Górna krawêdŸ*/
		if (y < WALLSY)
		{
			Destroy();
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////
//								BALL											 //
///////////////////////////////////////////////////////////////////////////////////

Direction GameBall::CollidesWith(GamePlayer Player){
	Direction Resoult = CollisionCirRec({ Player.x, Player.y, Player.w, Player.h });
	if (Resoult)
	{
		
		/*Przyklejenie pi³ki w razie aktywnego Bonusu*/
		if (Player.ActiveBonus[GLUE] > 0.0f)
			glued = true;

		/*Przyspieszanie pi³ki*/
		if (speed < MAX_SPEED) speed += 0.3;

		/*Zmiana k¹ta odbicia pi³ki*/
		if (Resoult / 10 > 0)
		{
			double deltaAngle = (x - (Player.x + Player.w / 2)) * 2 / Player.w;
			newAngle = deltaAngle * 70;
		}
		return Resoult;
	}
	else return NONE;
}
Direction GameBall::CollidesWith(GameBrick* Brick){
	Direction Resoult;
	for (short i = 0; i < MAX_BRICKS; i++)
	{
		if (Brick[i].alive)
		{
			Resoult = CollisionCirRec({ Brick[i].x, Brick[i].y, Brick[i].w, Brick[i].h });
			if (Resoult)
			{
				Brick[i].Destroy();
				return Resoult;
			}
		}
	}
	return NONE;
}
bool GameBall::CollidesWith(GameBlock* Block){
	for (short i = 0; i < MAX_BLOCKS; i++)
	{
		if (Block[i].alive)
		{
			if (CollisionCirRec({ Block[i].x, Block[i].y, Block[i].w, Block[i].h }))
			{
				Block[i].Destroy();
				angle = rand() % 360;
				return true;
			}
		}
	}
	return false;
}
Direction GameBall::OutOfMap(){
	if (alive)
	{
		/*Lewa krawêdŸ*/
		if (x - r < WALLSX)
		{
			x = WALLSX + r;
			return LEFT;
		}
		/*Prawa krawêdŸ*/
		if (x + r > WALLSX + WALLS_WIDTH)
		{
			x = WALLSX + WALLS_WIDTH - r;
			return RIGHT;
		}
		/*Górna krawêdŸ*/
		if (y - r < WALLSY)
		{
			y = WALLSY + r;
			return UP;
		}
		/*Dolna krawêdŸ*/
		if (y + r > WALLSY + WALLS_HEIGHT)
		{
			Destroy();
			return DOWN;
		}
	}
	return NONE;
}

///////////////////////////////////////////////////////////////////////////////////
//								BONUS											 //
///////////////////////////////////////////////////////////////////////////////////

bool GameBonus::CollidesWith(GamePlayer Player){
	if (CollisionRecRec({ Player.x, Player.y, Player.w, Player.h }))
	{
		Gather();
		Destroy();
		return true;
	}
	else return false;
}


///////////////////////////////////////////////////////////////////////////////////
//								BULLET											 //
///////////////////////////////////////////////////////////////////////////////////

bool GameBullet::CollidesWith(GameBrick* Brick){
	for (short i = 0; i < MAX_BRICKS; i++)
	{
		if (Brick[i].alive)
		{
			if (CollisionRecRec({ Brick[i].x, Brick[i].y, Brick[i].w, Brick[i].h }))
			{
				Brick[i].Destroy();
				Destroy();
				return true;
			}
		}
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////////
//								BLOCK											 //
///////////////////////////////////////////////////////////////////////////////////

bool GameBlock::CollidesWith(GamePlayer Player){
	if (CollisionRecRec({ Player.x, Player.y, Player.w, Player.h }))
	{
		Destroy();
		return true;
	}
	else return false;
}
Direction GameBlock::CollidesWith(GameBrick* Brick){
	Direction Resoult = NONE;
	for (short i = 0; i < MAX_BRICKS; i++)
	{
		if (Brick[i].alive)
		{
			Resoult = CollisionRecRec({ Brick[i].x, Brick[i].y, Brick[i].w, Brick[i].h });
			if (Resoult)
			{
				return Resoult;
			}
		}
	}
	return NONE;
}
Direction GameBlock::CollidesWith(GameBlock* Block){
	Direction Resoult = NONE;
	for (short i = 0; i < MAX_BLOCKS; i++)
	{
		if (Block[i].alive)
		{
			Resoult = CollisionRecRec({ Block[i].x, Block[i].y, Block[i].w, Block[i].h });
			if (Resoult)
			{
				return Resoult;
			}
		}
	}
	return NONE;
}


///////////////////////////////////////////////////////////////////////////////////
//								ALL												 //
///////////////////////////////////////////////////////////////////////////////////

void GameObject::BounceVertical(){
	angle *= -1;
}
void GameObject::BounceHorizontal(){
	angle = 180 - angle;
}