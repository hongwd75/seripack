#include <windows.h>
#include"stdafx.h"
#include"keyprocess.h"

cKeyProcess cInUseKeyClass;
//////////////////////////////////////////////////////////////////
// Ŭ���� ����, �Ҹ�.
//
cKeyProcess::cKeyProcess()
{
	this->ClearKeyState();
}

cKeyProcess::~cKeyProcess()
{

}




//////////////////////////////////////////////////////////////////
// Ű���� �ʱ�ȭ
//
void cKeyProcess::ClearKeyState(void)
{
	memset(this->Keystate,0,255);
	memset(this->Keystateup,0,255);
	memset(this->Keystate2,0,255);
	SetKeyboardState(Keystate);
}



//////////////////////////////////////////////////////////////////
// Ű���� ���¾˾Ƴ���.
//
int  cKeyProcess::GetVKeyState(int iskey)
{
	if((this->Keystate[iskey] & 0x80) && !(this->Keystateup[iskey] & 0x80)) return _VKEY_ONE_DOWN;
	else
		if((this->Keystate[iskey] & 0x80) && (this->Keystateup[iskey] & 0x80)) return _VKEY_STILL_DOWN;
		else 
			return _VKEY_NO_DOWN;


}



void  cKeyProcess::GetKEYSateLoop(void)			// ������ ���ν����� �־� �ּ���.
{
	memcpy(Keystateup,Keystate,256);
	GetKeyboardState(Keystate);
	Keystate[144] = 0;
	GetCursorPos(&MousePos);
}

void cKeyProcess::GetWM_message(int key, BOOL state)
{
	this->Keystate2[key] = state;
}


POINT cKeyProcess::GetMousePos(void)
{
	return this->MousePos;
}