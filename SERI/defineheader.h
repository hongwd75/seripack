#ifndef __DEFINEHEADER_H__
#define __DEFINEHEADER_H__

#include"keyprocess.h"
#include"filemanager.h"
//#include"audio.h"

extern cKeyProcess   cInUseKeyClass;

#define		Df_InitInputProcess()		cInUseKeyClass.ClearKeyState()		// �ʱ�ȭ.
#define     Df_CatchInputState()		cInUseKeyClass.GetKEYSateLoop()		// Loop���� ���� �־��ּ���
#define     Df_ClearInputState()		cInUseKeyClass.ClearKeyState()		// �ʱ�ȭ 2
#define		Df_GetInputState(a)			cInUseKeyClass.GetVKeyState(a)	    // ����  define�� ���� �����Ѵ�.
#define     GetWM_message(a,b)			cInUseKeyClass.GetWM_message(a,b)
#define     Df_GetMousePos()			cInUseKeyClass.GetMousePos()		// ���콺 ��ġ �˾Ƴ���


BOOL ImageClicked(int x,int y,POINT mpos,pIMAGE *img,WORD biN);  // ���콺�� ����ġ�� �ִ��� �˾� ����

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