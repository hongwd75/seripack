#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include"stdafx.h"
#include"imageprocess.h"
#include<stdlib.h>

// ���̾ Ÿ�� ����
#define                 LAYER1SIZE				256
#define					LAYER2SIZE				256
#define                 LAYER3SIZE				256

// ���̾� 2�� ���� 1,3�� ����ġ 
#define					LAYER1ADD				2
#define                 LAYER2ADD				11			// ǥ�ؿ�
#define					LAYER3ADD				2		

// Init Error
#define                 _MAPREADY				0			// �б� ���� 
#define					_MAPFILENOTFOUND		1			// ȭ�� �� ã�Ҿ� T_T
#define                 _MAPFILEERROR			2			// ��ȭ���� �̻���..

// Message ID
#define                 _ID_SCROLL_SET			100

#define					_ID_SCROLL_LEFT			1
#define                 _ID_SCROLL_RIGHT		2
#define                 _ID_SCROLL_FORCE_RIGHT	64
#define                 _ID_SCROLL_UP			3
#define                 _ID_SCROLL_DOWN			4
#define                 _ID_SHOW				5
#define                 _ID_SHOW2				11			// �����Ϳ��� ������
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

			BOOL	ForceScroll;				// ���� ��ũ���̳�
			BOOL	Noscroll;					// ������ ��ũ�� ����
			int     Scrollcount;				// ī����
			int		AddLayer1;

			int		width[4];					// ���̾ ���� 
			int		height[4];					// ���̾ ����
			POINT   tilesize[4];				// ���̾ Ÿ�� ũ��
			BOOL	maploaded;					// ���� �ε� �Ǿ�����.

			pIMAGE  *iLayer1[256];				//	���̾� 1 �� ����Ÿ 
			pIMAGE  *iLayer2[256];				//  ���̾� 2 �� ����Ÿ 
			pIMAGE  *iLayer3[256];				//  ���̾� 3 �� ����Ÿ

			BYTE    *mlayer1;					// ���̾� 1
			BYTE	*mlayer2;					// ���̾� 2
			BYTE    *mlayer3;					// ���̾� 3
			BYTE    *mapattribute;				// �� �Ӽ� ����Ÿ

			floatPOINT L1,L3;

			int ctx,cty,px,py,addy,addx,sttx,stty;			// ���̾�1�� ó�� ������ 
			int ctx1,cty1,px1,py1,addy1,addx1,sttx1,stty1;  // ���̾�3�� ó�� ������ 

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
//		���� �޴���																	   //
/////////////////////////////////////////////////////////////////////////////////////////
#define FRAMESIZE								   10
#define SERIMAXENEGY							   59
class CLSeriManager
{
	public:

		int   life;									// ����� �󸶳� �ִ°� 
		int   addpower;								// �Ŀ���
		int   itemsize;								// ��� �������� ������ �ִ°�

		int   score;								// ����
		int	  stage;								// �� �������� �ΰ�.
		int   dashstage;
		int   gameend;								// 0�̸� ��� ����, 1 : �׾���, 2: Ŭ����

		int   Item[5];								// ������ �ִ� �����۵� 
		int   ItemImage[5];

		//int   Gamelevel;

		pIMAGE *iSerilife;
		pIMAGE *iImage[5];							// �̹�������� 
		pIMAGE *ibackboard;							// ������
		pIMAGE *Gage;
		pIMAGE *iNumber[11];
		ImagePorcessClass *iDisplay;				// ���÷��� ������ 

		int SetLife;								// ���� ���۽� �����Ӹ���
		int SetSPEED;								// ���� �ӵ�

	public:
	  CLSeriManager(ImagePorcessClass *dds);			// �����
	  ~CLSeriManager(void);								// ���δ�.

	  void init(int rlife,int ritemsize,int rscore,int level);	// �ʱ�ȭ 
	  void Loadinit(void);
	  void ResetSeriData(int i = 0);						// ���� (1:�Ǳ��� 1��..)
	  void Load(void);									// �̹��� �ε� 
	  void Destroys(void);								// �׾����!!

	  void Display(void);								// ȭ�鿡 ������.

	  int UsesCard(int hu);							// ī�� ����

	  void SetStage(int ii,int ii2=0);							// �������� ���� 
	  void RepiarEnegy(int ss);							// ������ �߰�
	  void SetSeriNum(int ss);							// ���� ������ ����
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