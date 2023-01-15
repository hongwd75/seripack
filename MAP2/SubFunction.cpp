#include"stdafx.h"
#include"stdlib.h"
#include"stdio.h"
#include"conio.h"
#include"background.h"


extern BG			*Layers;
extern BG			*Scren;
extern inmain		InObjectlist;
extern outmain		OutObjectlist;
extern BOOL			opened;
extern mhsystem		wondae;
extern LAYERTYPE	maindata;
extern char			Listfilepath[256];
extern void			SaveMapResource(LPSTR filename);
extern int			img_width,img_height;
extern void			ReadObjectList(LPSTR filename);
extern void			ReadObject(LPSTR filename);
extern void			WriteObject(LPSTR filename);

extern void ShowObject(void);

void PutTile(int x,int y,int layer,int index)
{
	RGBstruct *ff = maindata.bigtile[layer];
	int lineindex = (640 / maindata.tilesize[layer].x);
	int IDX = (index % lineindex) * maindata.tilesize[layer].x;
	int IDY = (index / lineindex) * maindata.tilesize[layer].x;

	int addr = IDY * 640 + IDX;

	if(index > maindata.tilenum[layer]) return;
	if(layer>0)
	{
		if(index == 0) return;
	}
	for(int j=0; j < maindata.tilesize[layer].x; j++)
	{
		for(int i=0; i < maindata.tilesize[layer].x; i++)
		{
			Scren->PutPixel(x+i,y+j,ff[addr+i].R,ff[addr+i].G,ff[addr+i].B);
		}
		addr+=640;
	}
}

void PutTileInScreen(int sx,int sy,int layer)
{
	BYTE *mapdata = maindata.mapdata[layer];

	int addr = sy * maindata.mapsize[layer].x + sx;
	int maxx = 640 / maindata.tilesize[layer].x;
	int maxy = 480 / maindata.tilesize[layer].y;

	if(maxx + sx >= maindata.mapsize[layer].x)
	{
		maxx = maindata.mapsize[layer].x - sx;
	}
	if(maxy + sy >= maindata.mapsize[layer].y)
	{
		maxy = maindata.mapsize[layer].y - sy;
	}

	for(int i = 0; i < maxy; i++)
	{
		for(int j = 0; j < maxx; j++)
		{
			PutTile(j*maindata.tilesize[layer].x,i*maindata.tilesize[layer].x,layer,mapdata[addr+j]);
		}
		addr+= maindata.mapsize[layer].x;
	}
   
}


void ViewTiles(void)
{
	Scren->FillBackBuffer(0,0,0);
	for(int i=0; i<4;i++)
	{
		if(wondae.viewlayer[i])
		{
			if(i != 3) PutTileInScreen(wondae.showx,wondae.showy,i);
			else PutTileInScreen(wondae.showx*2,wondae.showy*2,i);
		}
	}
	if(wondae.viewlayer[4]) ShowObject();
	InvalidateRect(Scren->Dibhwnd,NULL,FALSE);
}


RGBstruct *ReadBMPfile(char *filename,int kk)
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
		return NULL;
	}
	else
	{
		
		fileLength = filelength(fileno(fp));
        if (!fileLength) 
		{
			fclose(fp);
			MessageBox(NULL, "FileLength 0", MB_OK, NULL);
			return NULL;
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
			return NULL;
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
		if(kk < 4)
		{
			maindata.width[kk] = img_width = m_pBmInfoHeader->biWidth;				// 가상페이지의 가로 크기
			maindata.height[kk] = img_height = m_pBmInfoHeader->biHeight;				// 가상페이지의 세로 크기 
		} else 
		{
			img_width = m_pBmInfoHeader->biWidth;				// 가상페이지의 가로 크기
			img_height = m_pBmInfoHeader->biHeight;				// 가상페이지의 세로 크기 
		}
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

	return m_virImg;
}



/////////////////////////////////////////////////////////////////////////////////
void GetPath(LPSTR source)
{
	char *tempstr = source;
	int i = strlen(tempstr);

	for(int loop = i; loop > 0; loop--)
	{
		if(tempstr[loop] == '\\')
		{
			memcpy(Listfilepath,tempstr,loop);
			Listfilepath[loop+1] = 0;
			break;
		}
	}
}

void convertBYTEData(BYTE *data,int sizes)
{
	for(int i = 0; i < sizes; i++)
	{
		if(data[i]==0) data[i]=255;
		else data[i]--;
	}
}

void InsertImageFile(LPSTR filename)
{
	FILE *fp;
	FILE *fp2;
	char Buffer[256],buffer2[256],buffer3[256];

	int  wid,hei,num;
	
	GetPath(filename);		// Get file path

	fp = fopen(filename,"rt");
	if(fp == NULL)
	{
		MessageBox(NULL,filename,"File not found.",MB_OK | MB_ICONSTOP);
		return;
	}
	do
	{
		if(fgets(Buffer,256,fp) == NULL) break;
		if(Buffer[0]==10) continue;				// 아무것도 없으면 다시 읽기
        if (!Buffer) continue;
        if (!strcmp(Buffer,"//")) continue;

		sscanf(Buffer,"%s %s %d %d %d",&buffer2,&buffer3,&wid,&hei,&num);

		if(!strcmp(buffer2,"OUT"))
		{
			sprintf(maindata.szOUTNameL4,"%s",buffer3);
		}
		if(!strcmp(buffer2,"LF1"))
		{
			sprintf(maindata.szFileNameL1,"%s",buffer3);
		}
		if(!strcmp(buffer2,"LF2"))
		{
			sprintf(maindata.szFileNameL2,"%s",buffer3);
		}
		if(!strcmp(buffer2,"LF3"))
		{
			sprintf(maindata.szFileNameL3,"%s",buffer3);
		}
		if(!strcmp(buffer2,"LF4"))
		{
			sprintf(maindata.szFileNameL4,"%s",buffer3);
		}

		if(!strcmp(buffer2,"OBJECT"))
		{
			char fname[256];
			sprintf(fname,"%s\\%s",Listfilepath,buffer3);
			ReadObjectList(fname);
		}

		if(!strcmp(buffer2,"OBJECTOUT"))
		{
			sprintf(maindata.OUTObjName,"%s\\%s",Listfilepath,buffer3);
			ReadObject(maindata.OUTObjName);
		}


		if(!strcmp(buffer2,"LAYER1"))
		{
			if(!strcmp(buffer3,"NONE")) maindata.havetile[0] = FALSE;
			else 
			{
				char ffx[256];
				sprintf(ffx,"%s\\%s",Listfilepath,buffer3);
				maindata.havetile[0] = TRUE;
				maindata.bigtile[0] = ReadBMPfile(ffx,0);
				
				maindata.tilesize[0].x = 32;
				maindata.tilesize[0].y = 32;
				maindata.mapsize[0].x  = wid;
				maindata.mapsize[0].y  = hei;
				maindata.tilenum[0] = num;
				maindata.mapdata[0] = new BYTE[maindata.mapsize[0].x * maindata.mapsize[0].y]; 
				memset(maindata.mapdata[0],0,maindata.mapsize[0].x * maindata.mapsize[0].y);

				sprintf(ffx,"%s\\%s",Listfilepath,maindata.szFileNameL1);
				fp2 =fopen(ffx,"rb");
				if(fp2 != NULL)
				{
					fread((BYTE *)maindata.mapdata[0],maindata.mapsize[0].x * maindata.mapsize[0].y,1,fp2);
					fclose(fp2);
				}
			}
		}
		if(!strcmp(buffer2,"LAYER2"))
		{
			if(!strcmp(buffer3,"NONE")) maindata.havetile[1] = FALSE;
			else 
			{
				char ffx[256];
				sprintf(ffx,"%s\\%s",Listfilepath,buffer3);
				maindata.havetile[1] = TRUE;
				maindata.bigtile[1] = ReadBMPfile(ffx,1);
				
				maindata.tilesize[1].x = 32;
				maindata.tilesize[1].y = 32;
				maindata.mapsize[1].x  = wid;
				maindata.mapsize[1].y  = hei;
				maindata.tilenum[1] = num;
				maindata.mapdata[1] = new BYTE[maindata.mapsize[1].x * maindata.mapsize[1].y]; 
				memset(maindata.mapdata[1],0,maindata.mapsize[1].x * maindata.mapsize[1].y);

				sprintf(ffx,"%s\\%s",Listfilepath,maindata.szFileNameL2);
				fp2 =fopen(ffx,"rb");
				if(fp2 != NULL)
				{
					fread((BYTE *)maindata.mapdata[1],maindata.mapsize[1].x * maindata.mapsize[1].y,1,fp2);
					fclose(fp2);
					//convertBYTEData(maindata.mapdata[1],maindata.mapsize[1].x * maindata.mapsize[1].y);
				}
			}
		}

		if(!strcmp(buffer2,"LAYER3"))
		{
			if(!strcmp(buffer3,"NONE")) maindata.havetile[2] = FALSE;
			else 
			{
				char ffx[256];
				sprintf(ffx,"%s\\%s",Listfilepath,buffer3);
				maindata.havetile[2] = TRUE;
				maindata.bigtile[2] = ReadBMPfile(ffx,2);
				maindata.tilesize[2].x = 32;
				maindata.tilesize[2].y = 32;
				maindata.mapsize[2].x  = wid;
				maindata.mapsize[2].y  = hei;
				maindata.tilenum[2] = num;
				maindata.mapdata[2] = new BYTE[maindata.mapsize[2].x * maindata.mapsize[2].y]; 
				memset(maindata.mapdata[2],0,maindata.mapsize[2].x * maindata.mapsize[2].y);

				sprintf(ffx,"%s\\%s",Listfilepath,maindata.szFileNameL3);
				fp2 =fopen(ffx,"rb");
				if(fp2 != NULL)
				{
					fread((BYTE *)maindata.mapdata[2],maindata.mapsize[2].x * maindata.mapsize[2].y,1,fp2);
					fclose(fp2);
				}

			}
		}


		if(!strcmp(buffer2,"LAYER4"))
		{
			if(!strcmp(buffer3,"NONE")) maindata.havetile[3] = FALSE;
			else 
			{
				maindata.havetile[3] = TRUE;
				char ffx[256];
				sprintf(ffx,"%s\\%s",Listfilepath,buffer3);
				maindata.bigtile[3] = ReadBMPfile(ffx,3);
				maindata.mapsize[3].x =maindata.mapsize[1].x * 2;
				maindata.mapsize[3].y =maindata.mapsize[1].y * 2; 
				maindata.tilesize[3].x = 16;
				maindata.tilesize[3].y = 16;
				maindata.tilenum[3] = 13;
				maindata.mapdata[3] = new BYTE[maindata.mapsize[3].x * maindata.mapsize[3].y]; 
				memset(maindata.mapdata[3],0,maindata.mapsize[3].x * maindata.mapsize[3].y);

				sprintf(ffx,"%s\\%s",Listfilepath,maindata.szFileNameL4);
				fp2 =fopen(ffx,"rb");
				if(fp2 != NULL)
				{
					fread((BYTE *)maindata.mapdata[3],maindata.mapsize[3].x * maindata.mapsize[3].y,1,fp2);
					fclose(fp2);
				}

			}
		}
   } while(1);
    fclose(fp);
	opened = TRUE;
	wondae.worklayer = -1;
	wondae.showx=0;
	wondae.showy=0;
	wondae.viewlayer[0] = FALSE;
	wondae.viewlayer[1] = FALSE;
	wondae.viewlayer[2] = FALSE;
	wondae.viewlayer[3] = FALSE;

}


void SaveWorkSpace(void)
{
	char ffx[256];
	FILE *fp;

	if(maindata.havetile[0])
	{
		sprintf(ffx,"%s\\%s",Listfilepath,maindata.szFileNameL1);
		fp = fopen(ffx,"wb");
		fwrite(maindata.mapdata[0],maindata.mapsize[0].x * maindata.mapsize[0].y,1,fp);
		fclose(fp);
	}

	if(maindata.havetile[1])
	{
		sprintf(ffx,"%s\\%s",Listfilepath,maindata.szFileNameL2);
		fp = fopen(ffx,"wb");
		fwrite(maindata.mapdata[1],maindata.mapsize[1].x * maindata.mapsize[1].y,1,fp);
		fclose(fp);
	}

	if(maindata.havetile[2])
	{
		sprintf(ffx,"%s\\%s",Listfilepath,maindata.szFileNameL3);
		fp = fopen(ffx,"wb");
		fwrite(maindata.mapdata[2],maindata.mapsize[2].x * maindata.mapsize[2].y,1,fp);
		fclose(fp);
	}

	if(maindata.havetile[3])
	{
		sprintf(ffx,"%s\\%s",Listfilepath,maindata.szFileNameL4);
		fp = fopen(ffx,"wb");
		fwrite(maindata.mapdata[3],maindata.mapsize[3].x * maindata.mapsize[3].y,1,fp);
		fclose(fp);
	}

	WriteObject(maindata.OUTObjName);
	sprintf(ffx,"%s\\%s",Listfilepath,maindata.szOUTNameL4);
	SaveMapResource(ffx);
}

void SetTileLayer(int layer)  // first 0 ..
{
	Layers->FillBackBuffer(0,0,0);
	Layers->Is_Haveimage = FALSE;

	if(layer < 4)
	{
		Layers->width = maindata.width[layer];
		Layers->height = maindata.height[layer];

		Layers->m_virImg =maindata.bigtile[layer];
		Scren->ree.x = Layers->ree.x = maindata.tilesize[layer].x;
		Scren->ree.y = Layers->ree.y = maindata.tilesize[layer].y;
	} else 
	{
		Layers->width = InObjectlist.data[InObjectlist.selobject].width;
		Layers->height = InObjectlist.data[InObjectlist.selobject].height;
		Layers->m_virImg = InObjectlist.data[InObjectlist.selobject].image;
	}
	Layers->Is_Haveimage = TRUE;
	ViewTiles();
	InvalidateRect(Layers->Dibhwnd,NULL,FALSE);
}