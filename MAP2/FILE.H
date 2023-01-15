#ifndef __FILE_H__
#define __FILE_H__

void PopFileInit(HWND);
BOOL PopFileOpenDlg(HWND,PSTR,PSTR);
BOOL PopLISTFileSaveDlg(HWND hWnd,PSTR pstrFileName,PSTR pstrTitleName);

extern OPENFILENAME	ofn;
#endif