#include"stdafx.h"
#include"superobjectclass.h"

#define MYDIED				123
#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))

extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
extern ObjectBase *Seripoint;

pIMAGE *ENEMYmouse::mouse[6];
RECT    mouserect;

void GetMouseSprite()
{
	ENEMYmouse::mouse[0] = new pIMAGE();
	ENEMYmouse::mouse[0]->Load(NULL,SeriROM.SetFilePosition("mouser0.spr"));
	ENEMYmouse::mouse[1] = new pIMAGE();
	ENEMYmouse::mouse[1]->Load(NULL,SeriROM.SetFilePosition("mouser1.spr"));
	ENEMYmouse::mouse[2] = new pIMAGE();
	ENEMYmouse::mouse[2]->Load(NULL,SeriROM.SetFilePosition("mouser2.spr"));  // 맞는거
	ENEMYmouse::mouse[3] = new pIMAGE();
	ENEMYmouse::mouse[3]->Load(NULL,SeriROM.SetFilePosition("mousel0.spr"));
	ENEMYmouse::mouse[4] = new pIMAGE();
	ENEMYmouse::mouse[4]->Load(NULL,SeriROM.SetFilePosition("mousel1.spr"));
	ENEMYmouse::mouse[5] = new pIMAGE();
	ENEMYmouse::mouse[5]->Load(NULL,SeriROM.SetFilePosition("mousel2.spr"));  // 맞는거
	mouserect.top = 31;
	mouserect.left = 21;
	mouserect.right = 53;
	mouserect.bottom = 33;
}

void DropMouseSprite()
{
	delete ENEMYmouse::mouse[0];
	delete ENEMYmouse::mouse[1];
	delete ENEMYmouse::mouse[2];
	delete ENEMYmouse::mouse[3];
	delete ENEMYmouse::mouse[4];
	delete ENEMYmouse::mouse[5];
}



#define _LEFTLOOK			0
#define _RIGHTLOOK			3
#define _MYLIFE				8

ENEMYmouse::ENEMYmouse(int px,int py)
{
	type = _ENEMY_MOUSE;

	status = ON_LAND;
	
	int j = getrandom(0,1);

	if(j==0) leftlook = _LEFTLOOK;      // left look
		else leftlook = _RIGHTLOOK;		// right look

	x      = px;
	y      = py;
	frame  = 0;
	motion = 0;
	count  = 0;
	life   = _MYLIFE;
	LoadMask(NULL,SeriROM.SetFilePosition("mouse.msk"));
	cliprect = (RECT *)&mouserect;
}


void ENEMYmouse::action(void)
{

	if(status != DAMAGEX && status != MYDIED)
	{
		ObjectBase *object = Seripoint;
		if ( CheckCrash( OBJ_X, OBJ_X + OBJ_XL, OBJ_Y, OBJ_Y + OBJ_YL,
                      x+cliprect->left , x+cliprect->left+cliprect->right,
					  y+cliprect->top ,y+cliprect->top+cliprect->bottom)) 
		{
			object->SendState(SEND_DAMAGE+(8 << 16));
		}
	}

	switch(status)
	{
		case ON_LAND:
			    if(x < 0) 
				{
					ClearMyTiles();
					DisappearObject(SELF);
					return;
				}
				if(!IsMove(MOVEOK_DOWN,1))
				{
					status = FALLING;
				} else 
				{
					if(leftlook == _LEFTLOOK)
					{
						if(IsMove(MOVE_LEFT+MOVE_ONLY_BLANK,10))
						//else 
						{
							leftlook = _RIGHTLOOK;
						}
					} else 
						if(leftlook == _RIGHTLOOK)
						{
							if(IsMove(MOVE_RIGHT+MOVE_ONLY_BLANK,10))
						//	else 
							{
								leftlook = _LEFTLOOK;
							}
						}
				}
				frame = 1 - frame;
			break;
		case FALLING:
				if(this->IsMove(MOVE_DOWN,9)) 
				{
					status = ON_LAND;
				}
				frame = 1 - frame;
			break;

		case MYDIED:
				count++;
				if(leftlook == _RIGHTLOOK)	x-=4; else x+=4;
				if(count < 10) y-=8;
				else 
				{
					if(y-mapmanager->layer2.y>470) 
					{
						SeriData->killed++;
						DisappearObject(SELF);
					} else y+=14;
				}
			break;
		case DAMAGEX:
				count++;
				this->IsMove(MOVE_DOWN,9); 
				if(leftlook == _RIGHTLOOK)
				{
					if(!IsMove(MOVEOK_LEFT,14))
						IsMove(MOVE_LEFT,14);
				} else 
				if(leftlook == _LEFTLOOK)
				{
					if(!IsMove(MOVEOK_RIGHT,14))
							IsMove(MOVE_RIGHT,14);
				}
				if(count>8)
				{
					if(life==0) 
					{
						count = 0;
						ClearMyTiles();
						InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
						InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
						InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
						InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
						InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y),5));
						status = MYDIED;
					} else 
					{
						count = 0;
						frame = 0;
						status = ON_LAND;
					}
				}
			break;
	}
}

void ENEMYmouse::putSprite(void)
{
	if(x-mapmanager->layer2.x+mouse[frame+leftlook]->width>0 && x-mapmanager->layer2.x <640 && y-mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
		Videomanager->AddSpr(x-mapmanager->layer2.x,y-mapmanager->layer2.y-16,mouse[frame+leftlook],_ANTIMODE,13);
}


void ENEMYmouse::SendState(int j)
{
	int j2 = j & 0xFF;
	int dam = j >> 16;
	switch(j2)
	{
		case SEND_DAMAGE:
			if(status != DAMAGEX && status != MYDIED)
			{
				status = DAMAGEX;
				life-=dam;
				if(life < 0) life = 0;
				count = 0;
				frame = 2;
			}
		break;
	}
}