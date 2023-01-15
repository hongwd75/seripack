#include"stdafx.h"
#include"superobjectclass.h"

#define PIGLIFE		10

#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))


extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
extern ObjectBase *Seripoint;

int PIGFRAME[5] = {0,-24,-36,0,0};
mChar *pig;
RECT pigrect;

void GetPIGSprite()
{
	pig = new mChar(Videomanager);
	pig->mImageLoad(SeriROM.SetFilePosition("pig.res"));
	pigrect.top = 59;
	pigrect.left = 21;
	pigrect.right = 77;
	pigrect.bottom = 51;
}

// 이미지 프리
void DropPIGSprite()
{
	delete pig;
}


ENEMYpig::ENEMYpig(int px,int py)
{
	type = _ENEMY_PIG;

	status = ON_LAND;

	x = px;
	y = py;
	count = 10;
	count2 = 0;

	life = PIGLIFE;
	frame = 0;
	lookright = 0;
	fade = 31;
	invincibility = 0;
	motion = _WALK_L-lookright;
	pig->iSendMessage(_dolINIT,NULL,0,0);
	pig->iSendMessage(_dolRESETMOTION,NULL,_WALK_L-lookright,0);
	this->LoadMask(NULL,SeriROM.SetFilePosition("pig.msk"));
	isize = pig->GetImageSize();
	cliprect = (RECT *)&pigrect;
	if(Seripoint->x < x) lookright = 1;
	motion = _WALK_L-lookright;
	pig->iSendMessage(_dolRESETMOTION,NULL,_WALK_L-lookright,0);

}


void ENEMYpig::action(void)
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
		case ON_LAND:
				if(!IsMove(MOVEOK_DOWN,1))
				{
					status = FALLING;
				} else 
				{
					if(!lookright)
					{
						if(IsMove(MOVE_RIGHT | MOVE_ONLY_BLANK,9))
						{
							lookright = 1 - lookright;
							motion  = _WALK_L-lookright;
						}
					} else 
					{
						if(IsMove(MOVE_LEFT | MOVE_ONLY_BLANK,9))
						{
							lookright = 1 - lookright;
							motion  = _WALK_L-lookright;
						}
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
			if(fade == 31)
			{
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y),5));

			} 
			fade-=4;
			if(fade < 2) 
			{
				DisappearObject(SELF);
				SeriData->killed++;
			}
			break;

		case DAMAGEX:
				this->IsMove(MOVE_DOWN,4); 
				count++;
				if(count>3)
				{
					frame++;
					count=0;
				}
				if(x < Seripoint->x) this->IsMove(MOVE_LEFT | MOVE_ONLY_BLANK,3);
				else this->IsMove(MOVE_RIGHT | MOVE_ONLY_BLANK,3);

				pig->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
				if(pig->GetMotionFrame() <= frame)
				//if(slim->iSendMessage(_dolNEXTFRAME,NULL,0,0) == _RESETFRAME)
				{
					if(life<=0) 
					{
						frame =0;
						count = 0;
						ClearMyTiles();
						status = MYDIED;
					} else 
					{
						frame = 0;
						motion = _WALK_L-lookright;
						type = _ENEMY_PIG;
						status = ON_LAND;
						pig->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
					}
				}
			break;
	}
}


void ENEMYpig::putSprite(void)
{
	POINT temppos;
	temppos.x = x - mapmanager->layer2.x;
	temppos.y = y - mapmanager->layer2.y-16;
	if(status == ON_LAND) temppos.y+=PIGFRAME[frame];
	
	int tk = pig->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	if(status != DAMAGEX && status != MYDIED)
	{
		
		if(count2>=1)
		{
			if(tk)
			{
				frame = 0;
			} else frame++;
			count2=0;
		} 
	} 
	
	count2++;
	if(x+isize.x - mapmanager->layer2.x>=0 && x-mapmanager->layer2.x < 640 && y + isize.y - mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
	{
		if(status != MYDIED) pig->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,11);
		else if(fade>1) pig->iSendMessage(_dolSHOW_ALPHA,(void *)&temppos,fade,11);
	}
	

}

void ENEMYpig::SendState(int j)
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
//				if(lookright) this->IsMove(MOVE_LEFT | MOVE_ONLY_BLANK,14);
//				else  this->IsMove(MOVE_RIGHT | MOVE_ONLY_BLANK,14);
				motion = _DAMAGE_LAND_L-lookright;
				pig->iSendMessage(_dolRESETMOTION,NULL,_DAMAGE_LAND_L-lookright,0);
				count = 0;
				frame = 0;
			}
		break;
	}
}
