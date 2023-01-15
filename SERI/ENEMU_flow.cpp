#include"stdafx.h"
#include"superobjectclass.h"


#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))


extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
//extern clFileManager		 SeriROM;


mChar *flower;

void GetFLOWSprite()
{
	sprintf(GetFilePath,"%sdata\\flow.res",RunPath);
	flower = new mChar(Videomanager,GetFilePath);
	//ram->mImageLoad(SeriROM.SetFilePosition("ram.res"));
}

// 이미지 프리
void DropFLOWSprite()
{
	delete flower;
}


ENEMYflow::ENEMYflow(int px,int py)
{
	type = _ENEMY_FLOW;

	status = ON_LAND;

	x = px;
	y = py;
	count = 0;

	life = 30;
	frame = 0;
	lookright = 0;
	fade = 31;
	invincibility = 0;
	motion = _STAND_L-lookright;
	flower->iSendMessage(_dolINIT,NULL,0,0);
	//flower->iSendMessage(_dolRESETMOTION,NULL,_STAND_L-lookright,0);
	this->LoadMask(NULL,SeriROM.SetFilePosition("flowmask.dat"));
	isize = flower->GetImageSize();
}


void ENEMYflow::action(void)
{
	
	switch(status)
	{
		case ON_LAND:
				if(!IsMove(MOVEOK_DOWN,1))
				{
					status = FALLING;
				} else 
				{
					count++;
					if(count==20)
					{
						count=0;
					}
				}
			break;

		case FALLING:
				if(this->IsMove(MOVE_DOWN,9)) 
				{
					status = ON_LAND;
				}
			break;
	}
}


void ENEMYflow::putSprite(void)
{
	POINT temppos;
	temppos.x = x - mapmanager->layer2.x;
	temppos.y = y - mapmanager->layer2.y-16;
	if(flower->iSendMessage(_dolRESETMOTION,NULL,motion,frame))
	{
		frame = 1;
	} else frame++;
	flower->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,10);

}

void ENEMYflow::SendState(int j)
{
}