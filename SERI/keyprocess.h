#ifndef __KEYPROCESS_H__
#define __KEYPROCESS_H__

#include"stdafx.h"

#define		_VKEY_NO_DOWN					0

#define		_VKEY_ONE_DOWN					2
#define		_VKEY_STILL_DOWN				3

class cKeyProcess 
{
	protected:
		
		BYTE  Keystateup[256];					// Ű�� �������ٰ� �����·� ���ư�����.

		BOOL  Keystate2[256];

	public:
		BYTE  Keystate[256];					// Ű ����
		int  GetVKeyState(int iskey);
		void GetKEYSateLoop(void);				// ������ ���ν����� �־� �ּ���.
		void ClearKeyState(void);				// Ű���۸� �ʱ�ȭ�Ѵ�.
		void GetWM_message(int key,BOOL state);
		POINT GetMousePos(void);
		cKeyProcess();
		~cKeyProcess();


	public:
		POINT MousePos;
		//BOOL  MouseInRECT(int left,int up,int right,int bottom); // ���콺�� �������� �ִ����� �˻�.
};


//BOOL  keydown = FALSE;
#endif 