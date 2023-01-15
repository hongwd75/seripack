#include"stdafx.h"
#include<stdio.h>

OPENFILENAME	ofn;


void PopFileInit(HWND hWnd)
{

	static char szFilter[] = "BMP file  (*.BMP)\0*.bmp\0" \
							 "All Files (*.*)\0*.*\0\0";

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner			= hWnd;
	ofn.hInstance			= NULL;
	ofn.lpstrFilter			= szFilter;
	ofn.lpstrCustomFilter	= NULL;
	ofn.nMaxCustFilter		= 0;
	ofn.nFilterIndex		= 0;
	ofn.lpstrFile			= NULL;
	ofn.nMaxFile			= _MAX_PATH;
	ofn.lpstrFileTitle		= NULL;
	ofn.nMaxFileTitle		= _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir		= NULL;
	ofn.lpstrTitle			= NULL;
	ofn.Flags				=  OFN_ALLOWMULTISELECT;
	ofn.nFileExtension		= 0;
	ofn.nFileOffset			= 0;
	ofn.lpstrDefExt			= "MSK";
	ofn.lCustData			= 0L;
	ofn.lpfnHook			= NULL;
	ofn.lpTemplateName		= NULL;
}

BOOL PopFileOpenDlg(HWND hWnd,PSTR pstrFileName,PSTR pstrTitleName)
{
	char szFilter[] = "BMP file  (*.BMP)\0*.bmp\0" \
					  "All Files (*.*)\0*.*\0\0";
	ofn.hwndOwner 	   = hWnd;
	ofn.lpstrFilter	   = szFilter;
	ofn.lpstrFile	   = pstrFileName;
	ofn.lpstrFileTitle = pstrTitleName;
	//ofn.Flags          = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	ofn.Flags          = OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	return GetOpenFileName(&ofn);
}


BOOL PopLISTFileSaveDlg(HWND hWnd,PSTR pstrFileName,PSTR pstrTitleName)
{
	char szFilter[] = "mask file  (*.MSK)\0*.msk\0" \
					  "All Files (*.*)\0*.*\0\0";
	ofn.hwndOwner 	   = hWnd;
	ofn.lpstrFilter	   = szFilter;
	ofn.lpstrFile	   = pstrFileName;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.Flags          = OFN_HIDEREADONLY | OFN_CREATEPROMPT;

	return GetSaveFileName(&ofn);
}
