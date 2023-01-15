#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	char name[20];
} Filename;

#define		MAXHEIGHT			1024
#define     MAXWIDTH			2048

typedef struct {
	unsigned char B;
	unsigned char G;
	unsigned char R;
} RGBstruct;


typedef struct {
	WORD	r:5;
	WORD    g:5;
	WORD    b:5;
} Bit16Field;



typedef struct {
	int		width;
	int		height;
	int		ImageType;
	int		size;
	WORD     pattern[MAXHEIGHT];
	int		Yaddress[MAXHEIGHT];
	WORD	Image[MAXHEIGHT][MAXWIDTH];
	WORD     Lsize[MAXHEIGHT];
} Imagestruct;
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
int tile1size = 0;
int tile2size = 0;
int tile3size = 0;

Filename Tile1[256];
Filename Tile2[256];
Filename Tile3[256];

Filename MAPDATA[4];

int		     img_width,img_height;
WORD	     *Virimage16 = NULL;
Imagestruct  CompImage;
Bit16Field	BITS;

WORD GetColorValue(int R,int G, int B);	// 색값 교정
BOOL ReadBMPfile(char *filename);		// BMP file load
void Convert24TO16(RGBstruct *image);	// 24비트 16비트로 저장 

void SaveNormal555(FILE *file);			// 일반 스프라이트 저장시 쓴다
void VVCMP(FILE *file);					// 압축 스프라이트 저장시 불러쓴다
void ConvertCMP2(int width,int height,WORD *Data,FILE *file);

int mTransColor = GetColorValue(255,0,255);		// 투명값 저장

//////////////////////////////////////////
//
void ReadListFile(LPSTR listfile, LPSTR saveMAP)
{
	int loop;
	FILE *fp,*wp;
	char Buffer[256];

	fp = fopen(listfile,"rt");
	

	if(fp == NULL)
	{
		printf("File not found.\n");
		exit(-1);
	}

	do
	{   
		char Item[256],Item2[256]; 
		int arg1,arg2;
		if(fgets(Buffer,256,fp) == NULL) break;
        if (!Buffer) break;
		if(Buffer[0]==10) continue;				// 아무것도 없으면 다시 읽기

		sscanf(Buffer,"%s %d %d",Item,&arg1,&arg2);
        if (!strcmp(Item,"//")) continue;

		///////////////////////////////////////////////////////
		if (!strcmp(Item,"!"))
		{
			sscanf(Buffer,"%s %s %d",&Item,MAPDATA[0].name,&arg2);
			printf("detected layer 1 map%s\n",MAPDATA[0].name);
		}
		///////////////////////////////////////////////////////
		if (!strcmp(Item,"@"))
		{
			sscanf(Buffer,"%s %s %d",&Item,MAPDATA[1].name,&arg2);
			printf("detected layer 2 map : %s\n",MAPDATA[1].name);
		}
		///////////////////////////////////////////////////////
		if (!strcmp(Item,"#"))
		{
			sscanf(Buffer,"%s %s %d",&Item,MAPDATA[2].name,&arg2);
			printf("detected layer 3 map%s\n",MAPDATA[2].name);
		}
		///////////////////////////////////////////////////////
		if (!strcmp(Item,"$"))
		{
			sscanf(Buffer,"%s %s %d",&Item,MAPDATA[3].name,&arg2);
			printf("detected attribute map%s\n",MAPDATA[3].name);
		}
		///////////////////////////////////////////////////////

		if (!strcmp(Item,"%"))
		{
			switch(arg1)
			{
				case 1:
					{
						tile1size = arg2;
						printf("MAP 1 tiles [%d] tile..\n",arg2);
						if(arg2>256)
						{
							printf("  **** Tile Over flow Error Recovered 256size.\n");
							arg2 = 256;
						}
						for(int i = 0;i < arg2; i++)
						{
							fgets(Buffer,256,fp);
							sscanf(Buffer,"%s",Tile1[i].name);
							printf("  - %dMAP1 include : %s\n",i,Tile1[i].name);
						}
					}
					break;
				case 2:
					{
						tile2size = arg2;
						printf("MAP 2 tiles [%d] tile..\n",arg2);
						if(arg2>256)
						{
							printf("  **** Tile Over flow Error Recovered 256size.\n");
							arg2 = 256;
						}
						for(int i = 0;i < arg2; i++)
						{
							fgets(Buffer,256,fp);
							sscanf(Buffer,"%s",Tile2[i].name);
							printf("  - [%d]MAP2 include : %s\n",i,Tile2[i].name);
						}
					}
					break;
				case 3:
					{
						tile3size = arg2;
						printf("MAP 3 tiles [%d] tile..\n",arg2);
						if(arg2>256)
						{
							printf("  **** Tile Over flow Error Recovered 256size.\n");
							arg2 = 256;
						}
						for(int i = 0;i < arg2; i++)
						{
							fgets(Buffer,256,fp);
							sscanf(Buffer,"%s",Tile3[i].name);
							printf("  - [%d]MAP2 include : %s\n",i,Tile3[i].name);
						}
					}
					break;
			}
		}
    } while(1);
    fclose(fp);

	printf(" - Convert Image ==> Spritefile\n    #Creat %s (Map resource) file\n",saveMAP);
	wp = fopen(saveMAP,"wb");

	// Layer 1 tile save
	fwrite(&tile1size,4,1,wp);
	for(loop = 0; loop < tile1size; loop++)
	{
		if(!ReadBMPfile(Tile1[loop].name))
		{
			printf("  ******** BMP File Read Error :%s ********\n",Tile1[loop].name);
			fclose(wp);
			exit(-1);
		}
		VVCMP(wp);
	}


	// Layer 2 tile save
	fwrite(&tile2size,4,1,wp);
	for(loop = 0; loop < tile2size; loop++)
	{
		if(!ReadBMPfile(Tile2[loop].name))
		{
			printf("  ******** BMP File Read Error :%s ********\n",Tile2[loop].name);
			fclose(wp);
			exit(-1);
		}
		VVCMP(wp);
	}


	// layer 3 tile save
	fwrite(&tile3size,4,1,wp);
	for(loop = 0; loop < tile3size; loop++)
	{
		if(!ReadBMPfile(Tile3[loop].name))
		{
			printf("  ******** BMP File Read Error :%s ********\n",Tile3[loop].name);
			fclose(wp);
			exit(-1);
		}
		VVCMP(wp);
	}
	

	printf("    - Save MAP Datas\n");
	for(loop = 0; loop < 4; loop++)
	{
		FILE *tempfile;
		tempfile = fopen(MAPDATA[loop].name,"rb");
		long filesz = _filelength(_fileno(tempfile));
		BYTE *pointers = new BYTE[filesz];
		fread((BYTE *)pointers,filesz,1,tempfile);
		// Put List file name & index
		int *maps =(int *)pointers;
		maps[9] = maps[1];
		maps[10] = maps[2];
		pointers+=36;
		maps = (int *)pointers;
		filesz -=36;
		fwrite(pointers,filesz,1,wp);
		printf("X:%d  Y:%d\n",maps[0],maps[1]);
		pointers-=36;
		delete pointers;
		fclose(tempfile);
	}

	fclose(wp);


}



void main(int argc,char *argv[])
{
  printf("==============================================================================\n");
  printf("  Map & Tile manager\n");
  printf("------------------------------------------------------------------------------\n");	 

  if(argc == 1 || argc < 2)
  {
	  printf("MAPMAN [list file name] [Resource file name]\n");
  } else 
  {
		printf("  # List source file : %s\n",argv[1]);
		ReadListFile(argv[1],argv[2]);
  }

  printf("==============================================================================\n");
}



////////////////////////////////////////////////////////////////////////////////////////
// BMP Load
BOOL ReadBMPfile(char *filename)
{

	FILE		*fp;
	long		m_numColors;
	long		fileLength;
	int			sourceadd,targetadd;
	int			dummywidth;
	BOOL		UPDOWN=FALSE;
	RGBstruct	*m_virImg = NULL;			// 24비트 이미지 저정용
	char		ff[50];

	LPBITMAPINFO       m_pBmInfo;
	LPBITMAPINFOHEADER m_pBmInfoHeader;
	RGBQUAD           *m_pRGBTable;
	BYTE              *m_pDibBits;

	BITMAPFILEHEADER bmFileHeader;

	fp = fopen(filename,"rb");
    fread((void *)&bmFileHeader, sizeof(BITMAPFILEHEADER),1,fp);
	if (bmFileHeader.bfType != 0x4d42)
	{
		fclose(fp);
		return FALSE;
	}
	else
	{
		fileLength = filelength(fileno(fp));
        if (!fileLength) 
		{
			fclose(fp);
			MessageBox(NULL, "FileLength 0", MB_OK, NULL);
			return FALSE;
		}

		DWORD dibSize    = fileLength - sizeof(bmFileHeader);

		BYTE *pDib = new BYTE[dibSize];

		DWORD l = fread((void *)pDib, dibSize,1,fp);
        if (!l)
        {
			fclose(fp);
			delete pDib;
			wsprintf(ff,"BMP file Read Error : %s",filename);
            MessageBox(NULL, ff, MB_OK, NULL);
			return FALSE;
        }
        fclose(fp);

		m_pBmInfo = (LPBITMAPINFO) pDib;
		m_pBmInfoHeader = (LPBITMAPINFOHEADER) pDib;

		m_pRGBTable = (RGBQUAD *)(pDib + sizeof(BITMAPINFOHEADER));

		// Get color bit		
		if((m_pBmInfoHeader->biClrUsed == 0) && (m_pBmInfoHeader->biBitCount < 9))
			 m_numColors = (1 << m_pBmInfoHeader->biBitCount);
		else m_numColors = (int) m_pBmInfoHeader->biClrUsed;

		if(m_pBmInfoHeader->biHeight <0) 
		{
			m_pBmInfoHeader->biHeight = -m_pBmInfoHeader->biHeight;
			UPDOWN = TRUE;
		}
		// Get Image Size;
		if (m_pBmInfoHeader->biSizeImage == 0)
		{
	        m_pBmInfoHeader->biSizeImage = m_pBmInfoHeader->biWidth * m_pBmInfoHeader->biHeight;
			dummywidth = 0;
		} else 
		{
			dummywidth = (m_pBmInfoHeader->biSizeImage / m_pBmInfoHeader->biHeight) - m_pBmInfoHeader->biWidth;
		}
		if(m_pBmInfoHeader->biClrUsed == 0)
			m_pBmInfoHeader->biClrUsed = m_numColors;

		DWORD clrTableSize = m_numColors * sizeof(RGBQUAD);
		m_pDibBits = pDib + m_pBmInfoHeader->biSize + clrTableSize;

		m_virImg = new RGBstruct[m_pBmInfoHeader->biWidth * m_pBmInfoHeader->biHeight];
		img_width = m_pBmInfoHeader->biWidth;				// 가상페이지의 가로 크기
		img_height = m_pBmInfoHeader->biHeight;				// 가상페이지의 세로 크기 
		sourceadd = 0;
		targetadd = 0;
		if(m_numColors>0 && m_numColors <256)
		{
			for(int y = 0; y < m_pBmInfoHeader->biHeight; y++)
			{
				for(int x = 0; x < m_pBmInfoHeader->biWidth; x++)
				{
					m_virImg[targetadd].R = m_pRGBTable[m_pDibBits[sourceadd]].rgbRed;
					m_virImg[targetadd].B = m_pRGBTable[m_pDibBits[sourceadd]].rgbBlue;
					m_virImg[targetadd].G = m_pRGBTable[m_pDibBits[sourceadd]].rgbGreen;
					sourceadd++;
					targetadd++;
				}
				sourceadd+= dummywidth;
			}
		}
		else
		{
			for(int y = 0; y < m_pBmInfoHeader->biHeight; y++)
			{
				for(int x = 0; x < m_pBmInfoHeader->biWidth; x++)
				{
					m_virImg[targetadd].B = m_pDibBits[sourceadd];
					m_virImg[targetadd].G = m_pDibBits[sourceadd+1];
					m_virImg[targetadd].R = m_pDibBits[sourceadd+2];// b g r
					sourceadd+=3;
					targetadd++;
				}
				sourceadd+=dummywidth * 3;
			}
		}
		delete pDib;
	}

	if(!UPDOWN)
	{
		BYTE exR,exG,exB;
		int addr1,addr2;
		addr1=0;
		addr2=img_width * (img_height-1);
		for(int ayy=0;ayy<(img_height >> 1);ayy++)
		{
			for(int axx=0;axx<img_width;axx++)
			{
				exR = m_virImg[addr1+axx].R;
				exG = m_virImg[addr1+axx].G;
				exB = m_virImg[addr1+axx].B;

				m_virImg[addr1+axx].R = m_virImg[addr2+axx].R;
				m_virImg[addr1+axx].G = m_virImg[addr2+axx].G;
				m_virImg[addr1+axx].B = m_virImg[addr2+axx].B;

				m_virImg[addr2+axx].R = exR;
				m_virImg[addr2+axx].G = exG;
				m_virImg[addr2+axx].B = exB;

			}
			addr1+=img_width;
			addr2-=img_width;

		}
	}

	Convert24TO16(m_virImg);
	delete m_virImg;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// 압축 스프라이트 저장시 불러쓴다
void VVCMP(FILE *file)
{
	ConvertCMP2(img_width,img_height,Virimage16,file);
}

void ConvertCMP2(int width,int height,WORD *Data,FILE *file)
{
	WORD *tempbuffer;
	int  *heights;
	WORD *Dest;

	
	int srccount = 0;			// Data 주소 카운터 
	int memcount = 0;			// Dest 주소 카운터
	int bufferpos = 0;			// styles가 들어갈 주소 카운터
	int transcount;
	int styles;					// 몇개의 
	int hcount = 0;
	int mswitch;

	int tempcount;
	int csize;

	tempbuffer = new WORD[width * 2];
	heights = new int[height];
	Dest    = new WORD[width * height * 2];

	while(hcount < height)
	{
		styles = 0;
		heights[hcount] = memcount;
		bufferpos = memcount++;
		mswitch = 0;
		transcount = 0;
		for(int i=0; i < width; i++)
		{
			switch(mswitch)
			{
				case 0:
					if(Data[srccount+i] == mTransColor) transcount++;
					else 
					{
						i--;
						mswitch = 1;
						Dest[memcount++] = transcount;
						transcount=0;
						tempcount=0;
						csize = 0;
					}
					break;
				case 1:
					if(Data[srccount+i] != mTransColor)
					{
						tempbuffer[tempcount++] = Data[srccount+i];
					} else 
					{
						i--;
						mswitch = 2;
					}
					break;
				case 2:
					Dest[memcount++] = tempcount;
					for(int j=0;j<tempcount;j++)
					{
						Dest[memcount++] = tempbuffer[j];
					}
					if(i<width-1)
					{
						mswitch = 0;
						transcount = 0;
						i--;
					}
					styles++;
					break;
			}
		}

		if(mswitch == 0)
		{
			Dest[memcount++] = transcount;
			Dest[memcount++] = 0;
			styles++;
		}

		if(mswitch == 1)
		{
			Dest[memcount++] = tempcount;
			if(tempcount>0)
			{
				for(int j=0;j<tempcount;j++)
				{
					Dest[memcount++] = tempbuffer[j];
				}
			}
			styles++;
		}
		Dest[bufferpos] = styles;
		hcount++;
		srccount+=width;
	}
   // 가로, 세로, 세로데이타주소값, 데이타 변수가 있어야 한다.

	FILE   *fp = file;
	char HEADER[4];

	strcpy(HEADER,"SP1");
	fwrite(&HEADER,3,1,fp);
	fwrite(&width,4,1,fp);
	fwrite(&height,4,1,fp);
	fwrite(heights,height,4,fp);
	fwrite(&memcount,4,1,fp);
	fwrite(Dest,memcount*2,1,fp);

	delete tempbuffer;
	delete heights;
	delete Dest;
}


/////////////////////////////////////////////////////////////////////
void SaveNormal555(FILE *file)
{
	FILE *fp = file;
	char HEADER[4];
	strcpy(HEADER,"NO1");
	fwrite(&HEADER,3,1,fp);
	fwrite(&img_width,4,1,fp);
	fwrite(&img_height,4,1,fp);
	fwrite(Virimage16,img_width * img_height,2,fp);
}


////////////////////////////////////////////////////////////////////////////////
WORD GetColorValue(int R,int G, int B)
{
	return ((R >> 3) <<10) | ((G >> 3) << 5) | (B >> 3);
}


////////////////////////////////////////////////////////////////////////////////
void Convert24TO16(RGBstruct *image)
{
	int loop;

	if(Virimage16 != NULL) delete Virimage16;
	Virimage16 = new WORD[img_width * img_height];

	for(loop = 0; loop < img_width * img_height; loop++)
	{
		Virimage16[loop] = GetColorValue(image->R,image->G,image->B);
		image++;
	}
}
