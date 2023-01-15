#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include"stdafx.h"
#include"imageprocess.h"
#include<stdlib.h>

// 레이어별 타일 개수
#define                 LAYER1SIZE				256
#define					LAYER2SIZE				256
#define                 LAYER3SIZE				256

// 레이어 2에 따른 1,3의 증감치 
#define					LAYER1ADD				2
#define                 LAYER2ADD				11			// 표준용
#define					LAYER3ADD				2		

// Init Error
#define                 _MAPREADY				0			// 읽기 성공 
#define					_MAPFILENOTFOUND		1			// 화일 못 찾았어 T_T
#define                 _MAPFILEERROR			2			// 맵화일이 이상해..

// Message ID
#define                 _ID_SCROLL_SET			100

#define					_ID_SCROLL_LEFT			1
#define                 _ID_SCROLL_RIGHT		2
#define                 _ID_SCROLL_FORCE_RIGHT	64
#define                 _ID_SCROLL_UP			3
#define                 _ID_SCROLL_DOWN			4
#define                 _ID_SHOW				5
#define                 _ID_SHOW2				11			// 파이터에서 쓸꺼다
#define					_ID_MAP_STATE			6

#define                 _ID_CHAR_MOVE_LEFT		7
#define                 _ID_CHAR_MOVE_RIGHT		8
#define                 _ID_CHAR_MOVE_UP		9
#define                 _ID_CHAR_MOVE_DOWN		10

// MAP STATE return
#define					_RET_STAND				1

typedef struct
{
	float x,y;
} floatPOINT;

//////////////////////////////////////////////////////////////
class CLBackground
{
		public:

			BOOL	ForceScroll;				// 강제 스크롤이냐
			BOOL	Noscroll;					// 옆으로 스크롤 없다
			int     Scrollcount;				// 카운터
			int		AddLayer1;

			int		width[4];					// 레이어별 가로 
			int		height[4];					// 레이어별 세로
			POINT   tilesize[4];				// 레이어별 타일 크기
			BOOL	maploaded;					// 맵이 로드 되었수다.

			pIMAGE  *iLayer1[256];				//	레이어 1 맵 데이타 
			pIMAGE  *iLayer2[256];				//  레이어 2 맵 데이타 
			pIMAGE  *iLayer3[256];				//  레이어 3 맵 데이타

			BYTE    *mlayer1;					// 레이어 1
			BYTE	*mlayer2;					// 레이어 2
			BYTE    *mlayer3;					// 레이어 3
			BYTE    *mapattribute;				// 맵 속성 데이타

			floatPOINT L1,L3;

			int ctx,cty,px,py,addy,addx,sttx,stty;			// 레이어1용 처리 변수들 
			int ctx1,cty1,px1,py1,addy1,addx1,sttx1,stty1;  // 레이어3용 처리 변수들 

			ImagePorcessClass *iDisplay;

		public:
			POINT	layer2;

			CLBackground(ImagePorcessClass *dds);
			~CLBackground(void);

			void Clearmem();

			int initBackground(LPSTR filename);

			int iMessage(int mID,void*mMessage,int LParam,int RParam);

			BYTE GetMapAttribute(int x,int y);
};



/////////////////////////////////////////////////////////////////////////////////////////
//		세리 메니져																	   //
/////////////////////////////////////////////////////////////////////////////////////////
#define FRAMESIZE								   10
#define SERIMAXENEGY							   59
class CLSeriManager
{
	public:

		int   life;									// 목숨이 얼마나 있는가 
		int   addpower;								// 파워업
		int   itemsize;								// 몇개의 아이탬을 가지고 있는가

		int   score;								// 점수
		int	  stage;								// 몇 스테이지 인가.
		int   dashstage;
		int   gameend;								// 0이면 계속 진행, 1 : 죽었다, 2: 클리어

		int   Item[5];								// 가지고 있는 아이템들 
		int   ItemImage[5];

		//int   Gamelevel;

		pIMAGE *iSerilife;
		pIMAGE *iImage[5];							// 이미지저장소 
		pIMAGE *ibackboard;							// 빽보드
		pIMAGE *Gage;
		pIMAGE *iNumber[11];
		ImagePorcessClass *iDisplay;				// 디스플레이 포인터 

		int SetLife;								// 게임 시작시 세리머리수
		int SetSPEED;								// 게임 속도

	public:
	  CLSeriManager(ImagePorcessClass *dds);			// 만든다
	  ~CLSeriManager(void);								// 죽인다.

	  void init(int rlife,int ritemsize,int rscore,int level);	// 초기화 
	  void Loadinit(void);
	  void ResetSeriData(int i = 0);						// 리셋 (1:판까지 1로..)
	  void Load(void);									// 이미지 로드 
	  void Destroys(void);								// 죽어버려!!

	  void Display(void);								// 화면에 때린다.

	  int UsesCard(int hu);							// 카드 쓴다

	  void SetStage(int ii,int ii2=0);							// 스테이지 설정 
	  void RepiarEnegy(int ss);							// 에너지 추가
	  void SetSeriNum(int ss);							// 세리 마리수 수정
	  void ShowSerilife(int x,int y);

	  void ShowScore(int x, int y);
	  void ShowStageNum(int x, int y);
	  void SetGemeSpeed(void);
	  void ShowSeriGage(int i);
	  void ResetCardSystem(void);
	  void resetjumsu(void);
	  int  killed;
	  int  many;
	  int  eat;
};

#endif