#include"stdafx.h"
#include"superobjectclass.h"


#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))


extern ImagePorcessClass    *Videomanager;

extern ObjectBase *Seripoint;


CSound   *itemsound;

pIMAGE *ITEM::item[ITEM_SIZE];




RECT clips[ITEM_SIZE];

void GetITEMSprite()
{
	ITEM::item[0] = new pIMAGE();
	ITEM::item[0]->Load(NULL,SeriROM.SetFilePosition("healing.spr"));

	ITEM::item[1] = new pIMAGE();
	ITEM::item[1]->Load(NULL,SeriROM.SetFilePosition("powerup.spr"));

	ITEM::item[2] = new pIMAGE();
	ITEM::item[2]->Load(NULL,SeriROM.SetFilePosition("healingbig.spr"));

	ITEM::item[3] = new pIMAGE();
	ITEM::item[3]->Load(NULL,SeriROM.SetFilePosition("ball.spr"));

	for(int i=0;i < 3;i++)
	{
		clips[i].top = 0;
		clips[i].left = 0;
		clips[i].right = ITEM::item[i]->width;
		clips[i].bottom = ITEM::item[i]->height;
	}

	itemsound = new CSound();
	itemsound->Initialize(SeriROM.SetFilePositionMMF("count2.wav"),1);
}

// 이미지 프리
void DropITEMSprite()
{
	delete ITEM::item[0];
	delete ITEM::item[1];
	delete ITEM::item[2];
	delete ITEM::item[3];
	delete itemsound;
}


ITEM::ITEM(int px,int py,int what)
{
	type = _ITEM;
	number = what;
	cliprect = (RECT *)&clips[what];
	checkbl = NULL;

	up =0;
	add = 0;
	x = px;
	y = py;
	count = 0;
	status = 0;
}


void ITEM::action(void)
{
	//Seripoint
	if(status == 0)
	{
		
		if(count++>3)
		{
			if(up)
			{
				y+=2;
				add++;
				if(add >= 5)
				{
					add=0;
					up=0;
				}
				
			} else 
			{
				y-=2;
				add++;
				if( add >=5)
				{
					add =0;
					up =1;
				}
			}
			count = 0;
		}
		ObjectBase *object = Seripoint;
		if(CheckCrash( OBJ_X, OBJ_X + OBJ_XL, OBJ_Y, OBJ_Y + OBJ_YL,
					                x+cliprect->left , x+cliprect->left+cliprect->right,y+cliprect->top,y+cliprect->top+cliprect->bottom))
		{
			status = 1;
			itemsound->Play(0);
			switch(number)
			{
				case _ITEM_HEALING:
					object->SendState(SEND_LIFEADD + (8 << 16));
					break;
				case _ITEM_POWERUP:
					object->SendState(SEND_POWERUP + (5 << 16));
					break;
				case _ITEM_ALLHEAL:
					object->SendState(SEND_LIFEFULL);
					break;
				case _ITEM_BALL:
					object->SendState(SEND_ADDSCORE + (10 << 16));
					break;
			}
		}

	} else
	{
		y-=3;
		count+=5;
		if(count>30) 
		{
			DisappearObject(SELF);
		}
	}
}


void ITEM::putSprite(void)
{
	
	if(status)
	{
		if(x-mapmanager->layer2.x+item[number]->width>0 && x-mapmanager->layer2.x <640 && y-mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
			Videomanager->AddSpr(x-mapmanager->layer2.x,y-mapmanager->layer2.y,item[number],_FADEMODE,15,31 - count);
	} else 
	{
		if(x-mapmanager->layer2.x+item[number]->width>0 && x-mapmanager->layer2.x <640 && y-mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
			Videomanager->AddSpr(x-mapmanager->layer2.x,y-mapmanager->layer2.y,item[number],_NORMALMODE,15);

	}
}

void ITEM::SendState(int j)
{
}