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

///////////////////  인형들 데이타 화일 정의 /////////////////

#define CHR_DISPLAY								 	 0
#define CHR_MOTIONCHANGE						 	 1

#define _MOTIONNUMBER								64
#define _MAXMOTIONFRAME							   128

#define _DOLLLAYER									40

////////////////////////////////////////////////////////////////////////////////////////
//						제어용
////////////////////////////////////////////////////////////////////////////////////////
#define _dolPROCESS									0	// 동작 움직이기.
#define _dolSHOW									1	// 화면에 출력
#define _dolINIT									3   // 초기화
#define _dolNEXTFRAME								4
#define _dolRESETMOTION								5   // 바로 동작을 변경
#define _dolSHOW_ALPHA								7   // 알파 먹여서 출력

#define	_RESETFRAME									6   // 프레임이 끝까지 갔었고, 다시 리셋 되었다

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
		int		Imagenum;							 // 이미지수
		int     Motionnum;							 // 동작모음수
		pIMAGE  *motionimage[128];					 // 그림들어간곳.

		int      framesize[_MOTIONNUMBER];			 // 몇 프레임인가.
		DOLLINFO *frameinfo[_MOTIONNUMBER];          // 프레임 정보가 가득~ 우헤헤헤헤헤헤


		DOLLINFO *nframeinfo;						 // frameinfo에서 정보를 얻어서 가지고 있는다.
		int      lengthframe;						 // framesize에서 정보를 얻어서 가지고 있는다.


		ImagePorcessClass		*mDisplay;	

		int     iframe;								 // 지금 프레임
		int     Delaycount;							 // 프레임 지연 카운터

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