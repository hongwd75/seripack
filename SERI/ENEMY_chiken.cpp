#include"stdafx.h"
#include"superobjectclass.h"

#define DIEDCHIKEN		52

#define CHIKENLIFE		20

#define CHIKENMAX		200

#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))


extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
extern ObjectBase *Seripoint;


mChar *chiken;

RECT chikenrect;

void GetCHIKENSprite()
{
	chiken = new mChar(Videomanager);
	chiken->mImageLoad(SeriROM.SetFilePosition("chiken.res"));
	chikenrect.top = 30;
	chikenrect.left = 31;
	chikenrect.right = 87;
	chikenrect.bottom = 94;
}

// 이미지 프리
void DropCHIKENSprite()
{
	delete chiken;
}


ENEMYchiken::ENEMYchiken(int px,int py)
{
	type = _ENEMY_CHIKEN;

	status = ON_LAND;

	x = px;
	y = py;
	count = 10;
	count2 = 0;

	life = CHIKENLIFE;
	frame = 0;
	fade = 31;
	lookright = getrandom(0,1);
	invincibility = 0;
	motion = _STAND_L-lookright;
	chiken->iSendMessage(_dolINIT,NULL,0,0);
	chiken->iSendMessage(_dolRESETMOTION,NULL,_STAND_L-lookright,0);
	this->LoadMask(NULL,SeriROM.SetFilePosition("chiken.msk"));
	isize = chiken->GetImageSize();
	cliprect = (RECT *)&chikenrect;
	maxgo = CHIKENMAX;
	InScreen = 1;
}



void ENEMYchiken::action(void)
{

	if(status != DAMAGEX && status != MYDIED)
	{
		ObjectBase *object = Seripoint;
		if ( CheckCrash( OBJ_X, OBJ_X + OBJ_XL, OBJ_Y, OBJ_Y + OBJ_YL,
                      x+cliprect->left , x+cliprect->left+cliprect->right,
					  y+cliprect->top ,y+cliprect->top+cliprect->bottom)) 
		{
			object->SendState(SEND_DAMAGE+(8 << 16));
		}
	}

	switch(status)
	{
		case ON_LAND_RUN:	
			{
				IsMove(MOVE_DOWN,4);
				switch(motion)
				{
					case _RUN_R:
						this->IsMove(MOVE_LEFT | MOVE_ONLY_BLANK,15);
						maxgo-=15;
						break;
					case _RUN_L:
						this->IsMove(MOVE_RIGHT | MOVE_ONLY_BLANK,15);
						maxgo-=15;
						break;
				}
				if(maxgo<=0)
				{
					maxgo = CHIKENMAX;
					status = ON_LAND;
					count = getrandom(10,20);
					lookright = 1 - lookright;
					motion = _STAND_L-lookright;
				}
			}
			break;

		case ON_LAND:
				if(!IsMove(MOVEOK_DOWN,1))
				{
					status = FALLING;
				} else 
				{
					if(count<=0)
					{

						int motionpt = getrandom(0,10);
						switch(motionpt)
						{
							case 0: case 1: // left move
								{
									lookright = 0;
									motion = _STAND_L-lookright;
									count = getrandom(5,10);
								}
								break;
							case 2: case 3: // right move
								{
									lookright = 1;
									motion = _STAND_L-lookright;
									count = getrandom(5,10);
								}
								break;
							default:
									motion = _STAND_L-lookright;
								break;
						}
						if(Seripoint->x+Seripoint->isize.x > x - 150 &&  Seripoint->x < x + isize.x + 100)
						{
							status = ON_LAND_RUN;
							if(Seripoint->x < x) lookright = 1;
							else lookright = 0;
							motion = _RUN_L-lookright;
							frame = 0;
						}

					} else count--;

					switch(motion)
					{
						case _STAND_R:
								this->IsMove(MOVE_LEFT,4);
							break;
						case _STAND_L:
								this->IsMove(MOVE_RIGHT,4);
							break;
					}
				}
			break;

		case FALLING:
				if(this->IsMove(MOVE_DOWN,9)) 
				{
					status = ON_LAND;
				}
			break;

		
		case MYDIED:
			if(fade < 1)
			{
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y),5));
				DisappearObject(SELF);
				SeriData->killed++;
			} else fade -=2;
			break;

		case DAMAGEX:
				if(x < Seripoint->x) this->IsMove(MOVE_LEFT,6); else this->IsMove(MOVE_RIGHT,6);
				count++;
				if(count>3)
				{
					frame++;
					count=0;
				}
				chiken->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
				if(chiken->GetMotionFrame() <= frame)
				//if(slim->iSendMessage(_dolNEXTFRAME,NULL,0,0) == _RESETFRAME)
				{
					if(life<=0) 
					{
						frame =0;
						count = 0;
						ClearMyTiles();
						status = MYDIED;
						type = _NOTHING;
						chiken->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
					} else 
					{

						count = 0;
						frame = 0;
						motion = _STAND_L-lookright;
						count = getrandom(5,10);
						type = _ENEMY_CHIKEN;
						status = ON_LAND;
						chiken->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
					}
				}
			break;
	}
}


void ENEMYchiken::putSprite(void)
{
	POINT temppos;
	temppos.x = x - mapmanager->layer2.x;
	temppos.y = y - mapmanager->layer2.y-16;


	int tk = chiken->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	if(status != DAMAGEX && status != MYDIED && status !=ON_LAND_RUN)
	{
		
		if(count2>=2)
		{
			if(tk)
			{
				frame = 0;
			} else frame++;
			count2=0;
		} 
	} 
	if(status == ON_LAND_RUN)
	{
		count2=0;
		if(tk)
		{
			frame = 0;
		} else frame++;
	}
	count2++;
	if(x+isize.x - mapmanager->layer2.x>=0 && x-mapmanager->layer2.x < 640 && y + isize.y - mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
	{
		if(status != MYDIED) chiken->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,11);
		else if(fade>1)chiken->iSendMessage(_dolSHOW_ALPHA,(void *)&temppos,fade,11);
	}
	

}

void ENEMYchiken::SendState(int j)
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
				if(lookright) this->IsMove(MOVE_LEFT,5);
				else  this->IsMove(MOVE_RIGHT,5);
				if(life<=0) 
				{
					life = 0;
					motion = _DAMAGE_LAND_L-lookright;
					frame = 0;
					ClearMyTiles();
					chiken->iSendMessage(_dolRESETMOTION,NULL,_DIE,0);
				} else  
				{
					motion = _DAMAGE_LAND_L-lookright;
					chiken->iSendMessage(_dolRESETMOTION,NULL,_DAMAGE_LAND_L-lookright,0);
				}
				count = 0;
				frame = 0;
			}
		break;
	}
}
