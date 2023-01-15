/*
   ������� �׷��� Ŭ����
   16Bit Color �������� �Ǿ��ִ�.
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
		int					width;				// ������������ ���� ũ��
		int					height;				// ������������ ���� ũ�� 
		RGBstruct			*m_virImg;			// ������������ �̹��� ����Ÿ

		RGBstruct			*m_backimg;

		void m_PutImage2BackBuffer(void);							// �̹����� ����ۿ� �ִ´�.
		


	public:
		HANDLE				m_back_handle;
		LPBITMAPINFO		m_backround;


		POINT				m_ScreenPos;		// ��ũ�� ��ġ
		POINT				m_ScreenSize;		// ��ũ�� ������
		POINT				m_ScreenNPos;		// ��ũ�� �� ��ġ 


		BOOL                clicked;
		BOOL				m_HaveScrol;		// ��ũ�ѹٸ� ������ �ִ�.
		BOOL				Is_Haveimage;		// �̹����� �ε� �Ǹ� true���� ������.
		BOOL                CLOSE;				//
		BOOL				showcross;			// ���ڰ������� ���콺 ��ġ ��ǥ�� ��Ÿ���ش�.
		int                 m_ScMaxH;			// ���� ��ũ�ѹ� �ִ� ũ�� 
		int					m_ScMaxW;			// ���� ��ũ�ѹ� �ִ� ũ��
		int                 m_ScPosH;			// ���� ��ũ�ѹ� �� ��ġ
		int					m_ScPosW;			// ���� ��ũ�ѹ� �� ��ġ

		int                 m_Mouse_x;
		int                 m_Mouse_y;
		int                 m_Type;				// imagetype;

		int					m_Enlarge;			// Ȯ�� ����(���� x1)

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
			WORD        IFDcount;		/* IFD�� �� */
			TIFHEADER  m_header;		/* TIFF ��� */
			IFDHEADER  *m_IFDindex;    /* Image File Directory */
			IFDHEADER  IFDAddr;			/* index���� ã�� IFD�� �����ϴ°� */
			
			RGBc	   PAL[256];		/* For 8Bit */

		public:
			int Width;
			int Height;
			RGB8 *Image;
	

		public:
			HWND hwND;					/* ���� ��¿� �ڵ� */
			TIFF(HWND ff);
			~TIFF();

		public:
			int GetWidth(void);			/* ����ũ�� return */
			int GetHeight(void);		/* ����ũ�� return */

			BOOL LoadTIFF(char *filename);
			BOOL GetIFD_Index(int Index); /* �ε������� ã�´� */
			void IndexImageTo24BitImage(BYTE *src);
	};
#endif