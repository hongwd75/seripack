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
//////////////////////////////////////
typedef struct 
{
	  int width[4];
	  int height[4];
	  int tilenum[4];
	  BOOL havetile[4];
	  BYTE *mapdata[4];
	  RGBstruct *bigtile[4];
	  POINT tilesize[4];
	  POINT mapsize[4];

	  char szFileNameL1[_MAX_PATH];
	  char szFileNameL2[_MAX_PATH];
	  char szFileNameL3[_MAX_PATH];
	  char szFileNameL4[_MAX_PATH];
	  char szOUTNameL4[_MAX_PATH];
	  char OUTObjName[_MAX_PATH];
} LAYERTYPE;
typedef struct 
{
	int worklayer;
	BOOL viewlayer[5];
	int showx,showy;
} mhsystem;
//////////////////////////////
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
		BOOL                NoScroll;	
		int                 m_ScMaxH;			// ���� ��ũ�ѹ� �ִ� ũ�� 
		int					m_ScMaxW;			// ���� ��ũ�ѹ� �ִ� ũ��
		int                 m_ScPosH;			// ���� ��ũ�ѹ� �� ��ġ
		int					m_ScPosW;			// ���� ��ũ�ѹ� �� ��ġ

		int                 m_Mouse_x;
		int                 m_Mouse_y;
		int                 m_Type;				// imagetype;

		int					m_Enlarge;			// Ȯ�� ����(���� x1)
		char				szName[30];

		int                 rectsize;			// 16 �Ǵ� 32��...

		BOOL				viewRule;

		POINT				ree;

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
		void PutPixel(int x,int y,int R,int G,int B);
		void GetVirMem(int x,int y);
		BOOL ReadBMPfile(char *filename);
		BOOL ReadTIFfile(char *filename);
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



	/////////////////
typedef struct 
{
	char name[30];			// object name
	int  index;				// object index number;
	RGBstruct *image;		// object image data;
	int width,height;		// image size(width,height)
} linkin ;

struct linkout
{
	char name[30];			// object name 
	int index;				// object index
	int x;					// map position x
	int y;					// map position y
	int option1;			//
	int option2;
	int clicked;
	struct linkout *next;
};

typedef struct linkout*		LPlinkout;
typedef struct linkout		taglinkout;


typedef struct 
{
	char Path[256];			// object data file directory
	int  maxiumobject;		// object number
	int  selobject;
	linkin data[512];
} inmain;

typedef struct
{
	int maxiumobject;		// put object number
	LPlinkout Header;		// output object list pointer
} outmain;

#endif