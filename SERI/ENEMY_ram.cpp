#include"stdafx.h"
#include"superobjectclass.h"


#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))

#define WAITING		0xFF
#define RAMMAX		500

extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;

mChar *ram;

RECT    ramrect;

////////////////////////////////////////////////////////////////////////
// 이미지 로더 /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void GetRAMSprite()
{
	ram = new mChar(Videomanager);
	ram->mImageLoad(SeriROM.SetFilePosition("ram.res"));
	ramrect.top = 15;
	ramrect.left = 15;
	ramrect.right = 84;
	ramrect.bottom = 108;
	return;
}


////////////////////////////////////////////////////////////////////////
// 이미지 프리 /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void DropRAMSprite()
{
	delete ram;
}


////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////
ENEMYram::ENEMYram(int px,int py)
{
	type = _ENEMY_RAM;

	status = ON_LAND;

	x = px;
	y = py;
	count = 0;
	addx = 0;
	life = 70;
	frame = 0;
	lookright = 0;
	//motion = _STAND_L-lookright;
	motion = _JUMP_L - lookright;
	fade = 31;
	invincibility = 0;
	cliprect = (RECT *)&ramrect;
	ram->iSendMessage(_dolINIT,NULL,0,0);
	ram->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	isize = ram->GetImageSize();
	LoadMask(NULL,SeriROM.SetFilePosition("ram.msk"));
	maxgo = RAMMAX;
}


////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
int GetAdder(int x)
{
	if(x>15) return 15;
	else return x;
}


////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////
void ENEMYram::action()
{

	if(life>0)
	{
		if(maxgo<=0)
		{
			maxgo = RAMMAX;
			if(lookright) motion += lookright;
			lookright = 1 - lookright;
			motion -= lookright;
		}

		switch(status)
		{
			case JUMPING:
				{
					frame++;
					int falling = 0;
					if(!IsMove(MOVE_UP,GetAdder(addx)))
					{
						if(lookright)
						{
							if(IsMove(MOVE_RIGHT,6)) 
							{
								lookright = 1;
								motion = _JUMP_L - lookright;
								maxgo = RAMMAX;
							} else maxgo-=6;
						} else 
						{
							if(IsMove(MOVE_LEFT,6)) 
							{
								lookright = 0;
								motion = _JUMP_L - lookright;
								maxgo = RAMMAX;
							} else maxgo-=6;
						}
						addx-=3;
						if(addx <=0) falling = 1;
					} else falling = 1; 

					if(falling)
					{
						addx = 0;
						status = FALLING;
					}
				}
				break;

			case FALLING:
					frame++;
					if(IsMove(MOVE_DOWN,GetAdder(addx)))
					{
						status = ON_LAND;
						motion = _STAND_L-lookright;
						frame = 0;
						count = 0;
					}
					if(addx<15) addx+=3;

				break;

			case ON_LAND:
				if(!IsMove(MOVEOK_DOWN,1))
				{
					status = FALLING;
					motion = _JUMP_L - lookright;
					frame = 0;
					addx = 4;
				} else 
				{
					count++;
					if(count>10)
					{
						status = JUMPING;
						motion = _JUMP_L - lookright;
						frame = 0;
						addx = 20;
					}
				}
				break;
		case MYDIED:
				DisappearObject(SELF);
			break;

		case DAMAGEX:
				this->IsMove(MOVE_DOWN,6); 
				count++;
				if(count>2)
				{
					frame++;
					count=0;
				}
				ram->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
				if(ram->GetMotionFrame() <= frame)
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

						count = 0;
						frame = 0;
						motion = _WALK_L-lookright;
						count = getrandom(5,10);
						status = ON_LAND;
						ram->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
					}
				}
			break;
		}
	}
	else
	{
		fade-=4;
		if(fade<=0) 
		{
			ClearMyTiles();
			DisappearObject(SELF);
		}
	}
}



void ENEMYram::putSprite()
{
	POINT temppos;
	temppos.x = x - mapmanager->layer2.x;
	temppos.y = y - mapmanager->layer2.y;


	if(ram->iSendMessage(_dolRESETMOTION,NULL,_JUMP_L,frame)) 
	{
		if(status != DAMAGEX && status != MYDIED) frame = 1;
	} 
	if(life>0)
	{
		if(temppos.x + isize.x > 0 && temppos.x <640 && temppos.y < 480 && temppos.y + isize.y > 0 ) 
			ram->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,10);
	}	else 
		if(fade>0)
		{
		if(temppos.x + isize.x > 0 && temppos.x <640 && temppos.y < 480 && temppos.y + isize.y > 0 ) 
			ram->iSendMessage(_dolSHOW_ALPHA,(void *)&temppos,fade,10);
		}
}



void ENEMYram::SendState(int j)
{
	int j2 = j & 0xFF;
	int dam = j >> 16;
	switch(j2)
	{
		case SEND_DAMAGE:
			if(status != DAMAGEX && status != MYDIED)
			{

				status = DAMAGEX;
				life-=dam;
				if(life<=0) 
				{
					life = 0;
					motion = _DIE;
					ram->iSendMessage(_dolRESETMOTION,NULL,_DIE,0);
				} else  
				{
					motion = _DAMAGE_LAND_L-lookright;
					ram->iSendMessage(_dolRESETMOTION,NULL,_DAMAGE_LAND_L-lookright,0);
				}
				count = 0;
				frame = 0;
			}
		break;
	}
}