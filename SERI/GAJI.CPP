#include"stdafx.h"
#include"superobjectclass.h"
#include"filemanager.h"

#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))

extern ImagePorcessClass    *Videomanager;
extern clFileManager		 SeriROM;
extern clFileManager		 MiniROM;


extern int BarPos;
#define GGUP			1
#define GGMERONG		2
#define GGDOWN			3
#define GGDAMAGE		4
extern int moleslot[9];

pIMAGE *Gaji::up[GAJIUP];
pIMAGE *Gaji::down[GAJIDOWN];
pIMAGE *Gaji::damage[GAJIDAMAGE];
pIMAGE *Gaji::merong[GAJIMERONG];

extern ObjectBase *Begin,*End;
extern CSound   *moledeath;

void GetGajiSprite()
{
  Gaji::up[0] = new pIMAGE();
  Gaji::up[0]->Load(NULL,MiniROM.SetFilePosition("gaji_nd04.spr")); 	
  Gaji::up[1] = new pIMAGE();
  Gaji::up[1]->Load(NULL,MiniROM.SetFilePosition("gaji_n1.spr")); 	

  Gaji::down[0] = new pIMAGE();
  Gaji::down[0]->Load(NULL,MiniROM.SetFilePosition("gaji_nd01.spr"));
  Gaji::down[1] = new pIMAGE();
  Gaji::down[1]->Load(NULL,MiniROM.SetFilePosition("gaji_nd02.spr"));
  Gaji::down[2] = new pIMAGE();
  Gaji::down[2]->Load(NULL,MiniROM.SetFilePosition("gaji_n03.spr"));
  Gaji::down[3] = new pIMAGE();
  Gaji::down[3]->Load(NULL,MiniROM.SetFilePosition("gaji_nd04.spr"));
  Gaji::down[4] = new pIMAGE();
  Gaji::down[4]->Load(NULL,MiniROM.SetFilePosition("gaji_nd05.spr"));

  Gaji::damage[0] = new pIMAGE();
  Gaji::damage[0]->Load(NULL,MiniROM.SetFilePosition("gaji_d01.spr"));
  Gaji::damage[1] = new pIMAGE();
  Gaji::damage[1]->Load(NULL,MiniROM.SetFilePosition("gaji_d02.spr"));
  Gaji::damage[2] = new pIMAGE();
  Gaji::damage[2]->Load(NULL,MiniROM.SetFilePosition("gaji_dd01.spr"));
  Gaji::damage[3] = new pIMAGE();
  Gaji::damage[3]->Load(NULL,MiniROM.SetFilePosition("gaji_dd02.spr"));
  Gaji::damage[4] = new pIMAGE();
  Gaji::damage[4]->Load(NULL,MiniROM.SetFilePosition("gaji_dd03.spr"));
  Gaji::damage[5] = new pIMAGE();
  Gaji::damage[5]->Load(NULL,MiniROM.SetFilePosition("gaji_dd04.spr"));
  Gaji::damage[6] = new pIMAGE();
  Gaji::damage[6]->Load(NULL,MiniROM.SetFilePosition("gaji_dd05.spr"));

  Gaji::merong[0] = new pIMAGE();
  Gaji::merong[0]->Load(NULL,MiniROM.SetFilePosition("gaji_n1.spr"));
  Gaji::merong[1] = new pIMAGE();
  Gaji::merong[1]->Load(NULL,MiniROM.SetFilePosition("gaji_n2.spr"));
  Gaji::merong[2] = new pIMAGE();
  Gaji::merong[2]->Load(NULL,MiniROM.SetFilePosition("gaji_n3.spr"));
  Gaji::merong[3] = new pIMAGE();
  Gaji::merong[3]->Load(NULL,MiniROM.SetFilePosition("gaji_n4.spr"));

}


void DropGajiSprite()
{
	int i;
	for(i=0;i<GAJIUP;i++) delete  Gaji::up[i];
	for(i=0;i<GAJIDOWN;i++) delete  Gaji::down[i];
	for(i=0;i<GAJIDAMAGE;i++) delete  Gaji::damage[i];
	for(i=0;i<GAJIMERONG;i++) delete  Gaji::merong[i];
}


Gaji::Gaji(int where,int delay)
{

	type = 1;
	hole = where;
	maxgo = where;
	switch(where)
	{
		case 0:
			x = 22;
			y = 51;
			layer = 3;
			break;
		case 1:
			x = 230;
			y = 51;
			layer = 3;
			break;
		case 2:
			x = 438;
			y = 51;
			layer = 3;
			break;

		case 3:
			x = 22;
			y = 147;
			layer = 5;
			break;
		case 4:
			x = 230;
			y = 147;
			layer = 5;
			break;
		case 5:
			x = 438;
			y = 147;
			layer = 5;
			break;

		case 6:
			x = 22;
			y = 243;
			layer = 7;
			break;
		case 7:
			x = 230;
			y = 243;
			layer = 7;
			break;
		case 8:
			x = 438;
			y = 243;
			layer = 7;
			break;
	}

	motion = GGUP;
	count = 4;
	frame = 0;
	delays = delay;
	checkbl = NULL;

}

void Gaji::action()
{

	switch(motion)
	{
		case GGUP:
			{
				if(count) count--;
				else
				{
					frame++;
					count = 2;
					if(frame >= GAJIUP)
					{
						frame = 0;
						motion = GGMERONG;
					}
				}
			}
			break;
		case GGMERONG:
			{
				if(count) count--;
				else
				{
					frame++;
					count = delays;
					if(frame >= GAJIMERONG)
					{
						frame = 0;
						motion = GGDOWN;
					}
				}
			}
			break;
		case GGDOWN:
			{
				frame++;
				count = delays;
				if(frame >= GAJIDOWN)
				{
					moleslot[hole] = 0;
					DisappearObject(SELF);
					BarPos--;
				}
			}
			break;
		case GGDAMAGE:
			{
				frame++;
				count = delays;
				if(frame >= GAJIDAMAGE)
				{
					moleslot[hole] = 0;
					DisappearObject(SELF);
					BarPos++;
					SeriData->killed++;
				}
			}
			break;

	}
}


void Gaji::putSprite()
{
	switch(motion)
	{
		case  GGUP:
			Videomanager->AddSpr(x,y,up[frame],_ANTIMODE,layer);
		break;
		case  GGMERONG:
			Videomanager->AddSpr(x,y,merong[frame],_ANTIMODE,layer);
		break;
		case  GGDOWN:
			Videomanager->AddSpr(x,y,down[frame],_ANTIMODE,layer);
		break;
		case  GGDAMAGE:
			Videomanager->AddSpr(x,y,damage[frame],_ANTIMODE,layer);
		break;
	}
	
}


void Gaji::SendState(int j)
{
  if(j == SEND_DAMAGE)
  {
	  motion = GGDAMAGE;
	  frame =0;
	  count = 3;
	  type = 2;
	  moledeath->Play(0);
  }
}