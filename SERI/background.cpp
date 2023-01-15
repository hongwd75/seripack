#include"stdafx.h"
#include"background.h"
#include"filemanager.h"


//#define					LAYER2SIZE				256
//#define                 LAYER3SIZE				64

extern CLSeriManager		*SeriData;
extern clFileManager		SeriROM;
extern clFileManager		MiniROM;
extern tagSeriRELOADSystem tpSERISYSTEM;

int  ARseriSPEED[5] = {200,100,40,20,0};
/////////////////////////////////////////////////
//
//
CLBackground::CLBackground(ImagePorcessClass *dds)
{
	int i;

	for(i=0;i<LAYER1SIZE;i++) this->iLayer1[i] = NULL;
	for(i=0;i<LAYER2SIZE;i++) this->iLayer2[i] = NULL;
	for(i=0;i<LAYER3SIZE;i++) this->iLayer3[i] = NULL;
	
	this->iDisplay = dds;
	this->maploaded = FALSE;
	this->mlayer1 = NULL;
	this->mlayer2 = NULL;
	this->mlayer3 = NULL;
	this->mapattribute = NULL;

	this->ForceScroll = FALSE;
	this->Scrollcount = 0;

}

CLBackground::~CLBackground(void)
{
	this->Clearmem();
}

void CLBackground::Clearmem(void)
{
	if(this->maploaded)
	{
		int i;
		for(i=0;i<LAYER1SIZE;i++) 
		{
			if(this->iLayer1[i] != NULL) 
			{
				delete this->iLayer1[i];
				this->iLayer1[i] = NULL;
			}
		}

		for(i=0;i<LAYER2SIZE;i++) 
		{
			if(this->iLayer2[i] != NULL) 
			{
				delete this->iLayer2[i];
				this->iLayer2[i] = NULL;
			}
		}
		for(i=0;i<LAYER3SIZE;i++) 
		{
			if(this->iLayer3[i] != NULL) 
			{
				delete this->iLayer3[i];
				this->iLayer3[i] = NULL;
			}
		}
		if(this->mlayer1 != NULL) 
		{
			//delete this->mlayer1;
			free(mlayer1);
			this->mlayer1 = NULL;
		}
		if(this->mlayer2 != NULL) 
		{
			//delete (this->mlayer2);
			free(mlayer2);
			this->mlayer2 = NULL;
		}
		if(this->mlayer3 != NULL) 
		{
			//delete this->mlayer3;
			free(mlayer3);
			this->mlayer3 = NULL;
		}
		if(mapattribute != NULL) 
		{
			free(mapattribute);
			mapattribute = NULL;
		}
	}
	this->maploaded = FALSE;
}
////////////////////////////////////////////////
//
//
int CLBackground::initBackground(LPSTR filename)
{
	


	FILE *fp;
	BOOL saved[4];
	Noscroll = FALSE;
	fp = fopen(filename,"rb");
	int sizes,i;
	this->Clearmem();
	/////////////////////////////////////////////////////////////////////
	// Layer 1 타일 데이타 읽는다.
	fread(&sizes,4,1,fp);
	if(sizes != 0xFFFF)
	{
		for(i=0;i<=sizes;i++)
		{
			this->iLayer1[i] = new pIMAGE;
			this->iLayer1[i]->Load("###",fp);
		}
		this->tilesize[0].x = this->iLayer1[sizes-1]->width;
		this->tilesize[0].y = this->iLayer1[sizes-1]->height;
		saved[0]=TRUE;
	} else saved[0]=FALSE;
	/////////////////////////////////////////////////////////////////////
	// Layer 2 타일 데이타 읽는다.
	fread(&sizes,4,1,fp);
	if(sizes != 0xFFFF)
	{
		for(i=0;i<=sizes;i++)
		{
			this->iLayer2[i] = new pIMAGE;
			this->iLayer2[i]->Load("###",fp);
		}
		this->tilesize[1].x = this->iLayer2[sizes-1]->width;
		this->tilesize[1].y = this->iLayer2[sizes-1]->height;
		saved[1]=TRUE;
	} else saved[1]=FALSE;

	/////////////////////////////////////////////////////////////////////
	// Layer 3 타일 데이타 읽는다.
	fread(&sizes,4,1,fp);
	if(sizes != 0xFFFF)
	{
		for(i=0;i<=sizes;i++)
		{
			this->iLayer3[i] = new pIMAGE;
			this->iLayer3[i]->Load("###",fp);
		}
		this->tilesize[2].x = this->iLayer3[sizes-1]->width;
		this->tilesize[2].y = this->iLayer3[sizes-1]->height;
		saved[2]=TRUE;
	} else saved[2]=FALSE;

	saved[3] = TRUE;

	// ************************************************************
	// 맵 데이타 읽는다

	int xx,yy;
	for(i = 0; i < 4; i++)
	{
		width[i]=1;
		height[i]=1;
		if(!saved[i]) continue;
		fread(&xx,4,1,fp);
		fread(&yy,4,1,fp);
		width[i]=xx;
		height[i]=yy;
		switch(i)
		{
			case 0:
				{
					int psize1 = xx*yy;
					if (psize1<4096) psize1 = 4096;
					this->mlayer1 = (BYTE*)malloc(psize1);//new BYTE[psize1];
					fread((BYTE *)this->mlayer1,xx*yy,1,fp);
				}
				break;

			case 1:
				{
					int psize2 = xx*yy;
					if (psize2<4096) psize2 = 4096;
					this->mlayer2 = (BYTE*)malloc(psize2);//new BYTE[psize2];
					fread((BYTE *)this->mlayer2,xx*yy,1,fp);
				}
				break;

			case 2:
				{
					int psize3 = xx*yy;
					if (psize3<4096) psize3 = 4096;
					this->mlayer3 = (BYTE*)malloc(psize3);//new BYTE[psize3];
					fread((BYTE *)this->mlayer3,xx*yy,1,fp);
				}
				break;

			case 3:
				{
					int psize = xx*yy;
					if (psize<4096) psize = 4096;
					this->mapattribute =(BYTE*)malloc(psize);
					fread((BYTE *)this->mapattribute,xx*yy,1,fp);
				}
				break;
		}
	}

	fclose(fp);

	this->layer2.x = 0;
	this->layer2.y = 0;

	this->maploaded = TRUE;


	this->L1.x = (float)(this->width[0] << 5 ) / (this->width[1]  << 5);
	this->L1.y = (float)(this->height[0] << 5 ) / (this->height[1]  << 5);

	this->L3.x = (float)(this->width[2]  << 5 ) / (this->width[1] << 5);
	this->L3.y = (float)(this->height[2]  << 5 ) / (this->height[1] << 5);


	// 레이어 2에 따른 1과 3의 증감치
	return _MAPREADY;

}

int CLBackground::iMessage(int mID,void *mMessage,int LParam,int RParam)
{
	switch(mID)
	{

		case _ID_SCROLL_FORCE_RIGHT:
			{

				if(this->width[1] > ((this->layer2.x+LAYER2ADD) >> 5)+20 && !Noscroll)
				{
					this->layer2.x+=4;
					/*
					ctx = ( 640 / this->tilesize[0].x) + 1;
					px = (int)( this->layer2.x * this->L1.x );
					sttx = px / this->tilesize[0].x;
					addx = -(px % this->tilesize[0].x);

					ctx1 = ( 640 / this->tilesize[2].x) + 1;
					px1 = (int)( this->layer2.x * this->L3.x );
					sttx1 = px1 / this->tilesize[2].x;
					addx1 = -(px1 % this->tilesize[2].x);
					*/
				}
			}
			break;

		case _ID_SHOW2:
			{
				int mapnum;
				int laa = addx;

				if(ForceScroll)
				{
					this->iMessage(_ID_SCROLL_FORCE_RIGHT,NULL,0,0);
				}

				if(this->mlayer1 != NULL)
				{
					for(int l1=0;l1<21;l1++)
					{
						for(int l1a=0;l1a<15;l1a++)
						{
							mapnum = this->mlayer1[this->width[1]*((this->layer2.y >> 5)+l1a) + ((this->layer2.x >> 5)+l1)];
							if(mapnum>0) iDisplay->AddImage(-(this->layer2.x & 31)+(l1 << 5),(l1a << 5),
								this->iLayer1[mapnum-1],_NORMALMODE,2);
						}
					}
				}

				int ldd = addy1;
				
				for(int l2=0;l2<21;l2++)
				{
					for(int l2a=0;l2a<15;l2a++)
					{
						
						if(l2 < this->width[1])
						{
							mapnum = this->mlayer2[this->width[1]*((this->layer2.y >> 5)+l2a) + ((this->layer2.x >> 5)+l2)];
						
							if(mapnum>0) iDisplay->AddSpr(-(this->layer2.x & 31)+(l2 << 5),(l2a << 5),
								this->iLayer2[mapnum],_NORMALMODE,3);
						}

					}
				}
			}
			break;
		case _ID_SHOW:
			{
				int mapnum;
				int laa = addx;

				if(ForceScroll)
				{
					this->iMessage(_ID_SCROLL_FORCE_RIGHT,NULL,0,0);
				}
				if(this->mlayer1 != NULL)
				{
					for(int l1=0;l1<21;l1++)
					{
						for(int l1a=0;l1a<15;l1a++)
						{
							mapnum = this->mlayer1[this->width[1]*((this->layer2.y >> 5)+l1a) + ((this->layer2.x >> 5)+l1)];
							if(mapnum>0) iDisplay->AddImage(-(this->layer2.x & 31)+(l1 << 5),-(this->layer2.y & 31)+(l1a << 5),
								this->iLayer1[mapnum-1],_NORMALMODE,2);
						}
					}
				}

				int ldd = addy1;
				if(this->mlayer3 != NULL)
				{
					for(int l3=0;l3<21;l3++)
					{
						for(int l3a=0;l3a<15;l3a++)
						{
							mapnum = this->mlayer3[this->width[1]*((this->layer2.y >> 5)+l3a) + ((this->layer2.x >> 5)+l3)];
							if(mapnum>0) iDisplay->AddSpr(-(this->layer2.x & 31)+(l3 << 5),-(this->layer2.y & 31)+(l3a << 5),
								this->iLayer3[mapnum],_NORMALMODE,4);
						}
					}
				}

				for(int l2=0;l2<21;l2++)
				{
					for(int l2a=0;l2a<15;l2a++)
					{
						
						if(l2+(this->layer2.x >> 5) < this->width[1])
						{
							mapnum = this->mlayer2[this->width[1]*((this->layer2.y >> 5)+l2a) + ((this->layer2.x >> 5)+l2)];
						
							if(mapnum>0) iDisplay->AddSpr(-(this->layer2.x & 31)+(l2 << 5),-(this->layer2.y & 31)+(l2a << 5),
								this->iLayer2[mapnum],_NORMALMODE,3);
						}
					}
				}
			}
			break;

		case _ID_SCROLL_SET:
			if(LParam <0) LParam = 0;
			if(RParam <0) RParam = 0;

			if((LParam+640) >> 5 < width[1]-1) layer2.x = LParam;
				else layer2.x = ((width[1]-1) << 5) - 640;
			if((RParam+448) >> 5 < height[1]) layer2.y = RParam;
				else layer2.y = (height[1] << 5) - 480;
/*
			ctx = ( 640 / this->tilesize[0].x) + 1;
			if(this->height[0] * this->tilesize[0].y < 480)	cty = this->height[0];
			else cty = ( 480 / this->tilesize[0].y) + 1;
			px = (int)( this->layer2.x * this->L1.x );
			py = (int)( this->layer2.y * this->L1.y );
			addy = -(py % this->tilesize[0].y);
			sttx = px / this->tilesize[0].x;
			stty = py / this->tilesize[0].y;
			addx = -(px % this->tilesize[0].x);

			ctx1 = ( 640 / this->tilesize[2].x) + 1;
			cty1 = ( 480 / this->tilesize[2].y) + 1;
			px1 = (int)( this->layer2.x * this->L3.x );
			py1 = (int)( this->layer2.y * this->L3.y );
			addy1 = -(py1 % this->tilesize[2].y);
			sttx1 = px1 / this->tilesize[2].x;
			stty1 = py1 / this->tilesize[2].y;
			addx1 = -(px1 % this->tilesize[2].x);
*/
			break;

		case _ID_SCROLL_LEFT:
			{
				if(!ForceScroll && !Noscroll)
				{
					if(this->layer2.x - LAYER2ADD > 0)
					{
						this->layer2.x-=LAYER2ADD;
					}
				}
			}
			break;

		case _ID_SCROLL_RIGHT:
			{
				if(this->width[1] > ((this->layer2.x+LAYER2ADD) >> 5)+20 && !Noscroll)
				{
					this->layer2.x+=LAYER2ADD;

				}
			}
			break;

		case _ID_SCROLL_UP:
			{
				if(this->layer2.y - LAYER2ADD > 0)	
				{
					this->layer2.y-=LAYER2ADD;
/*			if(this->height[0] * this->tilesize[0].y < 480)	cty = this->height[0];
					else cty = ( 480 / this->tilesize[0].y) + 1;

				//	cty   = ( 480 / this->tilesize[0].y) + 1;
					py    = (int)( this->layer2.y * this->L1.y );
					addy  = -(py % this->tilesize[0].y);
					stty  = py / this->tilesize[0].y;
					cty1  = ( 480 / this->tilesize[2].y) + 1;
					py1   = (int)( this->layer2.y * this->L3.y );
					addy1 = -(py1 % this->tilesize[2].y);
					stty1 = py1 / this->tilesize[2].y;*/
				}
			}
			break;

		case _ID_SCROLL_DOWN:
			{
				if(((this->layer2.y+LAYER2ADD) >> 5)+14 < this->height[1])
				{
					this->layer2.y+=LAYER2ADD;
/*			if(this->height[0] * this->tilesize[0].y < 480)	cty = this->height[0];
					else cty = ( 480 / this->tilesize[0].y) + 1;
//					cty   = ( 480 / this->tilesize[0].y) + 1;
					py    = (int)( this->layer2.y * this->L1.y );
					addy  = -(py % this->tilesize[0].y);
					stty  = py / this->tilesize[0].y;
					cty1  = ( 480 / this->tilesize[2].y) + 1;
					py1   = (int)( this->layer2.y * this->L3.y );
					addy1 = -(py1 % this->tilesize[2].y);
					stty1 = py1 / this->tilesize[2].y;*/
				}
			}
			break;
	}
	return 0;
}



////////////////////////////////////////////////
//
//
BYTE CLBackground::GetMapAttribute(int x,int y)
{
	int px = x;
	int py = y;
	
	if (px<0) px = 0;
	if (py<0) py = 0;
	if (px>this->width[3]-1) px = (this->width[3]) - 1;
	if (py>this->height[3]-1) py = (this->height[3]) - 1;

	
	return (this->mapattribute[py * this->width[3] + px]);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

//	초기화 
CLSeriManager::CLSeriManager(ImagePorcessClass *dds)
{
	this->iDisplay = dds;
	this->stage = 1;
	this->addpower = 0;
	this->score = 0;
	this->life  = 3;
	for(int i = 0; i < 5; i++)
	{
		this->Item[i] = 0;
	}
	SetSPEED = 2;
	SetLife = 3;
}

CLSeriManager::~CLSeriManager(void)
{
	this->Destroys();
}


//   이미지 로드
void CLSeriManager::Load(void)
{
  int i;
  char fff[50];

  this->iSerilife = new pIMAGE();
  this->iSerilife->Load(NULL,MiniROM.SetFilePosition("gball.spr"));
  this->ibackboard = new pIMAGE();
  this->ibackboard->Load(NULL,SeriROM.SetFilePosition("backboard.spr"));

  this->iImage[0] = new pIMAGE();
  this->iImage[0]->Load(NULL,MiniROM.SetFilePosition("card1.spr"));
  this->iImage[1] = new pIMAGE();
  this->iImage[1]->Load(NULL,MiniROM.SetFilePosition("card2.spr"));
  this->iImage[2] = new pIMAGE();
  this->iImage[2]->Load(NULL,MiniROM.SetFilePosition("card3.spr"));
  this->iImage[3] = new pIMAGE();
  this->iImage[3]->Load(NULL,MiniROM.SetFilePosition("card8.spr"));
  this->iImage[4] = new pIMAGE();
  this->iImage[4]->Load(NULL,MiniROM.SetFilePosition("card6.spr"));
  this->Gage = new pIMAGE();
  this->Gage->Load(NULL,MiniROM.SetFilePosition("gage.spr"));	

  for(i = 0;i <10; i++)
  {
	  this->iNumber[i] = new pIMAGE();
	  sprintf(fff,"no_%d.spr",i);
	  iNumber[i]->Load(NULL,MiniROM.SetFilePosition(fff));
  }
  	  this->iNumber[10] = new pIMAGE();
	  iNumber[10]->Load(NULL,MiniROM.SetFilePosition("no_-.spr"));

}

void CLSeriManager::Destroys(void)
{

	delete this->iSerilife;
	delete this->ibackboard;
	delete this->Gage;
	for(int j=0;j<5;j++)
	{
		delete this->iImage[j];
	}
	for(int i=0;i<11;i++)
	{
		delete this->iNumber[j];
	}

}


void CLSeriManager::Display(void)
{
	this->iDisplay->AddSpr(0,0,this->ibackboard,_NORMALMODE,256);
	//this->iDisplay->AddSpr(60,5,this->iNumber[this->stage],_NORMALMODE,257);
	ShowStageNum(60,5);
	ShowScore(297,449);
	ShowSerilife(590,449);
	for(int i = 0; i < 5; i++)
	{
		if(this->Item[i]>0)
		{
			this->iDisplay->AddImage(7+(i*39),423,this->iImage[this->Item[i]-1],_NORMALMODE,257);
		}
	}

}


///////////////////////////////////////////////////////////////////
//
// 1번부터 
int CLSeriManager::UsesCard(int hu)
{
	if(this->Item[hu-1])
	{
		int ret = this->Item[hu-1];
		this->Item[hu-1] = 0;
		tpSERISYSTEM.BattleCard[hu-1] = 0;
		return ret;
	}
	return 0;
}


///////////////////////////////////////////////////////////////////
//
//

extern tagSeriRELOADSystem tpSERISYSTEM;


void CLSeriManager::init(int rlife,int ritemsize,int rscore,int level)
{
	if(rlife > 0 ) 
	{
		this->life = rlife;
		this->SetLife = rlife;
	} else 
	{
		this->life = this->SetLife;
	}
	if(this->SetLife> SERIMAXENEGY) this->SetLife =  SERIMAXENEGY;
	//this->addpower = 0;
	//this->itemsize = ritemsize;
	if(rscore >= 0 ) this->score = rscore;
	this->addpower = level;
	resetjumsu();
	gameend = 0;
}

void CLSeriManager::Loadinit(void)
{
	life     = tpSERISYSTEM.life;
	SetLife  = tpSERISYSTEM.maxlife;
	addpower = tpSERISYSTEM.addpower;
	score    = tpSERISYSTEM.score;
	for(int i =0; i < 5; i++) Item[i] = tpSERISYSTEM.BattleCard[i];
	resetjumsu();
	gameend = 0;
}


void CLSeriManager::SetStage(int ii,int ii2)
{
	this->stage = ii;
	this->dashstage = ii2;
	gameend = 0;
	SetGemeSpeed();
}


void CLSeriManager::RepiarEnegy(int ss)							// 에너지 추가
{

}


void CLSeriManager::SetSeriNum(int ss)							// 세리 마리수 수정 
{
	this->life = ss;
	this->SetLife = ss;
}


void CLSeriManager::ShowStageNum(int x, int y)
{
   this->iDisplay->AddSpr(x,y,this->iNumber[this->stage],_NORMALMODE,257);
   if(this->dashstage>0)
   {
	   this->iDisplay->AddSpr(x+24,y,this->iNumber[10],_NORMALMODE,258);
	   this->iDisplay->AddSpr(x+48,y,this->iNumber[this->dashstage],_NORMALMODE,257);
   }
}

void CLSeriManager::ShowSerilife(int x,int y)
{
	for(int i =0;i<this->life;i++)
	iDisplay->AddSpr(x-(i*20),y,this->iSerilife,_NORMALMODE,257);
}


void CLSeriManager::ShowScore(int x, int y)
{
	char tempstr[6];
	sprintf(tempstr,"%6d",this->score);
	for(int i = 0; i < 6; i++) 
	{
		if(tempstr[i] >='0')
		iDisplay->AddSpr(x+(i*23),y,this->iNumber[tempstr[i] - '0'],_NORMALMODE,257);
	}

}



void CLSeriManager::ShowSeriGage(int i)
{
	int steps = 59 - i;
	if(steps<59 && steps>0) for(int i = 0; i < steps; i++) this->iDisplay->AddImage(232-(i*4),473,this->Gage,_NORMALMODE,257);
}

void CLSeriManager::ResetSeriData(int i)
{
	this->addpower = 0;
	this->life = this->SetLife;
	this->score = 0;
	gameend = 0;
	for(int ix = 0 ; ix < 5 ; ix++) this->Item[ix] = 0;
	if(i>0) this->stage = i;
	SetGemeSpeed();
}


void CLSeriManager::SetGemeSpeed(void)
{
	iDisplay->VIDEO->Refresh = ARseriSPEED[SetSPEED];
}


void CLSeriManager::ResetCardSystem(void)
{
	for(int i=0;i < 5; i++) this->Item[i] = 0;
}


void CLSeriManager::resetjumsu(void)
{
	killed = 0;
	many = 0;
	eat = 0;
}