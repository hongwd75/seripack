#ifndef __FILE_H__
#define __FILE_H__

void PopFileInit(HWND);
BOOL PopFileOpenDlg(HWND,PSTR,PSTR);
BOOL PopLISTFileOpenDlg(HWND hWnd,PSTR pstrFileName,PSTR pstrTitleName);
BOOL PopLISTFileSaveDlg(HWND hWnd,PSTR pstrFileName,PSTR pstrTitleName);
BOOL PopRESFileSaveDlg(HWND hWnd,PSTR pstrFileName,PSTR pstrTitleName);

extern OPENFILENAME	ofn;
#endif