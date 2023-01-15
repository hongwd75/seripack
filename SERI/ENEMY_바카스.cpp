#include"stdafx.h"
#include"superobjectclass.h"


#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))

#define WAITING		0xFF
#define BAKAMAX		500

extern int GetAdder(int x);
extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
extern ObjectBase *Seripoint;

mChar *baka;

RECT    bakarect;

////////////////////////////////////////////////////////////////////////
// 이미지 로더 /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void GetBAKASprite()
{
	baka = new mChar(Videomanager);
	baka->mImageLoad(SeriROM.SetFilePosition("bacchus.res"));
	bakarect.top = 16;
	bakarect.left = 28;
	bakarect.right = 56;
	bakarect.bottom = 86;
	return;
}


////////////////////////////////////////////////////////////////////////
// 이미지 프리 /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void DropBAKASprite()
{
	delete baka;
}


////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////
ENEMYbaka::ENEMYbaka(int px,int py)
{
	type = _ENEMY_BAKA;

	status = JUMPREADY;

	x = px;
	y = py;
	count = 4;
	count2 = 0;
	addx = 0;
	life = 10;
	frame = 0;
	lookright = 0;
	motion = _JUMP_L - lookright;
	fade = 31;
	invincibility = 0;
	count3 = 4;	
	addy = 0;
	baka->iSendMessage(_dolINIT,NULL,0,0);
	baka->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	isize = baka->GetImageSize();
	LoadMask(NULL,SeriROM.SetFilePosition("bacchus.msk"));
	cliprect = (RECT *)&bakarect;
	maxgo = BAKAMAX;
	InScreen = 1;
}



////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////
void ENEMYbaka::action()
{

		if(maxgo<=0)
		{
			maxgo = BAKAMAX;
			if(lookright) motion += lookright;
			lookright = 1 - lookright;
			motion -= lookright;
		}
		switch(status)
		{
			case JUMPREADY:
				{
					if(frame == 0) 
					{
						//count2--;
					    if(Seripoint->x+Seripoint->isize.x > x - 150 &&  Seripoint->x < x + isize.x + 100)
						{
							count3 = 1;
							addx = 7;
						} else
						{
							count3 = 5;
							addx = 2;
						}

						if(addx == 2) lookright = getrandom(0,1);
						else 
						{
							if(Seripoint->x < x) lookright = 1; else lookright = 0;
							maxgo = BAKAMAX;
						}
						motion = _JUMP_L - lookright;
					}
					status = JUMPING;
					addy = getrandom(36,50);
				}
			break;

			case JUMPING:
				{
					if(count<=0) 
					{
						if(frame < baka->GetMotionFrame()-1)frame++;
						count = count3;
					} else count--;

						if(lookright)
						{
							if(IsMove(MOVE_LEFT | MOVE_ONLY_BLANK,addx))
							{
								lookright = 1 - lookright;
								motion  = _JUMP_L-lookright;
								maxgo = BAKAMAX;
							} else maxgo-=addx;
						} else 
						{
							if(IsMove(MOVE_RIGHT | MOVE_ONLY_BLANK,addx))
							{
								lookright = 1 - lookright;
								motion  = _JUMP_L-lookright;
								maxgo = BAKAMAX;
							} else maxgo-=addx;
						} 
						addy-=4;
						if(this->IsMove(MOVE_UP,GetAdder(addy)) || addy <=0)
						{
							status = FALLING;
							frame = 0;
							motion = _FALLING_L - lookright;
							addy = 0;
						}
				}
			break;

			case FALLING:
				{
					if(lookright)
					{
						if(IsMove(MOVE_LEFT | MOVE_ONLY_BLANK,addx))
						{
							lookright = 1 - lookright;
							motion  = _FALLING_L-lookright;
							maxgo = BAKAMAX;
						} else maxgo -=addx;
					} else 
					{
						if(IsMove(MOVE_RIGHT | MOVE_ONLY_BLANK,addx))
						{
							lookright = 1 - lookright;
							motion  = _FALLING_L-lookright;
							maxgo = BAKAMAX;
						} else maxgo -=addx;
					}
					addy+=4;
					if(this->IsMove(MOVE_DOWN,GetAdder(addy)))
					{
						status = FALLED;
						count = count3;
					}
				}
			break;

			case FALLED:
				if(count <= 0) 
				{
					frame++;
					count = count3;
				} else count--;
				if(frame >= baka->GetMotionFrame())
				{
					frame = 0;
					status = JUMPREADY;
					addy = getrandom(36,50);
					count = count3;
					motion = _JUMP_L - lookright;
				}
			break;

			case DAMAGEX:
				{
					if(count<=0)
					{
						frame++;
						count = 2;
					} else count--;
					if(x < Seripoint->x) this->IsMove(MOVE_LEFT,2);
					else this->IsMove(MOVE_RIGHT,2);
					if(frame >= baka->GetMotionFrame())
					{
						frame = count2;
						motion  = _FALLING_L-lookright;
						status = FALLING;
					}
				}
			break;
			case MYDIED:
				{
					if(count<=0)
					{
						if(frame < baka->GetMotionFrame()-1) frame++;
						count = 2;
					} else count--;

					if(frame == baka->GetMotionFrame()-1)
					{
						addy+=4;					
						if(this->IsMove(MOVE_DOWN,GetAdder(addy)))
						{
							fade-=4;
							if(fade < 1)
							{
								InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
								InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));		
								InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));		
								InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y),5));
								ClearMyTiles();
								DisappearObject(SELF);
								SeriData->killed++;
							}
						}
					}
				}
			break;
		}
}



void ENEMYbaka::putSprite()
{
	POINT temppos;
	temppos.x = x - mapmanager->layer2.x;
	temppos.y = y - mapmanager->layer2.y-16;


	baka->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
		if(temppos.x + isize.x > 0 && temppos.x <640 && temppos.y < 480 && temppos.y + isize.y > 0 ) 
			if(status != MYDIED)baka->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,10);
			else baka->iSendMessage(_dolSHOW_ALPHA,(void *)&temppos,fade,11);
}



void ENEMYbaka::SendState(int j)
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
				motion = _DAMAGE_LAND_L-lookright;
				if(life <=0) 
				{
					type = _NOTHING;
					addy = 0;
					motion = _DAMAGE_SKY_L-lookright;
					status = MYDIED;
				}
				baka->iSendMessage(_dolRESETMOTION,NULL,motion,0);
				count = 4;
				count2 = frame;
				frame = 0;
			}
		break;
	}
}