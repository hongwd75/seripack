#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<io.h>
//#include <dir.h>
#include<dos.h>


typedef struct {
	unsigned char B;
	unsigned char G;
	unsigned char R;
} RGBstruct;


int		     img_width,img_height;


BOOL ReadBMPfile(char *filename);		// BMP file load

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
			sprintf(ff,"BMP file Read Error : %s",filename);
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

	char outname[40];
	sprintf(outname,"%s.attr",filename);
	FILE *ofn;
	BYTE *data;
	data = new BYTE[img_height*img_width];
	ofn = fopen(outname,"wb");
	for(int loopred = 0; loopred < img_height*img_width; loopred++)
	{
		data[loopred] = m_virImg[loopred].R >> 5;
	}

	fwrite(&img_width,4,1,ofn);
	fwrite(&img_height,4,1,ofn);
	fwrite(data,img_height*img_width,1,ofn);
	fclose(ofn);
	delete m_virImg;
	return TRUE;
}


void main(int argc,char *argv[])
{
  printf("==============================================================================\n");
  printf("  Attribute manager                    for Seri\n");
  printf("  --------------------------------------------------------------------------  \n");
  printf("\n BMP2attr [bmp file name]\n");


  ReadBMPfile(argv[1]);
  printf("==============================================================================\n");
}
