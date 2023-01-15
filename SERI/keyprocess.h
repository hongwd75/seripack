#ifndef __KEYPROCESS_H__
#define __KEYPROCESS_H__

#include"stdafx.h"

#define		_VKEY_NO_DOWN					0

#define		_VKEY_ONE_DOWN					2
#define		_VKEY_STILL_DOWN				3

class cKeyProcess 
{
	protected:
		
		BYTE  Keystateup[256];					// 키가 눌러졌다가 원상태로 돌아갔을때.

		BOOL  Keystate2[256];

	public:
		BYTE  Keystate[256];					// 키 버퍼
		int  GetVKeyState(int iskey);
		void GetKEYSateLoop(void);				// 윈도우 프로시저에 넣어 주세요.
		void ClearKeyState(void);				// 키버퍼를 초기화한다.
		void GetWM_message(int key,BOOL state);
		POINT GetMousePos(void);
		cKeyProcess();
		~cKeyProcess();


	public:
		POINT MousePos;
		//BOOL  MouseInRECT(int left,int up,int right,int bottom); // 마우스가 범위내에 있는지를 검사.
};


//BOOL  keydown = FALSE;
#endif 