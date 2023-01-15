#include"stdafx.h"
#include<stdlib.h>
#include"imageprocess.h"


//////////////////////////////////////////////////////////////////////////////
//
//
extern clSCREEN			 *VIDEO;
#define  PutIMG(x,y,w,h,img)        VIDEO->PutIMG4(x,y,w,h,img)



typedef struct tagImageList* LPImageList;

//////////////////////////////////////////////////////////////////////////////
//  클래스 초기화
// 
ImagePorcessClass::ImagePorcessClass(clSCREEN *vv)
{
	int loop;

	this->VIDEO = vv;
	for(loop=0;loop<_MaxProcess;loop++)
		mBasememory[loop] = new (struct tagImageList);		// memory malloc
}




//////////////////////////////////////////////////////////////////////////////
//  클래스 파괴 
// 
ImagePorcessClass::~ImagePorcessClass()
{
	int loop;

	for(loop=0;loop<_MaxProcess;loop++)
		delete mBasememory[loop];

}




/////////////////////////////////////////////////////////////////////////////
// 이미지 리스트 초기화
//
void ImagePorcessClass::InitImageList(void)
{
	ImageCount = 0;
	Header=NULL;
	//Tail=NULL;
}




/////////////////////////////////////////////////////////////////////////////
// 이미지 프로세스 리스트에 추가
//
void ImagePorcessClass::AddList(int x,int y,int options,int layers,int ID,void *data,int op1=0,int op2=0)
{
	

	if(ImageCount >= _MaxProcess) return;
	LPImageList temp = mBasememory[ImageCount++];
	LPImageList header2,prev;

	int *intpoint = (int *)temp->Parameters;

	temp->ID = ID;
	temp->LayerLevel = layers;
	temp->next = NULL;

	intpoint[0] = x;
	intpoint[1] = y;
	temp->ProcessCommand = options;
	intpoint[2] = options;
	intpoint[3] = layers;
	intpoint[4] = op1;
	intpoint[5] = op2;

	if(ID == _ID_CHAR)
	{
		strcpy((LPSTR)(temp->Parameters+25),(LPSTR)data);
		//temp->LPImageClass = data;
	} else temp->LPImageClass = data;

	if(Header == NULL)
	{
		Header = temp;
		return;
	} 
	else
	{
		header2 = Header;
		prev    = Header;
		while(header2 != NULL)
		{
			if( header2->LayerLevel > temp->LayerLevel )
			{
				temp->next = header2;
				if(header2 == Header)
				{
					Header = temp;
				} else 
				{
					prev->next = temp;
				}
				return;
			}
			prev = header2;
			header2 = header2->next;
		}
		prev->next = temp;
	}
}




void ImagePorcessClass::AddSCREENEFFECT(int data, int imageoption, int layers)
{
	AddList(data,0,imageoption,layers,_ID_SCREEN,NULL);
}




/////////////////////////////////////////////////////////////////////////////
// 스프라이트를 리스트에 추가 
//
void ImagePorcessClass::AddSpr(int x, int y, void *data, int imageoption, int layers,int op)
{
	pIMAGE *data2 = (pIMAGE *)data;
	
    if(data == NULL) return;
	if(data2->ImageType == _IMAGE_COMP) AddList(x,y,imageoption,layers,_ID_SPR2,data,op);
	else AddList(x,y,imageoption,layers,_ID_SPR,data,op);
}



/////////////////////////////////////////////////////////////////////////////
//
void ImagePorcessClass::AddPoly(int x,int y,void *data,int sized,int rote,int layers)
{
	AddList(x,y,sized,layers,_ID_POLY,data,rote);
	//void ImagePorcessClass::AddList(int x,int y,int options,int layers,int ID,void *data,int op1=0,int op2=0)
}



/////////////////////////////////////////////////////////////////////////////
// 이미지를 리스트에 추가 
//
void ImagePorcessClass::AddImage(int x, int y, void *data, int imageoption, int layers)
{
	AddList(x,y,imageoption,layers,_ID_IMAGE,data);
}



/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
// 문자를 리스트에 추가 
//
void ImagePorcessClass::AddText(int x,int y,int color,LPSTR message,int CHARoption,int layers,int OP1)
{
	AddList(x,y,CHARoption,layers,_ID_CHAR,message,color,OP1);
}



/////////////////////////////////////////////////////////////////////////////
// 
//
void ImagePorcessClass::DrawImageList(void)
{
	LPImageList hder = Header;
/*
#define  PutSPR(x,y,w,h,img)		VIDEO->PutSprMMX4(x,y,w,h,img)
#define  PutIMG(x,y,w,h,img)        VIDEO->PutImgMMX4(x,y,w,h,img)
#define  PutSPRAL(x,y,img)			VIDEO->PutAlpha(x,y,img)		//알파블렌딩용
#define  PutSPRDK(x,y,img)			VIDEO->PutDark(x,y,img)		//알파블렌딩용
#define  PutHALFSPR(x,y,w,h,img)    VIDEO->halfSpr(x,y,w,h,img)

*/	

	while(hder != NULL)
	{
		switch(hder->ID)
		{
			case _ID_POLY:
				{
					int *intpoint = (int *)hder->Parameters;
					pIMAGE *temps = (pIMAGE *)hder->LPImageClass;

					VIDEO->PutSprZR(intpoint[0],intpoint[1],temps->image,temps->width,temps->height,intpoint[4],intpoint[2]);
					
				}
			break;
			case _ID_CHAR:
				{
					int *intpoint = (int *)hder->Parameters;
					char *strings = (char *)(hder->Parameters+25);
					if(intpoint[2] == 1)
						this->VIDEO->TextPut2(intpoint[0],intpoint[1],strings,intpoint[4]);
					else this->VIDEO->TextPut(intpoint[0],intpoint[1],strings,intpoint[4]);
				}
				break;
			case _ID_SPR:
				{
					
					int *intpoint = (int *)hder->Parameters;
					pIMAGE *temps = (pIMAGE *)hder->LPImageClass;
					switch(hder->ProcessCommand)
					{
						case _NORMALMODE: case _ANTIMODE:
							this->VIDEO->PutSpr4(intpoint[0],intpoint[1],temps->width,temps->height,temps->image);
							break;
						case _ALPHAMODE:
							this->VIDEO->PutAlpha(intpoint[0],intpoint[1],temps);
							break;
						case _DARKMODE:
							this->VIDEO->PutDark(intpoint[0],intpoint[1],temps);
							break;
						case _HALFTONEMODE:
							this->VIDEO->halfSpr(intpoint[0],intpoint[1],temps->width,temps->height,temps->image);
							break;
						case _FADEMODE:
							this->VIDEO->fadeSpr(intpoint[0],intpoint[1],temps->width,temps->height,intpoint[4],temps->image);
							break;

							break;
					}
					
				}
				break;
			case _ID_SPR2:
				{
					int *intpoint = (int *)hder->Parameters;
					pIMAGE *temps = (pIMAGE *)hder->LPImageClass;
					switch(hder->ProcessCommand)
					{
						case _NORMALMODE:
							this->VIDEO->CmpSpr(intpoint[0],intpoint[1],temps);
							break;
						case _ANTIMODE:
							this->VIDEO->CmpAntiSpr(intpoint[0],intpoint[1],temps);
							break;
						case _HALFTONEMODE:
							this->VIDEO->CmpHalf(intpoint[0],intpoint[1],temps);
							break;
						case _DHALFTONEMODE:
							this->VIDEO->cmpHalfdouble(intpoint[0],intpoint[1],temps);
							break;
						case _LIGHTMODE:
							this->VIDEO->cmpLIGHTSpr(intpoint[0],intpoint[1],temps);
							break;
						case _FADEMODE:
							this->VIDEO->cmpFadeSpr(intpoint[0],intpoint[1],intpoint[4],temps);
							break;

					}
				}
				break;
			case _ID_IMAGE:
				{
					int *intpoint = (int *)hder->Parameters;
					pIMAGE *temps = (pIMAGE *)hder->LPImageClass;
					if(temps==NULL) return;
					switch(hder->ProcessCommand)
					{
						case _NORMALMODE:
							//this->VIDEO->PutImgMMX4(intpoint[0],intpoint[1],temps->width,temps->height,temps->image );
							this->VIDEO->PutIMG4(intpoint[0],intpoint[1],temps->width,temps->height,temps->image );
						break;
						case _HALFTONEMODE:
							this->VIDEO->PutHalfIMG(intpoint[0],intpoint[1],temps->width,temps->height,temps->image );
						break;
						case _CLIPWIDTH:
							this->VIDEO->PutIMG4Clp(intpoint[0],intpoint[1],temps->width,temps->height,temps->image );
							break;
					}
				}
				break;
			case _ID_SCREEN:
				{
					int *intpoint = (int *)hder->Parameters;
					switch(hder->ProcessCommand)
					{
						case _HALFSCREEN:
							this->VIDEO->halfScreen((WORD)intpoint[0]);
						break;
					}

				}
				break;
		}
		hder = hder->next;
	}

}
