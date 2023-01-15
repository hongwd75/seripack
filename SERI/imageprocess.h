////////////////////////////////////////////////////////////////////////////////////////////////
// 링크드 리스트 이용한 이미지 처리용 프로세서
// 레이어 생성으로 각 이미지 마다 층을 둘수있따.
// 메모리 관리자가 필요할지도...

#ifndef __IMAGEPROCESS_H__
#define __IMAGEPROCESS_H__

#include"stdafx.h"
#include"video.h"
#include<stdlib.h>

// Process Command HIWORD

// 문자용 제어 명령 
#define	  INCLUDECHARSIZE		0001		// 문자의 크기를 가지고 있을때
#define   INCLUDECHARBKCOLOR	0002		// 스트링이 배경색을 정의
#define   INCLUDECHARSHADOW		0004		// 글자밑에 검색으로 한번 더 그려준다.


// Process Command LOWWORD
//#define DRAWANDKILL				0001		// 그린 후 이미지 클래스 소멸 
#define _ALPHAMODE				0x0002		// 알파브렌딩으로 그린다.
#define _FADEMODE				0x0005		// 페이드 인/아웃~ 이미지
#define _DARKMODE				0x0003		// 알파값을 가지고 단계를 표현
#define _HALFTONEMODE			0x0004		// 반투명 이미지 찍기
#define _DHALFTONEMODE			0x0006		// 2배 반투명 이미지 찍기
#define _NORMALMODE				0x0001		// 일반적으로 찍어준다.
#define _ANTIMODE				0x0007		// 안티 먹여서 찍어준다
#define _LIGHTMODE				0x0009		// 라이트 모드
#define _POLYMODE				0x000a
#define _CLIPWIDTH				0x000b

#define _HALFSCREEN				0x00f0      // 화면전체에 반투명으로 색을 넣는다.
											// 효과에 관한 정의는 HIWORD에 저장된다.
#define TEXTDRAWMODE			0008		// 문자 넣기.
											// 확장된 정보는 HIWORD에 정의되어 있음.


// 레이어 구분.
#define _BACKGROUNDLayer		   0		// 배경 레이어
#define _NormalTextDrawLayer	  10		// 일반적인 문자 레이어 
#define _OverLayer				 999		// 최상위 레이어


// 기타 정의문.
#define _MaxProcess				 1024		// 가상 메모리 크기


#define _ID_CHAR				   1		// 스트링 ID
#define _ID_IMAGE				   2		// 이미지 ID
#define _ID_SPR					   3		// 스프라이트 ID
#define _ID_SPR2				   4		// 압축된 스프라이트 ID(자동검출, 내부적으로 쓰인다.)	
#define _ID_SCREEN				   5		// 스크린전체에 효과를 넣을때 
#define _ID_POLY				   6		// 확대 축소



struct tagImageList 
{
	LONG	ProcessCommand;					// 위에 define된것들 넣는것.
	int		ID;								// 이미지,문자,기타등등
	int		LayerLevel;						// 레이어관리용	
	void	*LPImageClass;					// 이미지클래스 포인터 
	BYTE	Parameters[512];				// 옵션용 버퍼.
	struct	tagImageList *next;				// 다음 다음 다음 다음..
};



class ImagePorcessClass
{
		private:
			int ImageCount;							// 버퍼의 메모리를 쓸때의 타운터.
			struct tagImageList *Header,*Tail;
			struct tagImageList *mBasememory[_MaxProcess];	// 메모리를 미리 잡아 둠..

			

		private:
			void AddList(int x,int y,int options,int layers,int ID,void *data,int op1,int op2);

		public:

			clSCREEN *VIDEO;

			ImagePorcessClass(clSCREEN *vv);		// 이미지 처리 프로세서 생성
			~ImagePorcessClass();					// 소멸

			void InitImageList(void);				// 이미지 리스트 초기화.

			void DrawImageList(void);

		public:
			void AddPoly(int x,int y,void *data,int sized,int rote,int layers);
			void AddSpr(int x, int y, void *data, int imageoption, int layers,int op=0);
			void AddSCREENEFFECT(int data, int imageoption, int layers);
			void AddImage(int x, int y, void *data, int imageoption, int layers);
			void AddText(int x,int y,int color,LPSTR message,int CHARoption,int layers,int OP1=0);
};


#endif
