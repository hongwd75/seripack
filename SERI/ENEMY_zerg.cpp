#include"stdafx.h"
#include"superobjectclass.h"


#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))

#define MYLIFE			20

#define NORMAL			1
#define DIED			100


extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;

CSound   *eggsound;
RECT   zergrect[2];
pIMAGE *ENEMYegg::egg[20];

void GetEggSprite()
{
	for(int i = 0; i <10; i++)
	{
		char names[20];
		sprintf(names,"eggr%d.spr",i);
		ENEMYegg::egg[i] = new pIMAGE();
		ENEMYegg::egg[i]->Load(NULL,SeriROM.SetFilePosition(names));
	}
	for(int j = 0; j <10; j++)
	{
		char names[20];
		sprintf(names,"eggl%d.spr",j);
		ENEMYegg::egg[j+10] = new pIMAGE();
		ENEMYegg::egg[j+10]->Load(NULL,SeriROM.SetFilePosition(names));
	}

	zergrect[0].left	=  0;
	zergrect[0].top		= 25;
	zergrect[0].right	= 23;
	zergrect[0].bottom	= 39;

	zergrect[1].left	= 33;
	zergrect[1].top		= 25;
	zergrect[1].right	= 23;
	zergrect[1].bottom	= 39;

	eggsound  = new CSound();
	eggsound->Initialize(SeriROM.SetFilePositionMMF("eggbreak.wav"),1);

}

// 이미지 프리
void DropEggSprite()
{
	for(int i= 0; i < 20; i++) delete ENEMYegg::egg[i];
	delete eggsound;

}

ENEMYegg::ENEMYegg(int px,int py,int where)
{
	type = _ENEMY_EGG;
    InScreen = 1;
	if(where == _EGG_LEFT) 
	{
		wherelook = 10;
		LoadMask(NULL,SeriROM.SetFilePosition("zerg-eggl.msk"));
		cliprect = (RECT *)&zergrect[1];
	}
	else 
	{
		wherelook = 0;
		LoadMask(NULL,SeriROM.SetFilePosition("zerg-egg.msk"));
		cliprect = (RECT *)&zergrect[0];
	}

	status = NORMAL;
	x = px;
	y = py;
	frame = 0;
	count = 0;
	life  = MYLIFE;
	invincibility = 0;
	//PutMyTile();
}


void ENEMYegg::action(void)
{
	switch(status)
	{
		case NORMAL:
			count++;
			if(count>60)
			{
				if((count & 2) && (frame < 5)) frame++;

				if(frame==2 && (count & 2))
				{
					if(wherelook == 0)
						InsertObject(new ENEMYeggshoot(x+32,y+20,0,10));
					else InsertObject(new ENEMYeggshoot(x,y+20,1,10));
				}
				if(frame==5 && count == 80)   // 초기화
				{
					frame=0;
					count=0;
				}

			}
			break;
		case DAMAGEX:
			if(count==0)
			{
				if(frame>1 && frame <6)
				{
					frame = 6;
				}
				if(life==0)
				{
					frame = 7;
				}
			}
			count++;
			if(life==0)
			{
				if(!(count & 1)) frame++;
				if(frame>9) 
				{
					//DisappearObject(SELF);
					InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));		
					InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));		
					InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));		
					InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y),5));
					SeriData->killed++;
					type = _ZERG_ST;
					status = DIED;
					frame = 9;
					return;
				}
			}
			if(count>10) 
			{
				count = 0;
				status = NORMAL;
				frame = 0;
			}
			break;
	}
}


void ENEMYegg::putSprite(void)
{
	if(x-mapmanager->layer2.x+egg[frame+wherelook]->width>0 && x-mapmanager->layer2.x <640 && y-mapmanager->layer2.y+egg[frame+wherelook]->height>0 && y-mapmanager->layer2.y<480)
	{
		Videomanager->AddSpr(x-mapmanager->layer2.x,y-mapmanager->layer2.y,egg[frame+wherelook],_ANTIMODE,10);
		InScreen = 1;
	} else InScreen = 0;

}

void ENEMYegg::SendState(int j)
{
	int j2 = j & 0xFF;
	int dam = j >> 16;
	switch(j2)
	{
		case SEND_DAMAGE:
			if(status == NORMAL)
			{
				status = DAMAGEX;
				life-=dam;
				if(life<=0) 
				{
					life = 0;
					eggsound->Play(0);
				}
				count = 0;
			}
		break;
	}
}
