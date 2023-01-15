#include"stdafx.h"
#include"objectmanager.h"
#include"SuperObjectClass.h"
#include"imageprocess.h"
#include"defineheader.h"
#include"filemanager.h"
#include"video.h"
#include"음악정의.h"


extern clFileManager		 SeriROM;
extern HWND                 hwnd;												// window handle


extern tagSeriRELOADSystem tpSERISYSTEM;

extern tagSeridata	oldSeriState;
#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))

#define MENUPOSX	350	
#define MENUPOSY    160

extern clSCREEN				 *VIDEO;
extern CLObjectManager		 *Objectman;
extern ImagePorcessClass	 *Videomanager;
extern int Staffvoid(void);
extern int Collection(void);
extern int Openning(void);
extern int Ending(void);

pIMAGE *ibutn[6];			// 버튼들 
pIMAGE *iTitlebk;
pIMAGE *iSeriTitle;			
pIMAGE *iTitleScroll;		// 타이틀 배경 스크롤용 
pIMAGE *iTitleName;
CSound *click;
CSound *click2;


int  clicked;
int  titlesel = 0;


#define _GAME_START					1
#define _GAME_OPTION				2
#define _GAME_LOAD  				3
#define _GAME_CARD   				4
#define _GAME_EXIT  				5


int Ttitlebar = 0,Ttitlecounter=0;


void GetMainMenu(void)
{
	titlesel = 0;
	if(Ttitlecounter>0)
	{
		Videomanager->AddImage(MENUPOSX+34,MENUPOSY+129,ibutn[0],_NORMALMODE,5);
		Ttitlecounter--;
	} else
	{
		Videomanager->AddImage(MENUPOSX+34,MENUPOSY+129,ibutn[Ttitlebar+1],_NORMALMODE,5);
	}

	if((Df_GetInputState(VK_RIGHT) == _VKEY_ONE_DOWN || Df_GetInputState(VK_UP) == _VKEY_ONE_DOWN) && Ttitlecounter == 0)
	{
		Ttitlecounter = 8;
		Ttitlebar--;
		if(Ttitlebar < 0) Ttitlebar = 4;
		click2->Play(0);
	}

	if((Df_GetInputState(VK_LEFT) == _VKEY_ONE_DOWN || Df_GetInputState(VK_DOWN) == _VKEY_ONE_DOWN) && Ttitlecounter == 0)
	{
		Ttitlecounter = 8;
		Ttitlebar++;
		if(Ttitlebar > 4) Ttitlebar = 0;
		click2->Play(0);
	}
	if((Df_GetInputState(VK_SPACE) == _VKEY_ONE_DOWN  || Df_GetInputState(VK_RETURN) == _VKEY_ONE_DOWN ) && Ttitlecounter == 0)
	{
	  titlesel = Ttitlebar+1;
	}
/*
	if(Df_GetInputState(VK_LBUTTON) == _VKEY_ONE_DOWN)
	{
		POINT mpos = Df_GetMousePos();
		click2->Play(0);
		if(mpos.x>=MENUPOSX+34 && mpos.x<=MENUPOSX+34+152 && mpos.y>=MENUPOSY+129 && mpos.y<= MENUPOSY+129+112 && Ttitlecounter == 0)
		{
			titlesel = Ttitlebar+1;
		}
	}
*/
}

int Optionvoidloop = 0;

pIMAGE *Optionbk,*OPmenu[8];
int OptionScreen(void)
{
	static int life = SeriData->SetLife;
	static int speed = SeriData->SetSPEED;
	if(Optionvoidloop>0) 
	{
		Videomanager->AddImage(0,0,Optionbk,_NORMALMODE,1);	
		if(Df_GetInputState(VK_UP) == _VKEY_ONE_DOWN)
		{
			Optionvoidloop--;
			if(Optionvoidloop<=0) Optionvoidloop = 3;
		}
		if(Df_GetInputState(VK_DOWN) == _VKEY_ONE_DOWN)
		{
			Optionvoidloop++;
			if(Optionvoidloop>3) Optionvoidloop = 1;
		}

		Videomanager->AddSpr(290+(speed*40),163,OPmenu[6],_NORMALMODE,4);
		for(int th=0;th<life;th++)
		Videomanager->AddSpr(289+(th*40),259,OPmenu[7],_NORMALMODE,4);

	}
	switch(Optionvoidloop)
	{
		case  0:
			{
				Optionbk = new pIMAGE();
				for(int i = 0; i < 8;i++) OPmenu[i] = new pIMAGE();
				Optionbk->Load(NULL,SeriROM.SetFilePosition("optionbk.spr")); 
				OPmenu[0]->Load(NULL,SeriROM.SetFilePosition("speed.spr")); 
				OPmenu[1]->Load(NULL,SeriROM.SetFilePosition("seri.spr")); 
				OPmenu[2]->Load(NULL,SeriROM.SetFilePosition("cancel.spr")); 
				OPmenu[3]->Load(NULL,SeriROM.SetFilePosition("speed_n.spr")); 
				OPmenu[4]->Load(NULL,SeriROM.SetFilePosition("seri_n.spr")); 
				OPmenu[5]->Load(NULL,SeriROM.SetFilePosition("cancel_n.spr")); 
				OPmenu[6]->Load(NULL,SeriROM.SetFilePosition("ball.spr")); 
				OPmenu[7]->Load(NULL,SeriROM.SetFilePosition("serihead.spr")); 
				Optionvoidloop++;
			}
		break;
		case 1:
			{
			 	Videomanager->AddImage(133,154,OPmenu[0],_NORMALMODE,4);
				Videomanager->AddImage(133,257,OPmenu[4],_NORMALMODE,4);
				Videomanager->AddImage(266,344,OPmenu[5],_NORMALMODE,4);
				if(Df_GetInputState(VK_LEFT) == _VKEY_ONE_DOWN)
				{
					if(speed > 0) speed--;
				}
				if(Df_GetInputState(VK_RIGHT) == _VKEY_ONE_DOWN)
				{
					if(speed < 4) speed++;
				}

			}
		break;
		case 2:
			{
				Videomanager->AddImage(133,154,OPmenu[3],_NORMALMODE,4);
				Videomanager->AddImage(133,257,OPmenu[1],_NORMALMODE,4);
				Videomanager->AddImage(266,344,OPmenu[5],_NORMALMODE,4);
				if(Df_GetInputState(VK_LEFT) == _VKEY_ONE_DOWN)
				{
					if(life > 1) life--;
				}
				if(Df_GetInputState(VK_RIGHT) == _VKEY_ONE_DOWN)
				{
					if(life < 6) life++;
				}

			}

		break;
		case 3:
				Videomanager->AddImage(133,154,OPmenu[3],_NORMALMODE,4);
				Videomanager->AddImage(133,257,OPmenu[4],_NORMALMODE,4);
				Videomanager->AddImage(266,344,OPmenu[2],_NORMALMODE,4);
				if((Df_GetInputState(VK_SPACE) == _VKEY_ONE_DOWN  || Df_GetInputState(VK_RETURN) == _VKEY_ONE_DOWN ))
				{
					Videomanager->InitImageList();
					Optionvoidloop = 0;
					delete Optionbk;
					for(int i = 0; i < 8;i++) delete OPmenu[i];
					SeriData->SetLife = life;
					SeriData->SetSPEED = speed;
					return 1;
				}
				break;
		case -3:	// ESC
			{
				Videomanager->InitImageList();
				Optionvoidloop = 0;
				delete Optionbk;
				for(int i = 0; i < 8;i++) delete OPmenu[i];
				return 1;
			}
		break;
	}
	return 0;
}


void TitleDestroy(void)
{
	delete click;
	click2->Stop();
	delete click2;
	delete iTitleScroll;
	delete iSeriTitle;
	delete iTitleName;
	delete ibutn[0];
	delete ibutn[1];
	delete ibutn[2];
	delete ibutn[3];
	delete ibutn[4];
	delete ibutn[5];
	DroptStarSprite();
	ObjectClear();
	AllDelete();
}


int TitleProc(void) // void
{
	//int i =0;
	static int ct=31;
	static int titlect= -2;
	static int xxx = 223;
	static int ssx = 0;
   
	
	switch(titlect)
	{
		case -2:
			titlect++;
			VIDEO->Refresh = 40;
			break;

		case -1:
			PlayCD(OPENNINGMUSIC);
			titlect++;
			break;
		case 0:
			{
				Videomanager->AddImage(0,0,iTitleScroll,_NORMALMODE,1);
				Videomanager->AddSpr(120,80,iSeriTitle,_NORMALMODE,4);
				Videomanager->AddSpr(40,60,iTitleName,_NORMALMODE,5);
				titlect = 1;
				for(int i = 0; i < 30; i++) 
					InsertObject(new TitleStar(getrandom(0,630),0));
			}
			break;
		case 1:
			InsertObject(new TitleStar(getrandom(0,630),0));
			ActionObject();
			PutSprites();
			Videomanager->AddImage(0,0,iTitleScroll,_NORMALMODE,1);
			Videomanager->AddSpr(120,80,iSeriTitle,_ANTIMODE,3);
			Videomanager->AddSpr(40,60,iTitleName,_ANTIMODE,5);
			if(Df_GetInputState(VK_SPACE) || Df_GetInputState(VK_RETURN) || Df_GetInputState(VK_LBUTTON))
			{
				titlect = 2;
				ObjectClear();
			}
			break;
		case 2:
			titlect = 9;
			PlayCD(OPENNINGMUSIC);
			//ShowCursor(TRUE);
			break;
		case 9:
			Videomanager->AddImage(0,0,iTitleScroll,_NORMALMODE,1);
			Videomanager->AddSpr(120,80,iSeriTitle,_ANTIMODE,3);
			Videomanager->AddSpr(40,60,iTitleName,_ANTIMODE,5);
			Videomanager->AddSpr(MENUPOSX,MENUPOSY,iTitlebk,_ANTIMODE,4);
			GetMainMenu();
			if(titlesel) clicked = click->Play(0);
			switch(titlesel)
			{
				case _GAME_START:
					    while(click->IsPlaying(clicked)) ;
						TitleDestroy();
						Videomanager->InitImageList();
						titlect = 4;
				break;
				case _GAME_OPTION:
					{
						StopCD();
						OptionScreen();
						titlect = 3;
					}
				break;
				case _GAME_CARD:
					{
						StopCD();
						Collection();
						titlect= 5;
					}
				break;
				case _GAME_EXIT:
					    while(click->IsPlaying(clicked)) ;
						TitleDestroy();
						Videomanager->InitImageList();
						PostMessage(hwnd, WM_CLOSE, 0, 0);
						
				break;

				case _GAME_LOAD:
					{
						if(tpSERISYSTEM.stageswitch>0)
						{
							SeriData->Loadinit();
							oldSeriState.SeriLife = 35;
							InitSGame(Videomanager,tpSERISYSTEM.stageswitch);
							titlect = 2;
						}
					}
				break;
			}
		break;
		case 3:
			if(OptionScreen()) 
			{
				titlect = 2;
				
			}
		break;

		case 4:
			if(Openning())
			{
				SeriData->init(-1,0,0,1);
				SeriData->ResetCardSystem();
				InitSGame(Videomanager,1); // 1
				titlect = 2;
			}
		break;

		case 5:
			if(Collection()) 
			{
				titlect = 2;
			}
			break;
	}

	return 1;
}


int TitleCreate(void)
{

	titlesel = 0;

	iTitleScroll = new pIMAGE();
	iTitleScroll->Load(NULL,SeriROM.SetFilePosition("t_bg_t2.spr"));

	iTitleName = new pIMAGE();
	iTitleName->Load(NULL,SeriROM.SetFilePosition("제목.spr"));	

	iSeriTitle = new pIMAGE();
	iSeriTitle->Load(NULL,SeriROM.SetFilePosition("t_peguri.spr"));

	iTitlebk = new pIMAGE();
	iTitlebk->Load(NULL,SeriROM.SetFilePosition("menuback.spr"));

	ibutn[0] = new pIMAGE();
	ibutn[0]->Load(NULL,SeriROM.SetFilePosition("menuchange.spr"));
	ibutn[1] = new pIMAGE();
	ibutn[1]->Load(NULL,SeriROM.SetFilePosition("menustart.spr"));
	ibutn[2] = new pIMAGE();
	ibutn[2]->Load(NULL,SeriROM.SetFilePosition("menuoption.spr"));
	ibutn[3] = new pIMAGE();
	ibutn[3]->Load(NULL,SeriROM.SetFilePosition("menuload.spr"));
	ibutn[4] = new pIMAGE();
	ibutn[4]->Load(NULL,SeriROM.SetFilePosition("menucard.spr"));
	ibutn[5] = new pIMAGE();
	ibutn[5]->Load(NULL,SeriROM.SetFilePosition("menuexit.spr"));

	GettStarSprite();

	click = new CSound();
	click->Initialize(SeriROM.SetFilePositionMMF("w_mark.wav"),1);
	click2 = new CSound();
	click2->Initialize(SeriROM.SetFilePositionMMF("타자기.wav"),1);

	VIDEO->Refresh = 80;
	mainproc = TitleProc;
	ObjectClear();
	AllDelete();
	GetInitObject();
	SeriData->ResetCardSystem();			// 카드 시스템 초기화
	return 0;
}

