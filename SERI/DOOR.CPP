#include"stdafx.h"
#include"superobjectclass.h"


#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))


extern ImagePorcessClass    *Videomanager;

pIMAGE *DOOR::idoor;

RECT myrect;


void GetDOORPSprite()
{
	DOOR::idoor = new pIMAGE();
	DOOR::idoor->Load(NULL,SeriROM.SetFilePosition("door.spr"));
	myrect.left = 0;
	myrect.right = DOOR::idoor->width;
	myrect.top = 0;
	myrect.bottom = DOOR::idoor->height;
}

// 이미지 프리
void DropDOORPSprite()
{
	delete DOOR::idoor;
}


DOOR::DOOR(int px,int py,int wjump)
{
	type = _DOOR;
	motion = wjump;					// 어디로 갈것인지가 들어있다.
	x = px;
	y = py;
	cliprect = (RECT *)&myrect;
	checkbl = NULL;
}


void DOOR::action(void)
{
	
}


void DOOR::putSprite(void)
{
	if(x - mapmanager->layer2.x + idoor->width >= 0 && 
		x - mapmanager->layer2.x < 640 && y + idoor->height - mapmanager->layer2.y > 0 && 
		y - mapmanager->layer2.y < 480)	Videomanager->AddImage(x - mapmanager->layer2.x, y - mapmanager->layer2.y, idoor, _NORMALMODE, 6);
}

void DOOR::SendState(int j)
{
}