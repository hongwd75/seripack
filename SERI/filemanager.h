#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include"stdafx.h"
#include<stdlib.h>
#include<stdio.h>

typedef struct 
{
	int CardHave[15];			// �������� 1 ������ 0
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
		int  Items;							// �󸶳� ������ �ִ°�.
		FILE *file;							// ȭ��������

		char	ROMFile[_MAX_PATH];			// ȭ�� �̸� ����

		tagFileIndex DATA[MAXFILESIZE];     // ����Ÿ ���� ��

		BOOL Init(LPSTR filename);			// LST file�� ���� 
		BOOL Close(void);					// LST file�� �ݴ´�,

		clFileManager(void);		// �ʱ�ȭ 
		~clFileManager(void);

	public:
		FILE *SetFilePosition(LPSTR filename);  // LSTȭ�Ͽ��� ȭ���̸��� ã��,
												// DATAȭ���� ȭ�� ��ġ�� �̵� ������ ���� 
		FILE *SetFilePosition(int index);		// LSTȭ�Ͽ��� ��ȣ�� ã�´�.

		HMMIO SetFilePositionMMF(LPSTR filename);  // WAVȭ�Ͽ� ȭ����ġã��
};

#endif