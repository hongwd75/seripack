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
extern HWND                 hwnd;	
//////////////////////////////////////////////////////////////////
void InsertSeriChar(void);
//////////////////////////////////////////////////////////////////
#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))


tagSeridata	oldSeriState;

extern int Ending(void);
extern int ContinueProc();
extern clSCREEN			 *VIDEO;
extern ImagePorcessClass    *Videomanager;
extern CLBackground			*mapmanager;
extern CLSeriManager		*SeriData;	
extern CLObjectManager		 *Objectman;
extern cWindow				 *myWindow;
extern ObjectBase *Seripoint;
extern int Staffvoid(void);

extern int Event(int evennum);
//////////// 스테이지 메세지
extern void ReSetShowTime(void);
extern void ShowStageNum(int sn,int sn2);
extern int StageClearScreen(void);


//////////// 미니골프용
extern void InitMiniGolfFiles(void);
extern int loops();
extern void DestroyMiniGolfFiles(void);

////////////  끝
int (*mainproc)(void);
int inGameLoop(void);
int inGameLoop2(void);
int inGameLoop3(void);
int inGameLoop4(void);
int inGameLoop5(void);
int inGameLoop6(void);

void ExitGameLoop2(void);
void ExitGameLoop3(void);

int GateRet = 1;
int Where	= 1;

int forstage1 = 0;
int tempcount;
/// 미니 게임용
extern int Bomb(void);
extern void DestroyBombstage(void);
extern int MoleGame();
extern void DestroyBOSS1();

pIMAGE *PauseBack;
pIMAGE *iPause[6];

pIMAGE *stHelp;

mChar *Serichar;

BOOL gamepause = FALSE;
int count   = 0;			// 게임 제어용 카운터

int (*mainproc2)(void);

void tempvoid(void)
{

}

void (*mainproc3)(void);
void (*quitprocess)(void);

void InitPause(void)
{
	PauseBack = new pIMAGE();
	iPause[0] = new pIMAGE();
	iPause[1] = new pIMAGE();
	iPause[2] = new pIMAGE();
	iPause[3] = new pIMAGE();
	iPause[4] = new pIMAGE();
	iPause[5] = new pIMAGE();

	PauseBack->Load(NULL,SeriROM.SetFilePosition("pauseback.spr"));
	iPause[0]->Load(NULL,SeriROM.SetFilePosition("main.spr"));
	iPause[1]->Load(NULL,SeriROM.SetFilePosition("exitgame.spr"));
	iPause[2]->Load(NULL,SeriROM.SetFilePosition("return.spr"));

	iPause[3]->Load(NULL,SeriROM.SetFilePosition("main_n.spr"));
	iPause[4]->Load(NULL,SeriROM.SetFilePosition("exitgame_n.spr"));
	iPause[5]->Load(NULL,SeriROM.SetFilePosition("return_n.spr"));
}

void DestroyPause(void)
{
	delete PauseBack;
	delete iPause[0];
	delete iPause[1];
	delete iPause[2];
	delete iPause[3];
	delete iPause[4];
	delete iPause[5];
}


int PauseGame(void)
{
	static int pos = 0;
	/* 우선 화면 출력*/

	if(Df_GetInputState(VK_UP) == _VKEY_ONE_DOWN)
	{
		pos--;
		if(pos < 0) pos = 2;
	}

	if(Df_GetInputState(VK_DOWN) == _VKEY_ONE_DOWN)
	{
		pos++;
		if(pos > 2) pos = 0;
	}


//	POINT mpos = Df_GetMousePos();
//	if(mpos.x>=266 && mpos.x<=378 && mpos.y>=154 && mpos.y<= 214) pos = 0;
//	if(mpos.x>=263 && mpos.x<=375 && mpos.y>=249 && mpos.y<= 309) pos = 1;
//	if(mpos.x>=264 && mpos.x<=376 && mpos.y>=339 && mpos.y<= 436) pos = 2;
	
	Videomanager->AddSpr(0,0,PauseBack,_NORMALMODE,199);
	if(pos == 0)
	{
		Videomanager->AddImage(266,154,iPause[0],_NORMALMODE,200);
		Videomanager->AddImage(263,249,iPause[4],_NORMALMODE,200);
		Videomanager->AddImage(264,339,iPause[5],_NORMALMODE,200);
	}
	if(pos == 1)
	{
		Videomanager->AddImage(266,154,iPause[3],_NORMALMODE,200);
		Videomanager->AddImage(263,249,iPause[1],_NORMALMODE,200);
		Videomanager->AddImage(264,339,iPause[5],_NORMALMODE,200);
	}
	if(pos == 2)
	{
		Videomanager->AddImage(266,154,iPause[3],_NORMALMODE,200);
		Videomanager->AddImage(263,249,iPause[4],_NORMALMODE,200);
		Videomanager->AddImage(264,339,iPause[2],_NORMALMODE,200);
	}


	if(Df_GetInputState(VK_RETURN) == _VKEY_ONE_DOWN)// || Df_GetInputState(VK_LBUTTON) == _VKEY_ONE_DOWN)
	{
		switch(pos)
		{
			case 2:	
				{
					mainproc = mainproc2;
				}
				return 0;
			case 0:		// Quit
				{
					
					ObjectClear();
					quitprocess();
					count =0;
					SeriData->init(-1,-1,0,0);	// 초기화
					SeriData->SetStage(1);
					mainproc = TitleCreate;
					Videomanager->InitImageList();
				}
				return 1;
			case 1:
				{
						ObjectClear();
						quitprocess();
						Videomanager->InitImageList();
						PostMessage(hwnd, WM_CLOSE, 0, 0);
				}
				return 1;
		}
	}

	if(Df_GetInputState(VK_ESCAPE) == _VKEY_ONE_DOWN)
	{
				mainproc = mainproc2;
	}

	return 0;	
}


/////////////////////////////////////////////////////////////////////
void LoadObjectfiles(void)
{
	SetSprite(_ITEMSTAR);
	SetSprite(_SERI);
	SetSprite(_ENEMY_RULKER);
	SetSprite(_SERI_SHOOT);
	SetSprite(_ENEMY_FOX);
	SetSprite(_ENEMY_CHIKEN);
	SetSprite(_ENEMY_RAM);
	SetSprite(_ENEMY_SLIM);
	SetSprite(_STAR);
	SetSprite(_ENEMY_MOUSE);
	SetSprite(_ENEMY_EGG);
	SetSprite(_ENEMY_SHOOT);
	SetSprite(_ENEMY_GHOST);
	SetSprite(_ENEMY_ROBO1);
	SetSprite(_ENEMY_ROBO2);
	SetSprite(_ENEMY_GOREA);
	SetSprite(_ENEMY_PIG);
	SetSprite(_ENEMY_BAKA);
	SetSprite(_PPBOMB);
	SetSprite(_PAPUN);
	SetSprite(_STEPS);
	SetSprite(_RAMP);
	SetSprite(_DOOR);
	SetSprite(_ITEM);
	SetSprite(_ENEMY_ROBO3);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void InitSGame(ImagePorcessClass *cdisplay,int stagebk)
{
	pIMAGE *loading;

	ShowCursor(FALSE);
	switch(stagebk)
	{
		case 1:
			loading = new pIMAGE();
			loading->Load(NULL,SeriROM.SetFilePosition("stage1nam.spr"));
			Videomanager->AddImage(0,0,loading,_NORMALMODE,12);
			Videomanager->DrawImageList();				//
			VIDEO->Flip();				//
			delete loading;
			break;
		case 2:
			loading = new pIMAGE();
			loading->Load(NULL,SeriROM.SetFilePosition("stage2nam.spr"));
			Videomanager->AddImage(0,0,loading,_NORMALMODE,12);
			Videomanager->DrawImageList();				//
			VIDEO->Flip();				//
			delete loading;
			break;
		case 3:
			loading = new pIMAGE();
			loading->Load(NULL,SeriROM.SetFilePosition("stage3nam.spr"));
			Videomanager->AddImage(0,0,loading,_NORMALMODE,12);
			Videomanager->DrawImageList();				//
			VIDEO->Flip();				//
			delete loading;
			break;
		case 4:
			loading = new pIMAGE();
			loading->Load(NULL,SeriROM.SetFilePosition("stage4nam.spr"));
			Videomanager->AddImage(0,0,loading,_NORMALMODE,12);
			Videomanager->DrawImageList();				//
			VIDEO->Flip();				//
			delete loading;
			break;
		case 5:
			loading = new pIMAGE();
			loading->Load(NULL,SeriROM.SetFilePosition("stage5nam.spr"));
			Videomanager->AddImage(0,0,loading,_NORMALMODE,12);
			Videomanager->DrawImageList();				//
			VIDEO->Flip();				//
			delete loading;
			break;
		case 6:
			loading = new pIMAGE();
			loading->Load(NULL,SeriROM.SetFilePosition("stage6nam.spr"));
			Videomanager->AddImage(0,0,loading,_NORMALMODE,12);
			Videomanager->DrawImageList();				//
			VIDEO->Flip();				//
			delete loading;
			break;


	}
	LoadObjectfiles();
//	Videomanager->VIDEO->Refresh = SeriData->SetSPEED;
	Videomanager->InitImageList();				//
	Objectman->DestroyObject();
	Objectman->InitObject();
	SeriData->SetStage(stagebk,1);
	SeriData->resetjumsu();
	ReSetShowTime();
	switch(stagebk)
	{

		case 1:
			forstage1 = 0;	
			mapmanager = new CLBackground(cdisplay);
			sprintf(GetFilePath,"%sdata\\stage\\stage1n-1.map",RunPath);
			mapmanager->initBackground(GetFilePath);
			mapmanager->ForceScroll = 0;
			SeriData->ResetSeriData(1);
			PlayCD(STAGE1MUSIC);
			AllDelete();
			sprintf(GetFilePath,"%sdata\\stage\\stage1n-1.ses",RunPath);
			ReadText(GetFilePath);
			GetInitObject();
			quitprocess = ExitGameLoop;				// 메모리 복구부분
			mainproc = inGameLoop;
			mainproc3 = tempvoid;
			break;


		case 2:
			count = -1;
			forstage1 = 0;				
			mapmanager = new CLBackground(cdisplay);
			sprintf(GetFilePath,"%sdata\\stage\\stage2n-1.map",RunPath);
			mapmanager->initBackground(GetFilePath);
			mapmanager->ForceScroll = 0;

			//SeriData->ResetSeriData(1);
			PlayCD(STAGE2MUSIC);
			AllDelete();
			sprintf(GetFilePath,"%sdata\\stage\\stage2n-1.ses",RunPath);
			ReadText(GetFilePath);
			GetInitObject();
			quitprocess = ExitGameLoop;				// 메모리 복구부분
			mainproc = inGameLoop2;
			mainproc3 = tempvoid;
			Seripoint->SendState(SEND_GETOLDSTATUS);
			break;

		case 3:
			GateRet = 1;
			Where	= 1;
			forstage1=0;
//			SeriData->ResetSeriData(1); ///////////////////////////////////////////////
			mapmanager = new CLBackground(cdisplay);
			sprintf(GetFilePath,"%sdata\\stage\\stage1-1.map",RunPath);
			mapmanager->initBackground(GetFilePath);
			mapmanager->ForceScroll = 0;
			PlayCD(STAGE3MUSIC);
			AllDelete();
			SeriData->SetStage(stagebk,0);
			sprintf(GetFilePath,"%sdata\\stage\\stage1-1.ses",RunPath);
			ReadText(GetFilePath);
			GetInitObject();
			quitprocess = ExitGameLoop;				// 메모리 복구부분
			mainproc = inGameLoop3;
			mainproc3 = tempvoid;
			Seripoint->SendState(SEND_GETOLDSTATUS);
			break;


		case 4:
			forstage1 = 0;				
			mapmanager = new CLBackground(cdisplay);
			sprintf(GetFilePath,"%sdata\\stage\\stage4n-1.map",RunPath);
			mapmanager->initBackground(GetFilePath);
			mapmanager->ForceScroll = 0;

			PlayCD(STAGE4MUSIC);
			AllDelete();
			sprintf(GetFilePath,"%sdata\\stage\\stage4n-1.ses",RunPath);
			ReadText(GetFilePath);
			GetInitObject();
			quitprocess = ExitGameLoop;				// 메모리 복구부분
			mainproc = inGameLoop4;
			mainproc3 = tempvoid;
			Seripoint->SendState(SEND_GETOLDSTATUS);
			break;

		case 5:
			AllDelete();
			DestroySprite(_ITEMSTAR);
			DestroySprite(_SERI);
			DestroySprite(_ENEMY_RULKER);
			DestroySprite(_SERI_SHOOT);
			DestroySprite(_ENEMY_FOX);
			DestroySprite(_ENEMY_CHIKEN);
			DestroySprite(_ENEMY_RAM);
			DestroySprite(_ENEMY_SLIM);
			DestroySprite(_STAR);
			DestroySprite(_ENEMY_MOUSE);
			DestroySprite(_ENEMY_EGG);
			DestroySprite(_ENEMY_SHOOT);
			DestroySprite(_ENEMY_GHOST);
			DestroySprite(_ENEMY_ROBO1);
			DestroySprite(_ENEMY_ROBO2);
			DestroySprite(_ENEMY_GOREA);
			DestroySprite(_ENEMY_PIG);
			DestroySprite(_ENEMY_BAKA);
			DestroySprite(_PPBOMB);
			DestroySprite(_PAPUN);
			DestroySprite(_STEPS);
			DestroySprite(_RAMP);
			DestroySprite(_DOOR);
			DestroySprite(_ITEM);
			DestroySprite(_ENEMY_ROBO3);

			PlayCD(MINIGOLF);
			InitMiniGolfFiles();
			quitprocess = ExitGameLoop3;				// 메모리 복구부분
			mainproc = inGameLoop5;
			mainproc3 = DestroyMiniGolfFiles;
//			ShowCursor(TRUE);
			break;

		case 6:
			forstage1 = 0;				
			GetDarkSeriFile();
			mapmanager = new CLBackground(cdisplay);
			sprintf(GetFilePath,"%sdata\\stage\\stage6n-1.map",RunPath);
			mapmanager->initBackground(GetFilePath);
			mapmanager->ForceScroll = 0;
			PlayCD(STAGE6MUSIC);
			AllDelete();
			sprintf(GetFilePath,"%sdata\\stage\\stage6n-1.ses",RunPath);
			ReadText(GetFilePath);
			GetInitObject();
			quitprocess = ExitGameLoop;				// 메모리 복구부분
			mainproc = inGameLoop6;
			mainproc3 = tempvoid;
			break;

	}

}


/// STAGE 1
int inGameLoop(void)
{
	switch(count)
	{
		case 0:
			ShowStageNum(SeriData->stage,SeriData->dashstage);
			SeriData->Display();
			ActionObject();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			PutSprites();
			if(Df_GetInputState(VK_ESCAPE) == _VKEY_ONE_DOWN)
			{
				mainproc2 = inGameLoop;
				mainproc = PauseGame;

			}
			
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);

			if(Seripoint->x>=9300) //9//////////////////////////////////////////////////////////////////////////
			{
				count = 3;
				tempcount = 0;
				Seripoint->SendState(SEND_GETOLDSTATUS);
			}
/*
			if(Df_GetInputState(VK_F8) == _VKEY_ONE_DOWN)
			{
				count = 3;
				tempcount = 0;
				Seripoint->SendState(SEND_GETOLDSTATUS);
			}
*/
			if(SeriData->gameend == 2) count = 2;		//다시 시작
			else if(SeriData->gameend == 1) 
			{
				PlayCD(GAMEOVERMUSIC);
				count = 1;
				ObjectClear();
				AllDelete();
			}
			return 0;
		case 1:
			{
				int i = ContinueProc();
				switch(i)
				{
					case 2:			// Restart;
						count =2;
						SeriData->ResetSeriData();
						break;
					case 3:			// Game over;
						count =0;
						quitprocess();
						mainproc = TitleCreate;
						SeriData->ResetSeriData(1);
						return 1;
				}
			}
			return 0;

		case 2:
			count =0;
			ObjectClear();
			AllDelete();
			PlayCD(STAGE1MUSIC);
			if(forstage1)
			{
				ReSetShowTime();
				sprintf(GetFilePath,"%sdata\\stage\\stage1n-2.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stage1n-2.ses",RunPath);
				ReadText(GetFilePath);
			}
			//forstage1 = 0;
			//SeriData->SetStage(1,2);
			else
			{
				ReSetShowTime();
				sprintf(GetFilePath,"%sdata\\stage\\stage1n-1.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stage1n-1.ses",RunPath);
				ReadText(GetFilePath);

//		mapmanager->initBackground(".\\data\\stage\\stage1n-1.map");
//		ReadText(".\\data\\stage\\stage1n-1.ses");
			}
			GetInitObject();
			SeriData->gameend = 0;
			return 0;

		case 3:
			if(tempcount++>6)
			{
				forstage1++;
				if(forstage1 == 1)
				{
					count = 0;
					ObjectClear();
					AllDelete();
					SeriData->SetStage(1,2);
				//	mapmanager->initBackground(".\\data\\stage\\stage1n-2.map");
				//	ReadText(".\\data\\stage\\stage1n-2.ses");
					sprintf(GetFilePath,"%sdata\\stage\\stage1n-2.map",RunPath);
					mapmanager->initBackground(GetFilePath);
					sprintf(GetFilePath,"%sdata\\stage\\stage1n-2.ses",RunPath);
					ReadText(GetFilePath);

					GetInitObject();
					SeriData->gameend = 0;
					Seripoint->SendState(SEND_SETOLDSTATUS);
					ReSetShowTime();
				} else
				{
					count = 5;
					PlayCD(BOSSMUSIC);
				}
			} else Videomanager->AddSCREENEFFECT(1,_HALFSCREEN,1);

			return 0;

		case 5:  // MINIGAME
			{
				if(Df_GetInputState(VK_ESCAPE) == _VKEY_ONE_DOWN)
				{
					mainproc2 = inGameLoop;
					mainproc = PauseGame;
					mainproc3 = DestroyBOSS1; // DestroyBombstage;
				}
				int rests = MoleGame(); // Bomb();
				if(rests)
				{
					if(rests == 3)
					{
						count =0;
						quitprocess();
						mainproc = TitleCreate;
						return 1;
					}
					if(rests == 2)  // Clear
					{

						count = 6;
					}
				}
			}
			return 0;

		case 6:
			if(StageClearScreen())
			{
				count = 0;
				mainproc3 = tempvoid;
				quitprocess();
				InitSGame(Videomanager,2);
				SaveSystemFile(2,SeriData->life,SeriData->SetLife,SeriData->score,SeriData->addpower);
			}
			return 0;
	}
	return 0;
}

/// STAGE 2
int stfade;
int inGameLoop2(void)
{
	switch(count)
	{
		case -1:
			SeriData->Display();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			PutSprites();
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			stHelp = new pIMAGE();
			stHelp->Load(NULL,MiniROM.SetFilePosition("tip2.spr"));
			count = -2;
			stfade = 0;
			break;

		case -2:
			SeriData->Display();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			PutSprites();
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			Videomanager->AddSpr(36,76,stHelp,_FADEMODE,128,stfade);
			stfade++;
			if(stfade>30) count = -3;
			break;

		case -3:
			SeriData->Display();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			PutSprites();
			Videomanager->AddSpr(36,76,stHelp,_NORMALMODE,128);
			if(Df_GetInputState(VK_SPACE) || Df_GetInputState(VK_RETURN) || Df_GetInputState(VK_LBUTTON)) count = -4;
			break;

		case -4:
			SeriData->Display();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			PutSprites();
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			Videomanager->AddSpr(36,76,stHelp,_FADEMODE,128,stfade);
			stfade--;
			if(stfade<1) count = -5;
			break;
		case -5:
			SeriData->Display();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			PutSprites();
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			delete stHelp;
			mapmanager->ForceScroll = 1;
			count = 0;
			break;
		case 0:
			ShowStageNum(SeriData->stage,SeriData->dashstage);
			SeriData->Display();
			ActionObject();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			PutSprites();
			if(Df_GetInputState(VK_ESCAPE) == _VKEY_ONE_DOWN)
			{
				mainproc2 = inGameLoop2;
				mainproc = PauseGame;

			}
			
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);

			if(Seripoint->x>=11000)//11000)
			{
				count = 3;
				tempcount = 0;
				Seripoint->SendState(SEND_GETOLDSTATUS);
			}
/*
			if(Df_GetInputState(VK_F8) == _VKEY_ONE_DOWN)
			{
				count = 3;
				tempcount = 0;
				Seripoint->SendState(SEND_GETOLDSTATUS);
			}
*/
			if(SeriData->gameend == 2) count = 2;		//다시 시작
			else if(SeriData->gameend == 1) 
			{
				PlayCD(GAMEOVERMUSIC);
				count = 1;
				ObjectClear();
				AllDelete();
			}
			return 0;
		case 1:
			{
				int i = ContinueProc();
				switch(i)
				{
					case 2:			// Restart;
						count =2;
						forstage1=0;
						SeriData->ResetSeriData();
						break;
					case 3:			// Game over;
						count =0;
						forstage1=0;
						quitprocess();
						mainproc = TitleCreate;
						SeriData->ResetSeriData(1);
						return 1;
				}
			}
			return 0;

		case 2:
			count = 0;
			ObjectClear();
			AllDelete();
			PlayCD(STAGE2MUSIC);
			if(forstage1 == 1)
			{
				SeriData->SetStage(2,2);
				ReSetShowTime();
//				mapmanager->initBackground(".\\data\\stage\\stage2n-2.map");
//				ReadText(".\\data\\stage\\stage2n-2.ses");
				sprintf(GetFilePath,"%sdata\\stage\\stage2n-2.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stage2n-2.ses",RunPath);
				ReadText(GetFilePath);


			} else 
			{
				forstage1 = 0;
				SeriData->SetStage(2,1);
				ReSetShowTime();
//				mapmanager->initBackground(".\\data\\stage\\stage2n-1.map");
//				ReadText(".\\data\\stage\\stage2n-1.ses");
				sprintf(GetFilePath,"%sdata\\stage\\stage2n-1.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stage2n-1.ses",RunPath);
				ReadText(GetFilePath);

			}
			GetInitObject();
			SeriData->gameend = 0;
			return 0;

		case 3:
			if(tempcount++>8)
			{
				forstage1++;
				PlayCD(STAGE2MUSIC);
				if(forstage1 == 1)
				{
					count = 0;
					ObjectClear();
					AllDelete();
					SeriData->SetStage(2,2);
//					mapmanager->initBackground(".\\data\\stage\\stage2n-2.map");
//					ReadText(".\\data\\stage\\stage2n-2.ses");
					sprintf(GetFilePath,"%sdata\\stage\\stage2n-2.map",RunPath);
					mapmanager->initBackground(GetFilePath);
					sprintf(GetFilePath,"%sdata\\stage\\stage2n-2.ses",RunPath);
					ReadText(GetFilePath);

					GetInitObject();
					SeriData->gameend = 0;
					Seripoint->SendState(SEND_SETOLDSTATUS);
				} else
				{
					count = 5;
				}
			} else Videomanager->AddSCREENEFFECT(1,_HALFSCREEN,1);

			return 0;

		case 5:  // MINIGAME
			if(StageClearScreen())
			{
				count = 0;
				ObjectClear();
				AllDelete();
				quitprocess();
				SeriData->gameend = 0;
				SaveSystemFile(3,SeriData->life,SeriData->SetLife,SeriData->score,SeriData->addpower);
				InitSGame(Videomanager,3);
			}
			return 0;
	}
	return 0;
}


/// STAGE 3
int inGameLoop3(void)
{
	// static int count=0;

	switch(count)
	{
		case 0:
			GateRet = 1;		// 대문의 리턴값
			ShowStageNum(SeriData->stage,0);
			SeriData->Display();
			ObjectRestore();

			if(Df_GetInputState(VK_ESCAPE) == _VKEY_ONE_DOWN)
			{
				mainproc2 = inGameLoop3;
				mainproc = PauseGame;
			}

			ActionObject();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			PutSprites();
			
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);

			if(SeriData->gameend == 2) 
			{
				count = 2;		//다시 시작
				
			//	GateRet = Where;
			}
			else if(SeriData->gameend == 1) 
			{
				
				PlayCD(11);
				count = 1;
				ObjectClear();
				AllDelete();
			}
			return 0;
		case 1:
			{
				int i = ContinueProc();
				switch(i)
				{
					case 2:			// Restart;
						count =2;
						Where = GateRet = 1;
						SeriData->ResetSeriData();
						break;
					case 3:			// Game over;
						count =0;
						quitprocess();
						mainproc = TitleCreate;
						return 1;
				}
			}
			return 0;

		case 2:
			{
				count =0;
				ObjectClear();
				AllDelete();
				Where = GateRet;
				GateRet = 1;
//				SeriData->stage = Where;
				for(int i = 0; i <10; i++) Videomanager->AddSCREENEFFECT(0,_HALFSCREEN,1);
				char filenames1[128];
				char filenames2[128];
				if(Where < 23)
				{
					PlayCD(STAGE3MUSIC);
					sprintf(filenames1,"%sdata\\stage\\stage1-%d.map",RunPath,Where);
					sprintf(filenames2,"%sdata\\stage\\stage1-%d.ses",RunPath,Where);
					mapmanager->initBackground(filenames1);
					ReadText(filenames2);
					GetInitObject();
					Seripoint->SendState(SEND_SETOLDSTATUS);		
					SeriData->gameend = 0;
				} else 
				{
					Where = 23;
					count = 10;
				}
			}
			return 0;

		case 10:
				mapmanager->iMessage(_ID_SHOW,NULL,1,1);
				stHelp = new pIMAGE();
				stHelp->Load(NULL,MiniROM.SetFilePosition("tip3.spr"));
				count = 11;
				stfade = 0;
			break;

		case 11:
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			Videomanager->AddSpr(36,76,stHelp,_FADEMODE,128,stfade);
			stfade++;
			if(stfade>30) count = 12;
			break;

		case 12:
			SeriData->Display();
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			Videomanager->AddSpr(36,76,stHelp,_NORMALMODE,128);
			if(Df_GetInputState(VK_SPACE) || Df_GetInputState(VK_RETURN) || Df_GetInputState(VK_LBUTTON)) count = 13;
			break;

		case 13:
			SeriData->Display();
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			Videomanager->AddSpr(36,76,stHelp,_FADEMODE,128,stfade);
			stfade--;
			if(stfade<1) count = 14;
			break;
		case 14:
			SeriData->Display();
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			delete stHelp;
			count = 4;
			break;


		case 4:
			if(Event(3))
			{
				PlayCD(STAGE3MUSIC);
//				mapmanager->initBackground(".\\data\\stage\\stage3-e.map");
//				ReadText(".\\data\\stage\\stage3-e.ses");
				sprintf(GetFilePath,"%sdata\\stage\\stage3-e.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stage3-e.ses",RunPath);
				ReadText(GetFilePath);
				mapmanager->Noscroll = TRUE;
				GetInitObject();
				InsertObject(new ENEMYwater(0,0));
				Seripoint->SendState(SEND_SETOLDSTATUS);
				SeriData->gameend = 0;
				count = 5;
			} else mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			break;


		case 5:
			GateRet = 1;		// 대문의 리턴값
			SeriData->Display();
			ObjectRestore();

			if(Df_GetInputState(VK_ESCAPE) == _VKEY_ONE_DOWN)
			{
				mainproc2 = inGameLoop3;
				mainproc = PauseGame;
			}


			ActionObject();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			PutSprites();
			
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
/*
			if(Df_GetInputState(VK_F8) == _VKEY_ONE_DOWN)
			{
				GateRet = 4;
				count = 7;
				tempcount = 0;
				Seripoint->SendState(SEND_GETOLDSTATUS);
			}
*/
			if(SeriData->gameend == 2) 
			{
				
				if(GateRet == 4)
				{
					count = 7;
					tempcount = 0;
//					Seripoint->SendState(SEND_GETOLDSTATUS);
					return 0;
				} else count = 9;		// 다시 시작
			}
			else if(SeriData->gameend == 1) 
			{
				PlayCD(STAGE3MUSIC);
				count = 6;
				ObjectClear();
				AllDelete();
			}
			return 0;
		case 6:
			{
				int i = ContinueProc();
				switch(i)
				{
					case 2:			// Restart;
						count =9;
						GateRet = 1;
						SeriData->ResetSeriData();
						break;
					case 3:			// Game over;
						count =0;
						quitprocess();
						mainproc = TitleCreate;
						return 1;
				}
			}
			return 0;

		case 7:
			{
				tempcount = 0;
				ObjectClear();
				AllDelete();
				ReSetShowTime();
				Where = GateRet;
				GateRet = 1;
				for(int i = 0; i <10; i++) Videomanager->AddSCREENEFFECT(0,_HALFSCREEN,1);
				SeriData->gameend = 0;
				count = 8;
			}
			return 0;

		case 8:

			if(StageClearScreen())
			{
				count = 0;
				mainproc3 = tempvoid;
				quitprocess();
				InitSGame(Videomanager,4);
				SaveSystemFile(4,SeriData->life,SeriData->SetLife,SeriData->score,SeriData->addpower);
			}
			return 0;
		case 9:
				PlayCD(STAGE3MUSIC);
				tempcount = 0;
				ObjectClear();
				AllDelete();
				ReSetShowTime();
//				mapmanager->initBackground(".\\data\\stage\\stage3-e.map");
//				ReadText(".\\data\\stage\\stage3-e.ses");
				sprintf(GetFilePath,"%sdata\\stage\\stage3-e.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stage3-e.ses",RunPath);
				ReadText(GetFilePath);
				mapmanager->Noscroll = TRUE;

				InsertObject(new ENEMYwater(0,0));
				GetInitObject();
				SeriData->gameend = 0;
				count = 5;
			return 0;

	}
	return 0;
}


/// STAGE 4
int inGameLoop4(void)
{
	switch(count)
	{
		case 0:
			ShowStageNum(SeriData->stage,SeriData->dashstage);
			SeriData->Display();
			ActionObject();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			PutSprites();
			if(Df_GetInputState(VK_ESCAPE) == _VKEY_ONE_DOWN)
			{
				mainproc2 = inGameLoop4;
				mainproc = PauseGame;

			}
			
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);

			if(forstage1 == 0)
			{
				if(Seripoint->x>=6080 && Seripoint->y < 672) //9//////////////////////////////////////////////////////////////////////////
				{
					count = 3;
					tempcount = 0;
					Seripoint->SendState(SEND_GETOLDSTATUS);
				}
			} else 
			if(forstage1 == 1)
			{
				if(Seripoint->x<=100 && Seripoint->y < 384) //9//////////////////////////////////////////////////////////////////////////
				{
					count = 3;
					tempcount = 0;
					Seripoint->SendState(SEND_GETOLDSTATUS);
				}
			}
/*
			if(Df_GetInputState(VK_F8) == _VKEY_ONE_DOWN)
			{
				count = 3;
				tempcount = 0;
				Seripoint->SendState(SEND_GETOLDSTATUS);
			}
*/
			if(SeriData->gameend == 2) count = 2;		//다시 시작
			else if(SeriData->gameend == 1) 
			{
				PlayCD(11);
				count = 1;
				ObjectClear();
				AllDelete();
			}
			return 0;
		case 1:
			{
				int i = ContinueProc();
				switch(i)
				{
					case 2:			// Restart;
						count = 2;
						SeriData->ResetSeriData();
						break;
					case 3:			// Game over;
						count = 0;
						quitprocess();
						mainproc = TitleCreate;
						SeriData->ResetSeriData(1);
						return 1;
				}
			}
			return 0;

		case 2:
			count =0;
			ObjectClear();
			AllDelete();
			if(forstage1)
			{
				ReSetShowTime();
//				mapmanager->initBackground(".\\data\\stage\\stage4n-2.map");
//				ReadText(".\\data\\stage\\stage4n-2.ses");
				sprintf(GetFilePath,"%sdata\\stage\\stage4n-2.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stage4n-2.ses",RunPath);
				ReadText(GetFilePath);
				SeriData->SetStage(4,2);
			}
			//forstage1 = 0;
			//SeriData->SetStage(1,2);
			else
			{
				ReSetShowTime();
//				mapmanager->initBackground(".\\data\\stage\\stage4n-1.map");
//				ReadText(".\\data\\stage\\stage4n-1.ses");
				sprintf(GetFilePath,"%sdata\\stage\\stage4n-1.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stage4n-1.ses",RunPath);
				ReadText(GetFilePath);
				SeriData->SetStage(4,1);
			}
			GetInitObject();
			SeriData->gameend = 0;
			return 0;

		case 3:
			if(tempcount++>6)
			{
				forstage1++;
				if(forstage1 == 1)
				{
					count = 0;
					PlayCD(STAGE4MUSIC);
					ObjectClear();
					AllDelete();
					SeriData->SetStage(4,2);
//					mapmanager->initBackground(".\\data\\stage\\stage4n-2.map");
//					ReadText(".\\data\\stage\\stage4n-2.ses");
					sprintf(GetFilePath,"%sdata\\stage\\stage4n-2.map",RunPath);
					mapmanager->initBackground(GetFilePath);
					sprintf(GetFilePath,"%sdata\\stage\\stage4n-2.ses",RunPath);
					ReadText(GetFilePath);

					GetInitObject();
					SeriData->gameend = 0;
					Seripoint->SendState(SEND_SETOLDSTATUS);
					ReSetShowTime();
				} else
				{
					count = 5;
					PlayCD(3);
				}
			} else Videomanager->AddSCREENEFFECT(1,_HALFSCREEN,1);

			return 0;

		case 5:  // MINIGAME
			{
				if(Df_GetInputState(VK_ESCAPE) == _VKEY_ONE_DOWN)
				{
					mainproc2 = inGameLoop4;
					mainproc = PauseGame;
					mainproc3 = DestroyBombstage;
				}
				int rests = Bomb();
				if(rests)
				{
					if(rests == 3)
					{
						count =0;
						quitprocess();
						mainproc = TitleCreate;
						return 1;
					}
					if(rests == 2)  // Clear
					{

						count = 6;
					}
				}
			}
			return 0;

		case 6:

			if(StageClearScreen())
			{
				count = 0;
				mainproc3 = tempvoid;
				quitprocess();
				InitSGame(Videomanager,5);
				SaveSystemFile(5,SeriData->life,SeriData->SetLife,SeriData->score,SeriData->addpower);
			}
			return 0;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
//
int inGameLoop5(void)
{
	switch(count)
	{
		case 0:
			{
				int ress = loops();
				if(ress == 0xFF)
				{
					tempcount = 0;
					count = 0;
					ObjectClear();
					AllDelete();
					quitprocess();
					InitSGame(Videomanager,6);
					SaveSystemFile(6,SeriData->life,SeriData->SetLife,SeriData->score,SeriData->addpower);
//					ShowCursor(FALSE);
				}
				if(Df_GetInputState(VK_ESCAPE) == _VKEY_ONE_DOWN)
				{
					mainproc2 = inGameLoop5;
					mainproc = PauseGame;
					mainproc3 = DestroyMiniGolfFiles;
				}
/*
				if(Df_GetInputState(VK_F8) == _VKEY_ONE_DOWN)
				{
					tempcount = 0;
					count = 0;
					ObjectClear();
					AllDelete();
					quitprocess();
					InitSGame(Videomanager,6);
					SaveSystemFile(6,SeriData->life,SeriData->SetLife,SeriData->score,SeriData->addpower);
				}
*/
			}
		break;
		case 1:
			count = 0;
		break;
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////
//
int inGameLoop6(void)
{

	switch(count)
	{
		case 0:
			ShowStageNum(SeriData->stage,SeriData->dashstage);
			SeriData->Display();
			ActionObject();
			GetObject(mapmanager->layer2.x,mapmanager->layer2.y);
			PutSprites();
			if(Df_GetInputState(VK_ESCAPE) == _VKEY_ONE_DOWN)
			{
				mainproc2 = inGameLoop6;
				mainproc = PauseGame;
			}
	
			if(Seripoint->x>=9200) //9//////////////////////////////////////////////////////////////////////////
			{
				count = 3;
				tempcount = 0;
				Seripoint->SendState(SEND_GETOLDSTATUS);
			}
/*
			if(Df_GetInputState(VK_F8) == _VKEY_ONE_DOWN)
			{
				count = 3;
				tempcount = 0;
				Seripoint->SendState(SEND_GETOLDSTATUS);
			}
*/
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);

			if(SeriData->gameend == 10) count = 6;
			if(SeriData->gameend == 2) count = 2;		//다시 시작
			else if(SeriData->gameend == 1) 
			{
				PlayCD(11);
				count = 1;
				ObjectClear();
				AllDelete();
			}
			return 0;
		case 1:
			{
				int i = ContinueProc();
				switch(i)
				{
					case 2:			// Restart;
						count =2;
						SeriData->ResetSeriData();
						break;
					case 3:			// Game over;
						count =0;
						quitprocess();
						mainproc = TitleCreate;
						SeriData->ResetSeriData(1);
						return 1;
				}
			}
			return 0;

		case 2:

			count = 0;
			ObjectClear();
			AllDelete();
			if(forstage1 == 1)
			{
				PlayCD(STAGE6MUSIC);
				SeriData->SetStage(6,2);
				ReSetShowTime();
//				mapmanager->initBackground(".\\data\\stage\\stage6n-2.map");
//				ReadText(".\\data\\stage\\stage6n-2.ses");
				sprintf(GetFilePath,"%sdata\\stage\\stage6n-2.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stage6n-2.ses",RunPath);
				ReadText(GetFilePath);


			} 
			else 
			if(forstage1 == 0)
			{
				PlayCD(STAGE6MUSIC);
				forstage1 = 0;
				SeriData->SetStage(6,1);
				ReSetShowTime();
//				mapmanager->initBackground(".\\data\\stage\\stage6n-1.map");
//				ReadText(".\\data\\stage\\stage6n-1.ses");
				sprintf(GetFilePath,"%sdata\\stage\\stage6n-1.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stage6n-1.ses",RunPath);
				ReadText(GetFilePath);

			} else 
			if(forstage1 == 2)
			{
				SeriData->SetStage(6,3);
				ReSetShowTime();
				PlayCD(FINALMUSIC);
//				mapmanager->initBackground(".\\data\\stage\\stagefinal.map");
//				ReadText(".\\data\\stage\\stagefinal.ses");
				sprintf(GetFilePath,"%sdata\\stage\\stagefinal.map",RunPath);
				mapmanager->initBackground(GetFilePath);
				sprintf(GetFilePath,"%sdata\\stage\\stagefinal.ses",RunPath);
				ReadText(GetFilePath);
				
			} 
			GetInitObject();
			if(forstage1 == 2) InsertObject(new DarkSeriObject(640,200));
			SeriData->gameend = 0;
			return 0;

		case 3:
			if(tempcount++>6)
			{
				forstage1++;
				count = 0;
				ObjectClear();
				AllDelete();
				if(forstage1 == 0)
				{
					PlayCD(STAGE6MUSIC);
					SeriData->SetStage(6,1);
//					mapmanager->initBackground(".\\data\\stage\\stage6n-2.map");
//					ReadText(".\\data\\stage\\stage6n-2.ses");
					sprintf(GetFilePath,"%sdata\\stage\\stage6n-1.map",RunPath);
					mapmanager->initBackground(GetFilePath);
					sprintf(GetFilePath,"%sdata\\stage\\stage6n-1.ses",RunPath);
					ReadText(GetFilePath);
				} else
				if(forstage1 == 1)
				{
					PlayCD(STAGE6MUSIC);
					SeriData->SetStage(6,2);
//					mapmanager->initBackground(".\\data\\stage\\stage6n-2.map");
//					ReadText(".\\data\\stage\\stage6n-2.ses");
					sprintf(GetFilePath,"%sdata\\stage\\stage6n-2.map",RunPath);
					mapmanager->initBackground(GetFilePath);
					sprintf(GetFilePath,"%sdata\\stage\\stage6n-2.ses",RunPath);
					ReadText(GetFilePath);
				} else
				if(forstage1 == 2)
				{
					PlayCD(FINALMUSIC);
					SeriData->SetStage(6,3);
//					mapmanager->initBackground(".\\data\\stage\\stagefinal.map");
//					ReadText(".\\data\\stage\\stagefinal.ses");
					sprintf(GetFilePath,"%sdata\\stage\\stagefinal.map",RunPath);
					mapmanager->initBackground(GetFilePath);
					sprintf(GetFilePath,"%sdata\\stage\\stagefinal.ses",RunPath);
					ReadText(GetFilePath);
				}
				GetInitObject();
				if(forstage1 == 2) 
				{
					InsertObject(new DarkSeriObject(640,200));
					count = 7;
				}
				SeriData->gameend = 0;
				if(Seripoint != NULL) Seripoint->SendState(SEND_SETOLDSTATUS);
				ReSetShowTime();

			} else Videomanager->AddSCREENEFFECT(1,_HALFSCREEN,1);

			return 0;

		case 6:
			if(StageClearScreen())
			{
				count = 8;
			}
			return 0;

		case 7:		/// BOSS 첨 만났을때
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			PutSprites();
			if(Event(5))
			{
				count = 20;
			}
			return 0;
		case 8:		/// BOSS 죽었을때
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			PutSprites();
			if(Event(6))
			{
				count = 9;
				quitprocess();
				forstage1 = 0;
			}
			return 0;
		case 9:
			if(forstage1++>6) 
			{
				count = 10; 
				DropDarkSeriFile();
			}
			else
			Videomanager->AddSCREENEFFECT(0,_HALFSCREEN,1);
			return 0;
		case 10:
			if(Ending())
			{
				count = 11;
			}
			return 0;
		case 11:
			if(Staffvoid())
			{
				count =0;
				mainproc = TitleCreate;
			}
			return 0;

		case 20:
			SeriData->Display();
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			stHelp = new pIMAGE();
			stHelp->Load(NULL,MiniROM.SetFilePosition("tip5.spr"));
			count = 21;
			stfade = 0;
			break;

		case 21:
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			Videomanager->AddSpr(36,76,stHelp,_FADEMODE,128,stfade);
			stfade++;
			if(stfade>30) count = 22;
			break;

		case 22:
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			Videomanager->AddSpr(36,76,stHelp,_NORMALMODE,128);
			if(Df_GetInputState(VK_SPACE) || Df_GetInputState(VK_RETURN) || Df_GetInputState(VK_LBUTTON)) count = 23;
			break;

		case 23:
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			Videomanager->AddSpr(36,76,stHelp,_FADEMODE,128,stfade);
			stfade--;
			if(stfade<1) count = 24;
			break;
		case 24:
			mapmanager->iMessage(_ID_SHOW,NULL,1,1);
			delete stHelp;
			mapmanager->ForceScroll = 1;
			count = 0;
			break;

	}
	return 0;

}


void ExitGameLoop2(void)
{
	ExitGameLoop();
	DestroyMiniGolfFiles();
}
/////////////////////////////////////////////////////////////////
//
//
void ExitGameLoop(void)
{
	delete (mapmanager);
	DestroySprite(_ITEMSTAR);
	DestroySprite(_SERI);
	DestroySprite(_ENEMY_RULKER);
	DestroySprite(_SERI_SHOOT);
	DestroySprite(_ENEMY_FOX);
	DestroySprite(_ENEMY_CHIKEN);
	DestroySprite(_ENEMY_RAM);
	DestroySprite(_ENEMY_SLIM);
	DestroySprite(_STAR);
	DestroySprite(_ENEMY_MOUSE);
	DestroySprite(_ENEMY_EGG);
	DestroySprite(_ENEMY_SHOOT);
	DestroySprite(_ENEMY_GHOST);
	DestroySprite(_ENEMY_ROBO1);
	DestroySprite(_ENEMY_ROBO2);
	DestroySprite(_ENEMY_GOREA);
	DestroySprite(_ENEMY_PIG);
	DestroySprite(_ENEMY_BAKA);
	DestroySprite(_PPBOMB);
	DestroySprite(_PAPUN);
	DestroySprite(_STEPS);
	DestroySprite(_RAMP);
	DestroySprite(_DOOR);
	DestroySprite(_ITEM);
	DestroySprite(_ENEMY_ROBO3);
	StopCD();
	mainproc3();
}


void ExitGameLoop3(void)
{
	StopCD();
	mainproc3();
}
