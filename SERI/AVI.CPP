#include"stdafx.h"
#include<stdio.h>
//#include"defineheader.h"
extern char RunPath[MAX_PATH];
extern char GetFilePath[MAX_PATH];

MCI_ANIM_OPEN_PARMS		mciOpenParms;
MCI_PLAY_PARMS			mciPlayParms;
MCI_ANIM_WINDOW_PARMS	mciWindow;
MCI_ANIM_RECT_PARMS		mciRect;
UINT					wDeviceID;
void SetAVI(HWND m_pHwnd);
void CloseAVI(void);

int AVIdevice;
void SetAVI(HWND m_pHwnd)
{
	char fff[256];
	mciWindow.hWnd = m_pHwnd;
	mciOpenParms.lpstrDeviceType="AVIVideo";
	sprintf(GetFilePath,"%sdata\\wzsoft-logo.avi",RunPath);
	sprintf(fff,"%s",GetFilePath);
	mciOpenParms.lpstrElementName = fff;
	mciSendCommand(0,MCI_OPEN,MCI_OPEN_ELEMENT | MCI_OPEN_TYPE | MCI_ANIM_OPEN_WS,(DWORD)(LPVOID)&mciOpenParms);
	AVIdevice = wDeviceID = mciOpenParms.wDeviceID;
	AVIdevice = wDeviceID;
	mciRect.rc.top = 120;
	mciRect.rc.left = 60;
	mciRect.rc.right = 520;
	mciRect.rc.bottom = 240;
	mciSendCommand(wDeviceID,MCI_PUT ,MCI_ANIM_PUT_DESTINATION | MCI_ANIM_RECT,(DWORD)(LPVOID)&mciRect);
	mciSendCommand(wDeviceID,MCI_WINDOW,MCI_ANIM_WINDOW_HWND,(DWORD)(LPVOID)&mciWindow);
	mciPlayParms.dwCallback = (DWORD)m_pHwnd;
	mciSendCommand(wDeviceID,MCI_PLAY,MCI_NOTIFY,(DWORD)(LPVOID)&mciPlayParms);
	mciSendCommand(wDeviceID,MCI_REALIZE,MCI_ANIM_REALIZE_NORM,NULL);
}


void CloseAVI(void)
{
	mciSendCommand(wDeviceID,MCI_PAUSE,MCI_NOTIFY,(DWORD)(LPVOID) &mciPlayParms);
	mciSendCommand(wDeviceID,MCI_CLOSE,0,NULL);
}