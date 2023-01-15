#include"stdafx.h"
#include"stdlib.h"
#include"stdio.h"
#include"conio.h"
#include"background.h"


#define		MAXHEIGHT			1024
#define     MAXWIDTH			2048
extern BG   *Layers;
extern BG   *Scren;
extern mhsystem	 wondae;
extern LAYERTYPE maindata;
extern char      Listfilepath[256];

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


extern int		     img_width,img_height;

WORD	     *Virimage16 = NULL;
Imagestruct  CompImage;

WORD GetColorValue(int R,int G, int B);	// ���� ����
BOOL ReadBMPfile(char *filename);		// BMP file load
void Convert24TO16(RGBstruct *image);	// 24��Ʈ 16��Ʈ�� ���� 

void SaveNormal555(FILE *file);			// �Ϲ� ��������Ʈ ����� ����
void VVCMP(FILE *file);					// ���� ��������Ʈ ����� �ҷ�����
void ConvertCMP2(int width,int height,WORD *Data,FILE *file);
extern void convertBYTEData(BYTE *data,int sizes);
int mTransColor = GetColorValue(255,0,255);		// ������ ����




////////////////////////////////////////////////////////////////////////////////
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



////////////////////////////////////////////////////////////////////////
// ���� ��������Ʈ ����� �ҷ�����
void VVCMP(FILE *file)
{
	ConvertCMP2(img_width,img_height,Virimage16,file);
}


////////////////////////////////////////////////////////////////////////
/*
void ConvertCMP2(int width,int height,WORD *Data,FILE *file)
{
	WORD *tempbuffer;
	int  *heights;
	WORD *Dest;

	
	int srccount = 0;			// Data �ּ� ī���� 
	int memcount = 0;			// Dest �ּ� ī����
	int bufferpos = 0;			// styles�� �� �ּ� ī����
	int transcount;
	int styles;					// ��� 
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
   // ����, ����, ���ε���Ÿ�ּҰ�, ����Ÿ ������ �־�� �Ѵ�.

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
*/

#define BLANKTYPE			0				// �����÷��϶�
#define PIXELTYPE			1				// �Ϲ��÷��϶�

void ConvertCMP2(int width,int height,WORD *Data,FILE *file)
{
	WORD *tempbuffer;
	int  *heights;
	WORD *Dest;

	
	int stylecount = 0;				// [������][�÷���] ��� ��Ÿ���� ��� ����ֳ�.
	int styleBufferAddr = 0;		// styles�� �� �ּ� ī����
	int ColorSizeBufferAddr = 0;	

	int srccount = 0;				// Data �ּ� ī���� 
	int memcount = 0;				// Dest �ּ� ī����
	int count = 0;					// ī���Ϳ�
	int styles;

	tempbuffer = new WORD[width * 3];
	heights = new int[height];
	Dest    = new WORD[width * height * 3];

	for(int y = 0; y < height; y++)
	{
		styles = BLANKTYPE;				// �������� ����
		count = 0;						// �������� 0���̴�.

		heights[y] = memcount;			// ���� ���� ����

		stylecount	= 0;				// ��Ÿ�� �� ī���� 0���� �ʱ�ȭ
		styleBufferAddr = memcount++;	// ��Ÿ�ϼ��� �� ����

		for(int x = 0; x < width; x++)
		{
			if(Data[srccount] == mTransColor)
			{
				if(styles == BLANKTYPE) count++;	// �������� �����̸� �����ش�
					else							// �������� �ƴ�
					{
						Dest[ColorSizeBufferAddr] = count;
						// �������� �ʱ�ȭ
						count = 1;
						styles = BLANKTYPE;
					}
			} else 
			{
				if(styles == BLANKTYPE)
				{
					stylecount++;						// ��Ÿ�� ī���� ����.
					Dest[memcount++] = count;			// ������ �ְ� 
					ColorSizeBufferAddr = memcount;		// ���� ��巹�� ����
					memcount++;							// Ÿ�� ���� ����� ����
					count = 1;
					styles = PIXELTYPE;
				} else count++;
				Dest[memcount++] = Data[srccount];		// �÷��� �ְ� �޸� ����
			}
			srccount++;
		}  // End of X
		
		if(styles == BLANKTYPE) 
		{
			stylecount++;
			Dest[memcount++] = count;
			Dest[memcount++] = 0;
		} else 
		{
			Dest[ColorSizeBufferAddr] = count;
		}

		Dest[styleBufferAddr] = stylecount;
	}  // End of y

	FILE   *fp = file;
	char HEADER[4];

	//fp = fopen(filename,"wb");
	
	strcpy(HEADER,"SP1");
	fwrite(&HEADER,3,1,fp);
	fwrite(&width,4,1,fp);
	fwrite(&height,4,1,fp);
	fwrite(heights,height,4,fp);
	fwrite(&memcount,4,1,fp);
	fwrite(Dest,memcount*2,1,fp);
//	fclose(fp);

	delete tempbuffer;
	delete heights;
	delete Dest;
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


void GetTileImage(int index,int layer)
{
	RGBstruct *ttx = new RGBstruct[32*32];
	img_width = 32;
	img_height = 32;
	RGBstruct *ff = maindata.bigtile[layer];

	int lineindex = (640 / maindata.tilesize[layer].x);
	int IDX = (index % lineindex) * maindata.tilesize[layer].x;
	int IDY = (index / lineindex) * maindata.tilesize[layer].x;

	int addr = IDY * 640 + IDX;
	int addr2= 0;

	for(int j=0; j < maindata.tilesize[layer].x; j++)
	{
		for(int i=0; i < maindata.tilesize[layer].x; i++)
		{
			ttx[addr2].R = ff[addr+i].R;
			ttx[addr2].G = ff[addr+i].G;
			ttx[addr2].B = ff[addr+i].B;
			addr2++;
		}
		addr+=640;
	}
	Convert24TO16(ttx);
	delete ttx;
}

void convertmapdata(BYTE *data,int maxindex,int datasize)
{
	BYTE *DTA = data;
	for(int i =0; i <datasize;i++)
	{
		if(DTA[i]>maxindex) DTA[i]=0; else 
			DTA[i]++;
	}
}

void SaveMapResource(LPSTR filename)
{
  int tilesize,i;
  FILE *fp,*fp2;

  fp = fopen(filename,"wb");
  fp2 = fopen("c:\\ttt.txt","wt");
  for(i=0;i<3;i++)
  {
	  if(maindata.havetile[i])
	  {
		  tilesize = maindata.tilenum[i];
		  fwrite(&tilesize,4,1,fp);

		  for(int j=0;j <= tilesize;j++)
		  {
			  GetTileImage(j,i);
			  if(i==0) SaveNormal555(fp);
			  else VVCMP(fp);
		  }
	  } else 
	  {
		  tilesize = 0xFFFF;				// Ÿ�� ����, ���̾ ���� ����
		  fwrite(&tilesize,4,1,fp);
		  fprintf(fp2,"LAYER %d NONE\n",i);
	  }
  }

  int width;
  int height;

  for( i = 0; i < 4; i++)
  {
	  if(maindata.havetile[i])
	  {
		  width  = maindata.mapsize[i].x;
		  height = maindata.mapsize[i].y;
		  
		  fwrite(&width,4,1,fp);
		  fwrite(&height,4,1,fp);
		  if(i==0) 
		  {
			  convertmapdata(maindata.mapdata[i],maindata.tilenum[i],width * height);
		  }
		  fwrite(maindata.mapdata[i],width * height,1,fp);
		  if(i==0) 
		  {
			  convertBYTEData(maindata.mapdata[i],maindata.mapsize[i].x * maindata.mapsize[i].y);
		  }

	  }
  }

  fclose(fp);
  fclose(fp2);
}