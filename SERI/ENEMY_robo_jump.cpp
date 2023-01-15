#include"stdafx.h"
#include"superobjectclass.h"

#define MAXROBO2		400
#define ROBO2LIFE		24
#define FIREDELAY2		30
#define ROBO2JUMPDELAY  10

#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))


extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
extern ObjectBase *Seripoint;

CSound   *ro2sound;
CSound   *ro2gunsound;
CSound   *ro2sound2;
mChar *robo2;

RECT robo2rect[2];

void GetROBO2Sprite()
{
	robo2 = new mChar(Videomanager);
	robo2->mImageLoad(SeriROM.SetFilePosition("robo_j.res"));
	robo2rect[0].top = 16;
	robo2rect[0].left = 49;
	robo2rect[0].right = 69;
	robo2rect[0].bottom = 58;

	robo2rect[1].top = 16;
	robo2rect[1].left = 9;
	robo2rect[1].right = 74;
	robo2rect[1].bottom = 60;

	ro2sound = new CSound();
	ro2sound->Initialize(SeriROM.SetFilePositionMMF("akebono.wav"),1);
	ro2gunsound = new CSound();
	ro2gunsound->Initialize(SeriROM.SetFilePositionMMF("gun.wav"),1);
	ro2sound2 = new CSound();
	ro2sound2->Initialize(SeriROM.SetFilePositionMMF("drip.wav"),1);

}

// 이미지 프리
void DropROBO2Sprite()
{
	delete robo2;
	delete ro2sound;
	delete ro2sound2;
	delete ro2gunsound;
}


ENEMYrobo2::ENEMYrobo2(int px,int py)
{
	type = _ENEMY_ROBO2;

	status = ON_LAND;

	x = px;
	y = py;
	count = 0;
	count2 = ROBO2JUMPDELAY;

	life = ROBO2LIFE;
	frame = 0;
	invincibility = 0;
	lookright = 0;
	motion = _JUMP_L-lookright;
	robo2->iSendMessage(_dolINIT,NULL,0,0);
	robo2->iSendMessage(_dolRESETMOTION,NULL,_JUMP_L-lookright,0);
	this->LoadMask(NULL,SeriROM.SetFilePosition("robo_j.msk"));
	isize = robo2->GetImageSize();
	
	firedelay = FIREDELAY2;
	if(Seripoint->x < x) lookright = 1;
	motion = _JUMP_L-lookright;
	robo2->iSendMessage(_dolRESETMOTION,NULL,_JUMP_L-lookright,0);

	cliprect = (RECT *)&robo2rect[lookright];
	addy = 0;
	maxgo = MAXROBO2;
	InScreen = 1;
}



void ENEMYrobo2::action(void)
{
	if(maxgo<=0)
	{
		maxgo = MAXROBO2;
		if(lookright) motion += lookright;
		lookright = 1-lookright;
		motion -= lookright;
		frame = 0;
	}
	
	switch(status)
	{
		case ON_LAND:
				if(!IsMove(MOVEOK_DOWN,1))
				{
					status = FALLING;
					addy = 2;
				} else 
				{
					if(count2<=0)
					{
						status = JUMPREADY;
						count2 = getrandom(8,10);
						count = 2;
						addy = 14;
					} else count2--;
				}
			break;

		case JUMPREADY:
			if(count <=0)
			{
				frame++;
				if(robo2->GetMotionFrame() <= frame)
				{
					frame-=2;
					status = JUMPING;
					addy = 15;
					if(InScreen)ro2sound2->Play(0);
				}
				count = 2;
			} else count--;

			break;

		case JUMPING:
			{
				if(count<=0)
				{
				    addy-=3;
					count = getrandom(2,3);
				} else count--;
				if(addy <= 0 || this->IsMove(MOVE_UP,addy)) 
				{
					addy = 0;
					status = _ATTACKS;
					count = 0;
					frame = 0;
					motion = _ATTACK1_L-lookright;
					if(InScreen)ro2gunsound->Play(0);

				}
				if(lookright)
				{
					if(IsMove(MOVE_LEFT | MOVE_ONLY_BLANK,5))
					{
						lookright = 1 - lookright;
						motion  = _JUMP_L-lookright;
						maxgo = MAXROBO2;
					} else maxgo-=5;
				} else 
				{
					if(IsMove(MOVE_RIGHT | MOVE_ONLY_BLANK,5))
					{
						lookright = 1 - lookright;
						motion  = _JUMP_L-lookright;
						maxgo = MAXROBO2;
					} else maxgo-=5;
				}

			}
			break;

		case FALLING:
				if(this->IsMove(MOVE_DOWN,addy)) 
				{
					status = FALLED;
					count = 0;
					count2= ROBO2JUMPDELAY;
					frame=0;
					motion = _FALLING_L - lookright;
				} else
				{
					addy+=4;
					if(addy>14) addy = 14;
				}
				if(lookright)
				{
					if(IsMove(MOVE_LEFT | MOVE_ONLY_BLANK,3))
					{
						lookright = 1 - lookright;
						motion  = _FALLING_L-lookright;
						maxgo = MAXROBO2;
					} else maxgo-=3;
				} else 
				{
					if(IsMove(MOVE_RIGHT | MOVE_ONLY_BLANK,3))
					{
						lookright = 1 - lookright;
						motion  = _FALLING_L-lookright;
						maxgo = MAXROBO2;
					} else maxgo-=3;
				}

			break;
		case FALLED:
			if(count <=0)
			{
				frame++;
				if(robo2->GetMotionFrame() <= frame)
				{
					status = JUMPREADY;
					count = 2;
					addy = 15;
					motion = _JUMP_L-lookright;
					frame=0;
				}
				count = 2;
			} else count--;
			break;

		case _ATTACKS:
			{
				if(count>1)
				{
					count = 0;
					frame++;
					
					if(robo2->GetMotionFrame() <= frame)
					{
						motion  = _FALLING_L-lookright;
						status = FALLING;
						frame = 0;
						count = 0;
					}
				} else count++;
				this->IsMove(MOVE_UP,2);
				if(!lookright)
				{
					if(InScreen)InsertObject(new ENEMYeggshoot(x+110+getrandom(-4,12),y+47+getrandom(-3,3),12,0,3,5));
					IsMove(MOVE_LEFT,1);
				} else 
				{
					if(InScreen)InsertObject(new ENEMYeggshoot(x+16+getrandom(-12,4),y+47+getrandom(-3,3),-12,0,3,5));
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
				if(x < Seripoint->x) this->IsMove(MOVE_LEFT,5);
				else this->IsMove(MOVE_RIGHT,5);

				if(robo2->GetMotionFrame() <= frame)
				//if(slim->iSendMessage(_dolNEXTFRAME,NULL,0,0) == _RESETFRAME)
				{
					if(life==0) 
					{
						frame =0;
						count = 0;
						ClearMyTiles();
						status = MYDIED;
						if(InScreen)ro2sound->Play(0);
						InsertObject(new PAPUN(getrandom(x-24,x+isize.x),getrandom(y-24,y),getrandom(0,1)));
						InsertObject(new PAPUN(getrandom(x-24,x+isize.x),getrandom(y-24,y),getrandom(0,1)));
					} else 
					{

						frame = 0;
						motion =_FALLING_L - lookright;
						type = _ENEMY_ROBO2;
						status = FALLING;
						robo2->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
						InsertObject(new PAPUN(getrandom(x-24,x+isize.x),getrandom(y-24,y),getrandom(0,1)));
						InsertObject(new PAPUN(getrandom(x-24,x+isize.x),getrandom(y-24,y),getrandom(0,1)));
					}
				}

			}
			
			break;
		case MYDIED:
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y),5));

				InsertObject(new PAPUN(getrandom(x-24,x+isize.x),getrandom(y-24,y),getrandom(0,1)));
				InsertObject(new PAPUN(getrandom(x-24,x+isize.x),getrandom(y-24,y),getrandom(0,1)));
				InsertObject(new PAPUN(getrandom(x-24,x+isize.x),getrandom(y-24,y),getrandom(0,1)));
				InsertObject(new PAPUN(getrandom(x-24,x+isize.x),getrandom(y-24,y),getrandom(0,1)));
				InsertObject(new PAPUN(getrandom(x-24,x+isize.x),getrandom(y-24,y),getrandom(0,1)));
				SeriData->killed++;
				DisappearObject(SELF);
			break;
	}
}


void ENEMYrobo2::putSprite(void)
{
	POINT temppos;
	temppos.x = x - mapmanager->layer2.x;
	temppos.y = y - mapmanager->layer2.y-16;

	robo2->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	if(x+isize.x - mapmanager->layer2.x>=0 && x-mapmanager->layer2.x < 640 && y + isize.y - mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
	{
		if(status != MYDIED) robo2->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,11);
		InScreen = 1;
	}  else InScreen = 0;
	

}

void ENEMYrobo2::SendState(int j)
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
					robo2->iSendMessage(_dolRESETMOTION,NULL,_DAMAGE_LAND_L-lookright,0);
				}
				count = 0;
				frame = 0;
			}
		break;
	}
}