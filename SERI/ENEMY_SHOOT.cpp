#include"stdafx.h"
#include"superobjectclass.h"
#include"filemanager.h"


extern ImagePorcessClass    *Videomanager;
extern clFileManager		 SeriROM;
extern ObjectBase			*Seripoint;

#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))

RECT ES1 = {5,5,26,26};
RECT FOXSHOOT;
RECT ROBOTAN;
RECT WATERT[2];
RECT BOMMB[2];
pIMAGE *ENEMYeggshoot::iShoot[ENEMY_SPR];

extern ObjectBase *Begin,*End;

void GetENShootSprite()
{
	ENEMYeggshoot::iShoot[0] = new pIMAGE();
	ENEMYeggshoot::iShoot[0]->Load(NULL,SeriROM.SetFilePosition("zergshut.spr"));
	ENEMYeggshoot::iShoot[1] = new pIMAGE();
	ENEMYeggshoot::iShoot[1]->Load(NULL,SeriROM.SetFilePosition("fox_tan.spr"));
	ENEMYeggshoot::iShoot[2] = new pIMAGE();
	ENEMYeggshoot::iShoot[2]->Load(NULL,SeriROM.SetFilePosition("rtan1.spr"));
	ENEMYeggshoot::iShoot[3] = new pIMAGE();
	ENEMYeggshoot::iShoot[3]->Load(NULL,SeriROM.SetFilePosition("rtan2.spr"));
	ENEMYeggshoot::iShoot[4] = new pIMAGE();
	ENEMYeggshoot::iShoot[4]->Load(NULL,SeriROM.SetFilePosition("rtan3.spr"));
	ENEMYeggshoot::iShoot[5] = new pIMAGE();
	ENEMYeggshoot::iShoot[5]->Load(NULL,SeriROM.SetFilePosition("fox_tan2.spr"));
	ENEMYeggshoot::iShoot[6] = new pIMAGE();
	ENEMYeggshoot::iShoot[6]->Load(NULL,SeriROM.SetFilePosition("water_l.spr"));
	ENEMYeggshoot::iShoot[7] = new pIMAGE();
	ENEMYeggshoot::iShoot[7]->Load(NULL,SeriROM.SetFilePosition("water_r.spr"));
	ENEMYeggshoot::iShoot[8] = new pIMAGE();
	ENEMYeggshoot::iShoot[8]->Load(NULL,MiniROM.SetFilePosition("ftan.spr"));
	ENEMYeggshoot::iShoot[9] = new pIMAGE();
	ENEMYeggshoot::iShoot[9]->Load(NULL,MiniROM.SetFilePosition("ftanl.spr"));

	FOXSHOOT.top = 2;
	FOXSHOOT.left = 2;
	FOXSHOOT.bottom = 40;
	FOXSHOOT.right = 16;

	ROBOTAN.top = 0;
	ROBOTAN.left = 2;
	ROBOTAN.right = 8;
	ROBOTAN.bottom = 8;

	WATERT[0].top = 15;
	WATERT[0].left = 0;
	WATERT[0].right = 55;
	WATERT[0].bottom = 17;

	WATERT[1].top = 14;
	WATERT[1].left = 20;
	WATERT[1].right = 55;
	WATERT[1].bottom = 17;

	BOMMB[0].left = 3;
	BOMMB[0].top = 16;
	BOMMB[0].right = 17;
	BOMMB[0].bottom = 16;

	BOMMB[1].left = 11;                 //    ..
	BOMMB[1].top = 19;					//   ....
	BOMMB[1].right = 18;				//  ..  .....
	BOMMB[1].bottom = 13;				//       .....

}


void DropENShootSprite()
{
	delete ENEMYeggshoot::iShoot[0];
	delete ENEMYeggshoot::iShoot[1];
	delete ENEMYeggshoot::iShoot[2];
	delete ENEMYeggshoot::iShoot[3];
	delete ENEMYeggshoot::iShoot[4];
	delete ENEMYeggshoot::iShoot[5];
	delete ENEMYeggshoot::iShoot[6];
	delete ENEMYeggshoot::iShoot[7];
	delete ENEMYeggshoot::iShoot[8];
	delete ENEMYeggshoot::iShoot[9];
}


/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
ENEMYeggshoot::ENEMYeggshoot(int px,int py,int wh,int power)
{
    InScreen = 1;
	x = px;
	y = py;

	type = _ENEMY_SHOOT;
	name = 0;
	switch(wh)
	{
		case 0:
			addx = 7;
			frame = 0;
			addy = 0;
			this->cliprect =(RECT *)&ES1;
			break;
		case 1:
			frame = 0;
			addx = -7;
			addy = 0;
			this->cliprect =(RECT *)&ES1;
			break;
		case 2:			// 여우 총알
			frame = 1;
			addx = getrandom(-2,2);
			addy = -36;
			this->cliprect =(RECT *)&FOXSHOOT;
			break;
		case 4:			// 겨울 여우 총알
			frame = 5;
			addx = getrandom(-2,2);
			addy = -36;
			this->cliprect =(RECT *)&FOXSHOOT;
			break;

		case 5:			// 왼쪽으로 나간다.
			{
				frame = 6;
				count = 0;
				count3 = 0;
				addx = getrandom(-10,-6);
				addy = getrandom(-36,-18);
				name = wh;
				this->cliprect =(RECT *)&WATERT[0];
			}
			break;

		case 6:			// 오른쪽로 나간다.
			{
				frame = 7;
				count = 0;
				count3 = 0;
				name = wh;
				addx = getrandom(10,6);
				addy = getrandom(-36,-18);
				this->cliprect =(RECT *)&WATERT[1];
			}
			break;
	}
	life = power;
	checkbl = NULL;
	this->isize.x = iShoot[frame]->width;
	this->isize.y = iShoot[frame]->height;
}


ENEMYeggshoot::ENEMYeggshoot(int px,int py,int adx,int ady,int wh,int power)
{
	x = px;
	y = py;

	InScreen = 1;
	checkbl = NULL;
	type = _ENEMY_SHOOT;
	switch(wh)
	{
		case 3:
			addx = adx;
			addy = ady;
			this->cliprect =(RECT *)&ROBOTAN;
			count=2;
			name = 3;
			frame = getrandom(2,4);
			break;
		case 7:	// 왼쪽으로가는 폭탄   8번까지 쓴다
			addx = adx;
			addy = ady;
			count=2;
			name = wh;
			if(adx<0)
			{
				LoadMask(NULL,MiniROM.SetFilePosition("ftan.msk"));
				this->cliprect =(RECT *)&BOMMB[0];
				frame = 8;
				name = 7;
				addx*=-1;
			} else
			{ 
				LoadMask(NULL,MiniROM.SetFilePosition("ftanl.msk"));
				this->cliprect =(RECT *)&BOMMB[1];
				frame = 9;
				name = 8;
			}
			break;
	}
	life = power;
}


void ENEMYeggshoot::action(void)
{

	ObjectBase *object = Seripoint;
    if ( CheckCrash( OBJ_X, OBJ_X + OBJ_XL, OBJ_Y, OBJ_Y + OBJ_YL,
                      x+cliprect->left , x+cliprect->left+cliprect->right,
					  y+cliprect->top ,y+cliprect->top+cliprect->bottom)) 
	{
		object->SendState(SEND_DAMAGE+(life << 16));
		if(name == 7 || name == 8) ClearMyTiles();
			DisappearObject(SELF);
			return;
	}
	if(name != 7 && name != 8)
	{
		x+=addx;
		y+=addy;
	}

	switch(name)
	{
		case 3:
			{
				if(count<=0)
				{
					frame++;
					count = 1;
					if(frame > 4) frame = 2;
				} else count--;
			}
			break;
		case 5: case 6:
			addy+=3;
			break;
		case 7:
			if(addy<12) addy+=3;
			else addy = 12;
			if(IsMove(MOVE_LEFT | MOVE_ONLY_BLANK,addx) || IsMove(MOVE_DOWN,addy))
			{
				ClearMyTiles();
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				DisappearObject(SELF);
				return;
			}
			break;
			case 8:
			if(addy<12) addy+=3;
			else addy = 12;
			if(IsMove(MOVE_RIGHT | MOVE_ONLY_BLANK,addx) || IsMove(MOVE_DOWN | MOVE_ONLY_BLANK,addy))
			{
				ClearMyTiles();
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				InsertObject(new PAPUN(x+getrandom(-10,30),y-getrandom(5,15),getrandom(0,1)));
				DisappearObject(SELF);
				return;
			}
			break;
	}

	if(!(x - mapmanager->layer2.x + iShoot[frame]->width > 0 && x - mapmanager->layer2.x < 640 && y > 0 && y < mapmanager->height[1] << 5)) //y-mapmanager->layer2.y<480))
	{
		if(name == 7 || name == 8) ClearMyTiles();
		DisappearObject(SELF);
	}
}



void ENEMYeggshoot::putSprite(void)
{
		if(x-mapmanager->layer2.x+iShoot[frame]->width>0 && x-mapmanager->layer2.x <640 && y+iShoot[frame]->height > 0 && y-mapmanager->layer2.y > 0  && y-mapmanager->layer2.y + iShoot[frame]->height<480)
		Videomanager->AddSpr(x-mapmanager->layer2.x,y-mapmanager->layer2.y,iShoot[frame],_NORMALMODE,12);
}



void ENEMYeggshoot::SendState(int j)
{

}