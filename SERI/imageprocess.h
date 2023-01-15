////////////////////////////////////////////////////////////////////////////////////////////////
// ��ũ�� ����Ʈ �̿��� �̹��� ó���� ���μ���
// ���̾� �������� �� �̹��� ���� ���� �Ѽ��ֵ�.
// �޸� �����ڰ� �ʿ�������...

#ifndef __IMAGEPROCESS_H__
#define __IMAGEPROCESS_H__

#include"stdafx.h"
#include"video.h"
#include<stdlib.h>

// Process Command HIWORD

// ���ڿ� ���� ��� 
#define	  INCLUDECHARSIZE		0001		// ������ ũ�⸦ ������ ������
#define   INCLUDECHARBKCOLOR	0002		// ��Ʈ���� ������ ����
#define   INCLUDECHARSHADOW		0004		// ���ڹؿ� �˻����� �ѹ� �� �׷��ش�.


// Process Command LOWWORD
//#define DRAWANDKILL				0001		// �׸� �� �̹��� Ŭ���� �Ҹ� 
#define _ALPHAMODE				0x0002		// ���ĺ귻������ �׸���.
#define _FADEMODE				0x0005		// ���̵� ��/�ƿ�~ �̹���
#define _DARKMODE				0x0003		// ���İ��� ������ �ܰ踦 ǥ��
#define _HALFTONEMODE			0x0004		// ������ �̹��� ���
#define _DHALFTONEMODE			0x0006		// 2�� ������ �̹��� ���
#define _NORMALMODE				0x0001		// �Ϲ������� ����ش�.
#define _ANTIMODE				0x0007		// ��Ƽ �Կ��� ����ش�
#define _LIGHTMODE				0x0009		// ����Ʈ ���
#define _POLYMODE				0x000a
#define _CLIPWIDTH				0x000b

#define _HALFSCREEN				0x00f0      // ȭ����ü�� ���������� ���� �ִ´�.
											// ȿ���� ���� ���Ǵ� HIWORD�� ����ȴ�.
#define TEXTDRAWMODE			0008		// ���� �ֱ�.
											// Ȯ��� ������ HIWORD�� ���ǵǾ� ����.


// ���̾� ����.
#define _BACKGROUNDLayer		   0		// ��� ���̾�
#define _NormalTextDrawLayer	  10		// �Ϲ����� ���� ���̾� 
#define _OverLayer				 999		// �ֻ��� ���̾�


// ��Ÿ ���ǹ�.
#define _MaxProcess				 1024		// ���� �޸� ũ��


#define _ID_CHAR				   1		// ��Ʈ�� ID
#define _ID_IMAGE				   2		// �̹��� ID
#define _ID_SPR					   3		// ��������Ʈ ID
#define _ID_SPR2				   4		// ����� ��������Ʈ ID(�ڵ�����, ���������� ���δ�.)	
#define _ID_SCREEN				   5		// ��ũ����ü�� ȿ���� ������ 
#define _ID_POLY				   6		// Ȯ�� ���



struct tagImageList 
{
	LONG	ProcessCommand;					// ���� define�Ȱ͵� �ִ°�.
	int		ID;								// �̹���,����,��Ÿ���
	int		LayerLevel;						// ���̾������	
	void	*LPImageClass;					// �̹���Ŭ���� ������ 
	BYTE	Parameters[512];				// �ɼǿ� ����.
	struct	tagImageList *next;				// ���� ���� ���� ����..
};



class ImagePorcessClass
{
		private:
			int ImageCount;							// ������ �޸𸮸� ������ Ÿ����.
			struct tagImageList *Header,*Tail;
			struct tagImageList *mBasememory[_MaxProcess];	// �޸𸮸� �̸� ��� ��..

			

		private:
			void AddList(int x,int y,int options,int layers,int ID,void *data,int op1,int op2);

		public:

			clSCREEN *VIDEO;

			ImagePorcessClass(clSCREEN *vv);		// �̹��� ó�� ���μ��� ����
			~ImagePorcessClass();					// �Ҹ�

			void InitImageList(void);				// �̹��� ����Ʈ �ʱ�ȭ.

			void DrawImageList(void);

		public:
			void AddPoly(int x,int y,void *data,int sized,int rote,int layers);
			void AddSpr(int x, int y, void *data, int imageoption, int layers,int op=0);
			void AddSCREENEFFECT(int data, int imageoption, int layers);
			void AddImage(int x, int y, void *data, int imageoption, int layers);
			void AddText(int x,int y,int color,LPSTR message,int CHARoption,int layers,int OP1=0);
};


#endif
