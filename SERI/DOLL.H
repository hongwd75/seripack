#ifndef __DOLLCLASS_H__
#define __DOLLCLASS_H__
#include"stdafx.h"
#include"imageprocess.h"
#include"filemanager.h"


#define _STAND_R							0
#define _STAND_L							1
#define _WALK_R								2
#define _WALK_L								3
#define _RUN_R								4
#define _RUN_L								5
#define _JUMP_R								6
#define _JUMP_L								7
#define _JUMPATTACK_R						8
#define _JUMPATTACK_L						9
#define _ATTACK1_R							10
#define _ATTACK1_L							11
#define _ATTACK2_R							12
#define _ATTACK2_L							13
#define _FALLING_R							14
#define _FALLING_L							15
#define _DAMAGE_LAND_R						16
#define _DAMAGE_LAND_L						17
#define _DAMAGE_SKY_R						18
#define _DAMAGE_SKY_L						19
#define _DIE								20

///////////////////  ������ ����Ÿ ȭ�� ���� /////////////////

#define CHR_DISPLAY								 	 0
#define CHR_MOTIONCHANGE						 	 1

#define _MOTIONNUMBER								64
#define _MAXMOTIONFRAME							   128

#define _DOLLLAYER									40

////////////////////////////////////////////////////////////////////////////////////////
//						�����
////////////////////////////////////////////////////////////////////////////////////////
#define _dolPROCESS									0	// ���� �����̱�.
#define _dolSHOW									1	// ȭ�鿡 ���
#define _dolINIT									3   // �ʱ�ȭ
#define _dolNEXTFRAME								4
#define _dolRESETMOTION								5   // �ٷ� ������ ����
#define _dolSHOW_ALPHA								7   // ���� �Կ��� ���

#define	_RESETFRAME									6   // �������� ������ ������, �ٽ� ���� �Ǿ���

////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	int x;
	int y;
	int layer;
	int imagetype;
} tagDollviewstate;

typedef struct 
{
	int Imagenumber;
	int delay;
	POINT mypos;
} DOLLINFO;


class mChar
{
	public:
		int		Imagenum;							 // �̹�����
		int     Motionnum;							 // ���۸�����
		pIMAGE  *motionimage[128];					 // �׸�����.

		int      framesize[_MOTIONNUMBER];			 // �� �������ΰ�.
		DOLLINFO *frameinfo[_MOTIONNUMBER];          // ������ ������ ����~ ��������������


		DOLLINFO *nframeinfo;						 // frameinfo���� ������ �� ������ �ִ´�.
		int      lengthframe;						 // framesize���� ������ �� ������ �ִ´�.


		ImagePorcessClass		*mDisplay;	

		int     iframe;								 // ���� ������
		int     Delaycount;							 // ������ ���� ī����

		POINT	Worldpoint;

	public:
		mChar(ImagePorcessClass *mm);
		mChar(ImagePorcessClass *mm,LPSTR filename);
		~mChar();

	public:
		int   iSendMessage(int ID,void *message,long LPARAM,long RPARAM);
		int   GetMotionFrame();
		POINT GetImageSize();
		POINT GetMotionNumber();
		void  mImageLoad(LPSTR filename);
		void  mImageLoad(FILE *fp);
		void  SetWorldpoint(int x,int y);
		void  AddSprWorldPos(POINT position,int sprnumber,int layer=_DOLLLAYER,int STATE=_NORMALMODE);
};

#endif