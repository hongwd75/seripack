#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include"stdafx.h"
#include<stdlib.h>
#include<stdio.h>

typedef struct 
{
	int CardHave[15];			// 가졌으면 1 없으면 0
} tagCollectionCard;


typedef struct
{
	tagCollectionCard CARD;
	int score;
	int addpower;
	int life;
	int maxlife;
	int stageswitch;
	int BattleCard[5];
} tagSeriRELOADSystem;


#define MAXFILESIZE			256
typedef struct
{
		char filename[15];
		long filepos;
} tagFileIndex;

class clFileManager
{
	public:
		int  Items;							// 얼마나 가지고 있는가.
		FILE *file;							// 화일포인터

		char	ROMFile[_MAX_PATH];			// 화일 이름 저장

		tagFileIndex DATA[MAXFILESIZE];     // 데이타 들어가는 곳

		BOOL Init(LPSTR filename);			// LST file을 연다 
		BOOL Close(void);					// LST file을 닫는다,

		clFileManager(void);		// 초기화 
		~clFileManager(void);

	public:
		FILE *SetFilePosition(LPSTR filename);  // LST화일에서 화일이름을 찾아,
												// DATA화일의 화일 위치로 이동 포인터 전달 
		FILE *SetFilePosition(int index);		// LST화일에서 번호로 찾는다.

		HMMIO SetFilePositionMMF(LPSTR filename);  // WAV화일용 화일위치찾기
};

#endif