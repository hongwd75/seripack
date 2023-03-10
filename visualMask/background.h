/*
   윈도우용 그래픽 클래스
   16Bit Color 모드용으로 되어있다.
*/

#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include"stdafx.h"

#define	tiffTag_ImageWidth				   0x0100
#define	tiffTag_ImageLength                0x0101
#define	tiffTag_BitsPerSample              0x0102
#define	tiffTag_Compression                0x0103
#define	tiffTag_PhotometricInterpretation  0x0106
#define	tiffTag_StripOffsets               0x0111
#define	tiffTag_SamplesPerPixels           0x0115
#define	tiffTag_RowsPerStrip               0x0116
#define	tiffTag_StripByteCounts            0x0117
#define	tiffTag_ColorMapOffset             0x0140

    typedef struct
	{
	    DWORD StripOffsets;
		DWORD StripByteCounts;
	} TStripInfo;

    typedef struct
	{
		WORD B;
		WORD G;
		WORD R;
	} RGBc;

    typedef struct
	{
		BYTE B;
		BYTE G;
		BYTE R;
	} RGB8;                       /* For Window 24Bit color mode */

	typedef struct 
	{
	    WORD Endian;
		WORD Confirm;
		DWORD FirstIFD;
	} TIFHEADER;


	class IFDHEADER
	{
		public:
		WORD Tag;
		WORD VarType;
		DWORD Count;
		DWORD Value;
	};

typedef struct {
	unsigned char B;
	unsigned char G;
	unsigned char R;
} RGBstruct;

class BG {

	private:
		static int m_MakeScrolBar;

	public:
		HWND				Dibhwnd;
		HWND				hwndParent;

	public:
		int					width;				// 가상페이지의 가로 크기
		int					height;				// 가상페이지의 세로 크기 
		RGBstruct			*m_virImg;			// 가상페이지의 이미지 데이타

		RGBstruct			*m_backimg;

		void m_PutImage2BackBuffer(void);							// 이미지를 백버퍼에 넣는다.
		


	public:
		HANDLE				m_back_handle;
		LPBITMAPINFO		m_backround;


		POINT				m_ScreenPos;		// 스크린 위치
		POINT				m_ScreenSize;		// 스크린 사이즈
		POINT				m_ScreenNPos;		// 스크린 현 위치 


		BOOL                clicked;
		BOOL				m_HaveScrol;		// 스크롤바를 가지고 있다.
		BOOL				Is_Haveimage;		// 이미지가 로드 되면 true값을 가진다.
		BOOL                CLOSE;				//
		BOOL				showcross;			// 십자가형태의 마우스 위치 좌표를 나타내준다.
		int                 m_ScMaxH;			// 세로 스크롤바 최대 크기 
		int					m_ScMaxW;			// 가로 스크롤바 최대 크기
		int                 m_ScPosH;			// 세로 스크롤바 현 위치
		int					m_ScPosW;			// 가로 스크롤바 현 위치

		int                 m_Mouse_x;
		int                 m_Mouse_y;
		int                 m_Type;				// imagetype;

		int					m_Enlarge;			// 확대 비율(최초 x1)

		char				szName[30];
		RECT				boxrect;

		BYTE                *MASK16;
		int                 maskwidth;
		int                 maskheight;
		int                 GetMasked(int x,int y);
		void                AutoMask(void);

	public:
		~BG();
		BG(int Posx,int Posy,int ScSizeX,int ScSizeY,BOOL HaveScroll,HWND hWndParent,HINSTANCE hInst);

	public:
		void WindowReSize();
		void PutBackGround(HDC hdc);
		void FillBackBuffer(int r,int g,int b);
		void DrawBox(int sx,int sy,int fx,int fy,int R,int G,int B);
		void ResetScroll();


		void WindowLen(void);

		void WindowEnlarge(void);
		void WindowReduce(void);

		void CaptureImage2BG(int pointx,int pointy,int width1,int height1,BG *target);

		BOOL ReadBMPfile(char *filename);
		BOOL ReadTIFfile(char *filename);

		void PutMask(int x,int y);
		void ShowMask(void);
};




/*
 ====================================================================
	TIFF class
	----------
													1999.5.1
 ====================================================================
*/
	class TIFF 	{
		public:
			WORD        IFDcount;		/* IFD의 수 */
			TIFHEADER  m_header;		/* TIFF 헤더 */
			IFDHEADER  *m_IFDindex;    /* Image File Directory */
			IFDHEADER  IFDAddr;			/* index에서 찾은 IFD를 저장하는곳 */
			
			RGBc	   PAL[256];		/* For 8Bit */

		public:
			int Width;
			int Height;
			RGB8 *Image;
	

		public:
			HWND hwND;					/* 에러 출력용 핸들 */
			TIFF(HWND ff);
			~TIFF();

		public:
			int GetWidth(void);			/* 가로크기 return */
			int GetHeight(void);		/* 세로크기 return */

			BOOL LoadTIFF(char *filename);
			BOOL GetIFD_Index(int Index); /* 인덱스에서 찾는다 */
			void IndexImageTo24BitImage(BYTE *src);
	};
#endif