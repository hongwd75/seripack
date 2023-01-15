#include"stdafx.h"
#include"superobjectclass.h"


#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))

#define GHOST_ATTACK				0xF00
#define GHOST_ATTACK2				0xF01
#define GHOST_AWAY					0xF02

extern ObjectBase *Seripoint;
extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;

mChar *ghost;
CSound   *ghostsound;
RECT    ghostrect[2];

////////////////////////////////////////////////////////////////////////
// 이미지 로더 /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void GetGHOSTSprite()
{
	ghost = new mChar(Videomanager);
	ghost->mImageLoad(SeriROM.SetFilePosition("ghost.res"));
	ghostrect[0].left = 53;
    ghostrect[0].top = 7;
	ghostrect[0].right = 74;
	ghostrect[0].bottom = 115;

	ghostrect[1].left = 15;
    ghostrect[1].top = 6;
	ghostrect[1].right = 78;
	ghostrect[1].bottom = 111;
	ghostsound = new CSound();
	ghostsound->Initialize(SeriROM.SetFilePositionMMF("ghost.wav"),1);			// 공격 2

	return;
}


////////////////////////////////////////////////////////////////////////
// 이미지 프리 /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void DropGHOSTSprite()
{
	delete ghost;
	delete ghostsound;
}


////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////
ENEMYghost::ENEMYghost(int px,int py)
{
	type = _ENEMY_GHOST;

	status = ON_LAND;

	x = px;
	y = py;
	count = 0;
	life = 9;
	frame = 0;
	count2=0;
	addx=0;
	addy=0;
	lookright = 0;
	motion = _STAND_L - lookright;
	fade = 16;
	invincibility = 0;
	
	ghost->iSendMessage(_dolINIT,NULL,0,0);
	ghost->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	isize = ghost->GetImageSize();
	cliprect = (RECT *)&ghostrect[lookright];
	checkbl = NULL;
	InScreen = 1;
}


////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////
void ENEMYghost::action()
{
	switch(status)
	{
		case ON_LAND:
			if(count++>3)
			{
				frame++;
				count = 0;
			}
				if(Seripoint->x > x - 300 && Seripoint->x < x + 400 + isize.x && 
				Seripoint->y > y - 300 && Seripoint->y < y + 300 + isize.y)
			{
				frame = 0;
				count = 0;
				motion = _WALK_L-lookright;
				status = ON_LAND_WALK;
				if(InScreen) ghostsound->Play(0);
			}
		break;
			case ON_LAND_WALK:
			{
			  if(count2<=0)
			  {
				if(Seripoint->x > x ) 
				{
					lookright = 0;
					addx = 3;
				}
				 else 
				 {
					 addx = -3;
					lookright = 1;
				 }
				if(Seripoint->y < y ) addy = -3;
				 else addy = 3;
				motion = _WALK_L-lookright;
				count2 = 20;
			  }
			  x +=addx;
			  y+=addy;
			  count++;
			  count2--;
			
			if(count > 2)
			{
				frame++;
				count = 0;
			}
					if(Seripoint->x > x - 30 && Seripoint->x < x + 30 + isize.x && 
					Seripoint->y > y - 30 && Seripoint->y < y + 50 )
				{
					frame = 0;
					count = 0;
					motion = _ATTACK1_L-lookright;
					status = GHOST_ATTACK;
				}
				
			}
		break;
			case GHOST_ATTACK:
			{
				if(count++>3)
				{
					frame++;
					count = 0;
				}
				if(ghost->GetMotionFrame() <= frame)
				{
					frame = ghost->GetMotionFrame() -1;
					status = GHOST_ATTACK2;
					type = _NOTHING;
				}
			}
		break;
			case GHOST_ATTACK2:
			{
				if(count++>3)
				{
					frame++;
					count=0;
				}
				if(lookright)
				{
					x-=21;
					if(x+isize.x < mapmanager->layer2.x) status = MYDIED;
				} else 
				{
					x+=21;
					if(x > mapmanager->layer2.x+640) status = MYDIED;
				}

				ObjectBase *object = Seripoint;
	            if ( CheckCrash( OBJ_X, OBJ_X + OBJ_XL, OBJ_Y, OBJ_Y + OBJ_YL,
		                         x+cliprect->left , x+cliprect->left+cliprect->right,
								y+cliprect->top ,y+cliprect->top+cliprect->bottom)) 
				{
					object->SendState(SEND_DAMAGE+(8 << 16));
				}
			}
			break;

			case MYDIED:
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));		
				InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));		
				DisappearObject(SELF);
			break;

			case DAMAGEX:
			count++;
			if(count>2)
			{
				frame++;
				count=0;
			}
			ghost->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
			if(ghost->GetMotionFrame() <= frame)
			{
				if(life==0) 
				{
					frame =0;
					count = 0;
					status = MYDIED;
				} else 
				{
					count = 0;
					frame = 0;
					motion = _WALK_L-lookright;
					type = _ENEMY_GHOST;
					status = ON_LAND_WALK;
					ghost->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
				}
			}
		break;
	}

}



void ENEMYghost::putSprite()
{
	POINT temppos;
	temppos.x = x - mapmanager->layer2.x;
	temppos.y = y - mapmanager->layer2.y-16;

	int tk = ghost->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	if(status != DAMAGEX && status != MYDIED)
	{
		if(tk)	frame = 0;
	} 
	
	if(x+isize.x - mapmanager->layer2.x>=0 && x-mapmanager->layer2.x < 640 && y + isize.y - mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
	{
		InScreen = 1;
		if(status != MYDIED) 
		{
			ghost->iSendMessage(_dolSHOW,(void *)&temppos,_HALFTONEMODE,11);
		}
	} else InScreen = 0;
}



void ENEMYghost::SendState(int j)
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
				if(lookright) x-=5; else x+=5;
				if(life<=0) 
				{
					life = 0;
					motion = _DIE;
					SeriData->killed++;
					ghost->iSendMessage(_dolRESETMOTION,NULL,_DIE,0);
				} else  
				{
					motion = _DAMAGE_LAND_L-lookright;
					ghost->iSendMessage(_dolRESETMOTION,NULL,_DAMAGE_LAND_L-lookright,0);
				}
				count = 0;
				frame = 0;
			}
		break;
	}
}