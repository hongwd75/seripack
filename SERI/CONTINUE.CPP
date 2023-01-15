#include"stdafx.h"
#include"imageprocess.h"
#include"defineheader.h"
#include"windowheader.h"
#include"background.h"
#include"objectmanager.h"
#include"doll.h"
#include"superobjectclass.h"
#include"filemanager.h"
#include"음악정의.h"


extern clFileManager		 SeriROM;

//////////////////////////////////////////////////////////////////
#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))

extern ImagePorcessClass    *Videomanager;
extern int (*mainproc2)(void);

pIMAGE *countNum[11];
pIMAGE *countBack;
pIMAGE *ctimage[7];

void CountCreate(void)
{
	int i;
	char names[15];
	char names2[15];
	for(i = 0; i < 11; i++)
	{
		sprintf(names,"ct%d.spr",i);
		countNum[i] = new pIMAGE();
		countNum[i]->Load(NULL,SeriROM.SetFilePosition(names));
	}

	
	ctimage[0] = new pIMAGE();
	ctimage[1] = new pIMAGE();
	if(getrandom(0,50) == 44)
	{
		sprintf(names,"c_kfc01.spr");
		sprintf(names2,"c_kfc02.spr");
	} else 
	{
		sprintf(names,"c_dft_1.spr");
		sprintf(names2,"c_dft_2.spr");
	}
	ctimage[0]->Load(NULL,SeriROM.SetFilePosition(names));
	ctimage[1]->Load(NULL,SeriROM.SetFilePosition(names2));

	ctimage[2] = new pIMAGE();
	ctimage[2]->Load(NULL,SeriROM.SetFilePosition("c_dft.spr"));

	for(i=1; i < 5; i++)
	{
		sprintf(names,"c_end0%d.spr",i);
		ctimage[2+i] =new pIMAGE();
		ctimage[2+i]->Load(NULL,SeriROM.SetFilePosition(names));
	}

	countBack = new pIMAGE();
	countBack->Load(NULL,SeriROM.SetFilePosition("continue.spr"));
}


void DestroyCount(void)
{
	int i;
	for(i=0;i < 7; i++) delete ctimage[i];
	for(i=0;i < 11; i++) delete countNum[i];
	delete countBack;
}


/////////////////////////////////////////////
/////////////////////////////////////////////
#define MAXJUMMCT			60
int CTposx,CTposy;
int CTaddx=0,CTaddy=0;
int CTMaxiumjump = 0;
int CTdelay = 0;


void ShowCountNumber1(int s)
{
	static int oldn = 11;
	if(s != oldn)
	{
		oldn = s;
		CTposx = 300;
		CTposy = 180;
		CTdelay = 16;
		CTaddx = 0;
		CTaddy =10;
	}
	Videomanager->AddSpr(CTposx,CTposy,countNum[oldn],_NORMALMODE,10);
	switch(CTaddx)
	{
		case 0:			// 올라온다
			CTposy -= CTdelay;
			CTdelay-=2;
			if(CTdelay<=0) 
			{
				CTaddx++;
				CTdelay = 0;
			}
			break;
		case 1:
			CTdelay++;
			if(CTdelay==2)
			{
				CTposy+=CTaddy;
				CTdelay = 0;
				CTaddx = 2;
			}
			
			break;
		case 2:
			CTdelay++;
			if(CTdelay==2)
			{
				CTposy-=CTaddy;
				CTdelay = 0;
				CTaddx = 1;
				if(CTaddy > 0) CTaddy--;
			}
			break;

	}

}

void ShowCountNumber(int s)  //떨어지는거
{
	static int oldn = 11;
	static int ct=31;

	if(s != oldn)
	{
		oldn = s;
		ct = 31;
		CTposx = 300;
		CTposy = 80;
		

		CTMaxiumjump = CTMaxiumjump >> 1;
		CTaddx = getrandom(-1,1);
		CTaddy = getrandom(-30,-10);

		CTMaxiumjump = MAXJUMMCT;
		CTdelay = 0;
	}
	if(CTMaxiumjump>1)
	{
		Videomanager->AddSpr(CTposx,CTposy,countNum[oldn],_NORMALMODE,10);
		if(CTdelay--<=0){
			if(CTaddy>0)
			{
				if(CTaddy > 15) CTposy+=15;
				else CTposy+=CTaddy;
			} else 
			{
				if(CTaddy < -15) CTposy+=-15;
				else CTposy+=CTaddy;
			}

			
			CTaddy+=6;
			CTposx+=CTaddx;
			if(CTaddy > CTMaxiumjump)
			{
				CTMaxiumjump = CTMaxiumjump >> 1;
				CTaddy =  -CTMaxiumjump;
			}
			CTdelay=0;
		}
}

}
/////////////////////////////////////////////
/////////////////////////////////////////////

int ContinueProc(void)
{
	static int rett = 0;
	static int counter = 10;
	static int delays = 0;
	static int delays2;
	static int CFrame=0;

	

	switch(rett)
	{
		case 0:
			counter = 10;
			delays = 0;
			delays2 = 0;
			CFrame = 0;
			rett++;
			CountCreate();
			StopCD();
			PlayCD(GAMEOVERMUSIC);
			break;

		case 1:
			Videomanager->AddImage(0,0,countBack,_NORMALMODE,1);
			ShowCountNumber(counter);
			if((Df_GetInputState(VK_SPACE) || Df_GetInputState(VK_RETURN)) && counter < 10)
			{
				delays2 = 0;
				delays = 0;
				rett=3;
			}
			switch(counter)
			{
				case 10: case 9: case 8: case 7:
					if(delays2++>8)
					{
						CFrame = 1 - CFrame;
						delays2 = 0;
					}
					Videomanager->AddSpr(258,181,ctimage[CFrame],_NORMALMODE,2);
					break;
				case 6: case 5:
					delays2 = 0;
					CFrame = 3;
					Videomanager->AddSpr(258,181,ctimage[2],_NORMALMODE,2);
					break;

				case 4: case 3: case 2: case 1: case 0:
					if(delays2++>5)
					{
						CFrame++;
						if(CFrame>5) CFrame = 3;
						delays2 = 0;
					}
					if(counter) Videomanager->AddSpr(258,181,ctimage[CFrame],_NORMALMODE,2);
					else Videomanager->AddSpr(258,181,ctimage[6],_NORMALMODE,2);
					break;
			}
			if(delays++ > 80)
			{
				delays = 0;
				counter--;
				if(counter < 0) 
				{
					rett=2;
					delays2 = 0;
					Videomanager->AddSpr(258,181,ctimage[6],_NORMALMODE,2);
				}
			}
			break;

		case 2:
			if(delays++>7)
			{
				delays = 0;
				delays2++;
				Videomanager->AddSCREENEFFECT(1,_HALFSCREEN,1);
			}
			if(delays2>5)
			{
				DestroyCount();
				rett=0;
				return 3; // Game over;
			}
			break;
		case 3:
			if(delays++>7)
			{
				delays = 0;
				delays2++;
				Videomanager->AddSCREENEFFECT(1,_HALFSCREEN,1);
			}
			if(delays2>5)
			{
				DestroyCount();
				rett=0;
				return 2; // continue;
			}

			break;
	}
	return 0;
}