#include"stdafx.h"
#include"doll.h"

#include"background.h"


///////////////////////////////////////////////////////////////////////
                      // 케릭터 클레스 입니다요 //
///////////////////////////////////////////////////////////////////////
POINT mChar::GetImageSize()
{
	POINT irets;

	irets.x  = motionimage[nframeinfo[this->iframe].Imagenumber]->width;
	irets.y  = motionimage[nframeinfo[this->iframe].Imagenumber]->height;
	return irets;
}

POINT mChar::GetMotionNumber()
{
	POINT irets;

	irets.x = this->Delaycount;
	irets.y = this->iframe;

	return irets;
}

mChar::mChar(ImagePorcessClass *mm,LPSTR filename)
{
	this->mDisplay = mm;
	this->mImageLoad(filename);
	this->iSendMessage(_dolINIT,NULL,0,0);
}
mChar::mChar(ImagePorcessClass *mm)
{
	this->mDisplay = mm;
}


//////////////////////////////////////////////////////////
//  클래스 죽이기 
//
mChar::~mChar()
{
	int i;
	for(i=0;i<this->Imagenum;i++) delete this->motionimage[i];
	for(i=0;i<this->Motionnum;i++) 
	{
		delete this->frameinfo[i];
	}
}

//////////////////////////////////////////////////////////
//  이미지를 읽는다.
//
void mChar::mImageLoad(LPSTR filename)
{
	int  i,j;
	FILE *fp;
	char HEADER[4];
	DOLLINFO *tempdoll;


	fp = fopen(filename,"rb");
	fread((void *)HEADER,3,1,fp);
	fread(&this->Imagenum,4,1,fp);

	for(i=0;i<this->Imagenum;i++)
	{
		this->motionimage[i] = new pIMAGE();
		this->motionimage[i]->Load("###",fp);
	}
	fread(&this->Motionnum,4,1,fp);
	
	
	for(i=0;i<this->Motionnum;i++)
	{
		fread(&this->framesize[i],4,1,fp);
		this->frameinfo[i] = new DOLLINFO[this->framesize[i]];
		tempdoll = this->frameinfo[i];
		for(j=0;j<this->framesize[i];j++)
		{
			fread(&tempdoll[j].Imagenumber,4,1,fp);
			fread(&tempdoll[j].delay,4,1,fp);
			fread(&tempdoll[j].mypos,sizeof(POINT),1,fp);
		}
	}
	fclose(fp);
}


void  mChar::mImageLoad(FILE *fpx)
{
	int  i,j;
	char HEADER[4];
	DOLLINFO *tempdoll;

	FILE *fp =fpx;

    if(fpx == NULL) MessageBox(NULL,"시비 없잖아","Error",MB_OK | MB_ICONSTOP);
	fread((void *)HEADER,3,1,fp);
	fread(&this->Imagenum,4,1,fp);

	for(i=0;i<this->Imagenum;i++)
	{
		this->motionimage[i] = new pIMAGE();
		this->motionimage[i]->Load("###",fp);
	}
	fread(&this->Motionnum,4,1,fp);
	
	
	for(i=0;i<this->Motionnum;i++)
	{
		fread(&this->framesize[i],4,1,fp);
		this->frameinfo[i] = new DOLLINFO[this->framesize[i]];
		tempdoll = this->frameinfo[i];
		for(j=0;j<this->framesize[i];j++)
		{
			fread(&tempdoll[j].Imagenumber,4,1,fp);
			fread(&tempdoll[j].delay,4,1,fp);
			fread(&tempdoll[j].mypos,sizeof(POINT),1,fp);
		}
	}

	fclose(fp);
}

/////////////////////////////////////////////////////////////////////
// 메세지 주고 받기 
int mChar::iSendMessage(int ID,void *message,long LPARAM,long RPARAM)
{
  switch(ID)
  {
		/////////////////////////////
		case   _dolINIT:
			{
				this->iframe = 0;
				this->nframeinfo = this->frameinfo[0];
				this->Delaycount = this->nframeinfo[0].delay; // 마지막으로 딜레이 세팅.
				this->lengthframe = this->framesize[0];
			}
			break;

		case _dolNEXTFRAME:
			{
				if(this->Delaycount == 0) 
				{
					this->iframe++;
					
					if(this->iframe < this->lengthframe)
					{
						this->Delaycount = this->nframeinfo[this->iframe].delay;
					} else
					{
						this->iframe = 0;			        // 프레임은 당삼 0부터 시작하고.
						this->Delaycount = this->nframeinfo[0].delay; // 마지막으로 딜레이 세팅.
						return _RESETFRAME;
						}
				} else this->Delaycount--;
			}
			break;

		case _dolPROCESS:
			{
				if(this->Delaycount == 0) 
				{
					this->iframe++;
					
					if(this->iframe < this->lengthframe)
					{
						this->Delaycount = this->nframeinfo[this->iframe].delay;
					} else
					{
						this->iframe = 0;			        // 프레임은 당삼 0부터 시작하고.
						this->Delaycount = this->nframeinfo[0].delay; // 마지막으로 딜레이 세팅.
						}
				} else this->Delaycount--;
			}
			break;

		/////////////////////////////
		case _dolSHOW:
			{
				POINT *h = (POINT *) message;

		//		this->mDisplay->AddSpr(h->x - (motionimage[nframeinfo[this->iframe].Imagenumber]->width >> 1),
		//			h->y - motionimage[nframeinfo[this->iframe].Imagenumber]->height,
		//			motionimage[nframeinfo[this->iframe].Imagenumber],LPARAM,RPARAM); // imagetype,layer
				this->mDisplay->AddSpr(h->x, h->y,
					motionimage[nframeinfo[this->iframe].Imagenumber],LPARAM,RPARAM); // imagetype,layer

			}
			break;

		case _dolSHOW_ALPHA:
			{
				POINT *h = (POINT *) message;

				this->mDisplay->AddSpr(h->x,
					h->y,
					motionimage[nframeinfo[this->iframe].Imagenumber],_FADEMODE,RPARAM,LPARAM); // imagetype,layer
			}
			break;

		/////////////////////////////	
		case _dolRESETMOTION:
			{
				this->nframeinfo = this->frameinfo[LPARAM];
				this->lengthframe = this->framesize[LPARAM];
			    int ret = 0;
				if(RPARAM < this->lengthframe) this->iframe = RPARAM;
				else 
				{
					this->iframe = 0;
					ret = 1;
				}

				this->Delaycount = this->nframeinfo[0].delay; // 마지막으로 딜레이 세팅.
				return ret;
			}
			break;
  }
  return 0;
}


int   mChar::GetMotionFrame()
{
	return this->lengthframe;
}
//////////////////////////////////////////////////////////
//  월드좌표 세팅
//
void mChar::SetWorldpoint(int x,int y)
{
	this->Worldpoint.x = x;
	this->Worldpoint.y = y;
}
