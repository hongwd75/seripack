#ifndef __DEFINEHEADER_H__
#define __DEFINEHEADER_H__

#include"keyprocess.h"
#include"filemanager.h"
//#include"audio.h"

extern cKeyProcess   cInUseKeyClass;

#define		Df_InitInputProcess()		cInUseKeyClass.ClearKeyState()		// 초기화.
#define     Df_CatchInputState()		cInUseKeyClass.GetKEYSateLoop()		// Loop문에 꼬옥 넣어주세욧
#define     Df_ClearInputState()		cInUseKeyClass.ClearKeyState()		// 초기화 2
#define		Df_GetInputState(a)			cInUseKeyClass.GetVKeyState(a)	    // 위에  define된 값을 리턴한다.
#define     GetWM_message(a,b)			cInUseKeyClass.GetWM_message(a,b)
#define     Df_GetMousePos()			cInUseKeyClass.GetMousePos()		// 마우스 위치 알아내기


BOOL ImageClicked(int x,int y,POINT mpos,pIMAGE *img,WORD biN);  // 마우스가 현위치에 있는지 알아 본다

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//extern void (*mainproc)(void);
extern int (*mainproc)(void);
extern WORD	biNcolor;
extern char RunPath[MAX_PATH];
extern char GetFilePath[MAX_PATH];
void InitSGame(ImagePorcessClass *cdisplay,int stagebk);
int inGameLoop(void);
void ExitGameLoop(void);


void CloseMid(void);
void PlayMid(void);
void LoadMid(LPSTR filename);
void OpenCD();
void StopCD();
void PlayCD(int track);
void CloseCD(void);



void OpenSystemFile(void);
void SaveSystemFile(int stage,int life,int maxlife,int score,int addpower);
void initSystemFile(void);

////////////////// maintitle.cpp
int TitleProc(void);
int TitleCreate(void);


#endif