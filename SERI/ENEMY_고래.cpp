#include"stdafx.h"
#include"superobjectclass.h"

#define GOREALIFE		25
#define FIREDELAY3		30
#define GOREAMAX		500

#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))


extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
extern ObjectBase *Seripoint;
CSound   *goreasound;

mChar *gorea;

RECT gorearect;

int GOREAF[5]  = {0,-12,-24,-12,0};
void GetGOREASprite()
{
	gorea = new mChar(Videomanager);
	gorea->mImageLoad(SeriROM.SetFilePosition("gorae.res"));
	gorearect.top = 49;
	gorearect.left = 14;
	gorearect.right = 100;
	gorearect.bottom = 73;

	goreasound = new CSound();
	goreasound->Initialize(SeriROM.SetFilePositionMMF("drip.wav"),1);			// 공격 2
}

// 이미지 프리
void DropGOREASprite()
{
	delete gorea;
	delete goreasound;
}


ENEMYgorea::ENEMYgorea(int px,int py)
{
	type = _ENEMY_GOREA;

	status = ON_LAND;

	x = px;
	y = py;
	count = 0;
	count2 = 0;

	life = GOREALIFE;
	frame = 0;
	invincibility = 0;
	lookright = 0;
	motion = _STAND_L-lookright;
	gorea->iSendMessage(_dolINIT,NULL,0,0);
	gorea->iSendMessage(_dolRESETMOTION,NULL,_STAND_L-lookright,0);
	this->LoadMask(NULL,SeriROM.SetFilePosition("gorea.msk"));
	isize = gorea->GetImageSize();
	cliprect = (RECT *)&gorearect;
	firedelay = FIREDELAY3;
	if(Seripoint->x < x) lookright = 1;
	motion = _STAND_L-lookright;
	gorea->iSendMessage(_dolRESETMOTION,NULL,_STAND_L-lookright,0);
	fade = 30;
	maxgo = GOREAMAX;
	InScreen = 1;
}



void ENEMYgorea::action(void)
{
	
	switch(status)
	{
		case ON_LAND:
			if(maxgo<=0)
			{
				lookright = 1 - lookright;
				motion  = _STAND_L-lookright;
				maxgo = GOREAMAX;
			}

			if(firedelay <= 0)
			{
				count = 0;
				frame = 0;
				motion = _ATTACK1_L-lookright;
				count2 = 0;
				status = _ATTACKS;
			} else firedelay--;
			if(!lookright)
			{
				if(IsMove(MOVE_RIGHT | MOVE_ONLY_BLANK,2))
				{
					lookright = 1 - lookright;
					motion  = _STAND_L-lookright;
					maxgo = GOREAMAX;
				} else maxgo-=2;
			} else 
			{
				if(IsMove(MOVE_LEFT | MOVE_ONLY_BLANK,2))
				{
					lookright = 1 - lookright;
					motion  = _STAND_L-lookright;
					maxgo = GOREAMAX;
				} else maxgo-=2;
			}
			break;

		case _ATTACKS:
			{
				if(count>1)
				{
					count = 0;
					frame++;
					if(gorea->GetMotionFrame() <= frame)
					{
						motion  = _STAND_L-lookright;
						status = ON_LAND;
						frame = 0;
						count = 0;
						firedelay = FIREDELAY3;
					}
				} else count++;
				if(InScreen)goreasound->Play(0);
				if(!lookright)
				{
					InsertObject(new ENEMYeggshoot(x+110+getrandom(-4,12),y+37+getrandom(-3,3),6,3));
					IsMove(MOVE_LEFT,1);
				} else 
				{
					InsertObject(new ENEMYeggshoot(x+16+getrandom(-12,4),y+37+getrandom(-3,3),5,3));
					IsMove(MOVE_RIGHT,1);
				}

			}
			break;
		case DAMAGEX:
			{
				count++;
				if(count>2)
				{
					frame++;
					count=0;
				}
				if(x < Seripoint->x) this->IsMove(MOVE_LEFT,4);
				else this->IsMove(MOVE_RIGHT,4);
				gorea->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
				if(gorea->GetMotionFrame() <= frame)
				//if(slim->iSendMessage(_dolNEXTFRAME,NULL,0,0) == _RESETFRAME)
				{
					if(life==0) 
					{
						frame =0;
						count = 0;
						ClearMyTiles();
						status = MYDIED;
					} else 
					{
						frame = 0;
						motion = _STAND_L - lookright;
						type = _ENEMY_GOREA;
						firedelay = FIREDELAY3;
						status = ON_LAND;
						gorea->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
					}
				}

			}
			break;
		case MYDIED:
			if(fade == 30)
			{
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y),5));
				SeriData->killed++;
			}
			fade-=3;
			if(fade < 2) DisappearObject(SELF);
			break;
	}
}


void ENEMYgorea::putSprite(void)
{
	POINT temppos;
	temppos.x = x - mapmanager->layer2.x;
	temppos.y = y - mapmanager->layer2.y-16;
	if(status == ON_LAND) temppos.y += GOREAF[frame];


	int tk = gorea->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	if(status != DAMAGEX && status != MYDIED && status != _ATTACKS)
	{
		if(count>=2)
		{
			if(tk)
			{
				frame = 0;
			//	robo1->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
			} else frame++;
			count=0;
		} 
		count++;
	} 

	if(x+isize.x - mapmanager->layer2.x>=0 && x-mapmanager->layer2.x < 640 && y + isize.y - mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
	{
		if(status != MYDIED) gorea->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,11);
		else if(fade>1) gorea->iSendMessage(_dolSHOW_ALPHA,(void *)&temppos,fade,11);
		InScreen = 1;
	} else InScreen = 0;
}

void ENEMYgorea::SendState(int j)
{
	int j2 = j & 0xFF;
	int dam = j >> 16;
	switch(j2)
	{
		case SEND_DAMAGE:
			if(status != DAMAGEX && status != MYDIED)
			{
				type = _NOTHING;
				status = DAMAGEX;
				life-=dam;
//				if(lookright) this->IsMove(MOVE_LEFT,14);
//				else  this->IsMove(MOVE_RIGHT,14);
				if(life<=0) 
				{
					life = 0;
					motion = _DAMAGE_LAND_L-lookright;
					frame = 0;
				} else  
				{
					motion = _DAMAGE_LAND_L-lookright;
					gorea->iSendMessage(_dolRESETMOTION,NULL,_DAMAGE_LAND_L-lookright,0);
				}
				count = 0;
				frame = 0;
			}
		break;
	}
}