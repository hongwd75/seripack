#include"stdafx.h"
#include"superobjectclass.h"


#define FOXLIFE			20
#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))


extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
extern ObjectBase *Seripoint;

CSound   *foxsound[2];
mChar	 *iFox;
BYTE	 *foxmask[2];
RECT      Foxrect;

void GetFoxSprite()
{

	iFox   = new mChar(Videomanager);
	foxsound[0]  = new CSound();
	foxsound[0]->Initialize(SeriROM.SetFilePositionMMF("burrowup.wav"),1);
	foxsound[1]  = new CSound();
	foxsound[1]->Initialize(SeriROM.SetFilePositionMMF("burrowdn.wav"),1);

	iFox->mImageLoad(SeriROM.SetFilePosition("fox.res"));
	Foxrect.top = 41;
	Foxrect.left = 44;
	Foxrect.bottom = 77;
	Foxrect.right = 46;
	foxmask[1] = GetMask(NULL,SeriROM.SetFilePosition("fox.msk"));
	foxmask[0] = GetMask(NULL,SeriROM.SetFilePosition("fox2.msk"));

}

// 이미지 프리
void DropFoxSprite()
{
	delete iFox;
	delete foxmask[0];
	delete foxmask[1];
	delete foxsound[0];
	delete foxsound[1];

}
/*
#define _IN_LAND			1		// 땅속에 있다
#define _IN_LAND2			2		// 땅속에 있다
#define _UPUP_LAND			3		// 땅속에서 나오고 있다.
#define _OUT_LAND			4		// 나왔다
#define _ATTACK_LAND		5		// 때린다 띠려
#define _DOWN_LAND			6		// 땅파고 들어간다.
#define _DOWN_LAND2			7		// 땅파고 들어간다.
*/
ENEMYfox::ENEMYfox(int px,int py)
{
	type = _NOTHING;

	status = _IN_LAND;

	life = FOXLIFE;
	x = px;
	y = py;
	frame = 0;
	count2 = 2;
	count = getrandom(40,50);
	invincibility = 0;
	motion = _STAND_R;
	
	this->LoadMask(NULL,SeriROM.SetFilePosition("fox.msk"));
	delete checkbl;
	checkbl = foxmask[0];
	iFox->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	isize = iFox->GetImageSize(); 
	this->cliprect = (RECT *)&Foxrect;
	InScreen = 1;
}

void ENEMYfox::action(void)
{
	this->IsMove(MOVE_DOWN,0); 
	switch(status)
	{
		case _IN_LAND:
			{
			  if(life>0)
			  {
				if(count > 0) count--;
				if(Seripoint->x+Seripoint->isize.x > x - 150 &&  Seripoint->x < x + isize.x + 100 && count <=0)
				{
					count = 2;
					frame = 0;
					status = _IN_LAND2;
					count2 = getrandom(2,3);
					
				}
			  }
			}
		break;

		case _IN_LAND2:
			{
			   if(count < 0)
			   {
					frame++;
					if(iFox->GetMotionFrame()<=frame)
					{
						frame = 0;
						if(count2--<=0) 
						{
							count = 1;
							frame = 0;
							count2 = 4;
							motion = _JUMP_R;
							status = _UPUP_LAND;
							if(InScreen)foxsound[0]->Play(0);
						}
					}
					count = 2;
			   } else count--;
			}
			break;

		case _UPUP_LAND:
			{
			  count--;
			  if(count<=0)
			  {
				  count=1;
				  frame++;
				  if(iFox->GetMotionFrame()<=frame)
				  {
					  type   = _ENEMY_FOX;
					  status = _OUT_LAND;
					  motion = _WALK_R;
					  count=getrandom(10,30);
					  frame = 0;
					 // ClearMyTiles(0,0);
					  checkbl = foxmask[1];
					  //foxmask
				  }
			  }
			}
			break;
		case _OUT_LAND:
			{
				if(count>0) count--;
				else
				{

					//life = FOXLIFE;
					status = _DOWN_LAND2;
					motion = _FALLING_R;
					type = _NOTHING;
					count = 2;
					frame = 0;
					ClearMyTiles(0,0);
				    checkbl = foxmask[0];
					count2 = 7;
					if(InScreen)foxsound[1]->Play(0);
				}
			}
			break;
		case _ATTACK_LAND:
			{
				// sound insert
				if(count <=0)
				{
					frame++;
					if(iFox->GetMotionFrame() <= frame)
					{
						count = 2;
						InsertObject(new ENEMYeggshoot(x+getrandom(10,100),y+110+getrandom(-5,5),2,3));	
						frame = 0;
						count2--;
						if(count2 <=0)
						{
							  status = _IN_LAND;
							  motion = _STAND_R;
							  frame = 0;
							  count = getrandom(40,50);
							  type = _NOTHING;
						}
					} 
				} else count--;
			}
			break;

		case _DOWN_LAND2: // 공격하러 들어갈때
			{
			  count--;
			  if(count<=0)
			  {
				  count=2;
				  frame++;
				  if(iFox->GetMotionFrame()<=frame)
				  {
					status = _ATTACK_LAND;
					motion = _ATTACK1_R;
					frame = 0;
					count2 = 5;
					count = 2;
				  }
			  }
			}
			break;


		case _DOWN_LAND:
			{
			  count--;
			  if(count<=0)
			  {
				  count=2;
				  frame++;
				  if(iFox->GetMotionFrame()<=frame)
				  {
					  status = _IN_LAND;
					  motion = _STAND_R;
					  count=count2;
					  frame = 0;
					  if(life <=0)
					  {
						InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));
						InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));		
						InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));		
						InsertObject(new STAR10(getrandom(x-24,x+isize.x),getrandom(y-24,y)));		

					  }
				  }
			  }
			}
			break;

		case DAMAGEX:
			{
				if(count <=0)
				{
					frame++;
					if(iFox->GetMotionFrame()<=frame)
					{
						status = _DOWN_LAND;
						motion = _FALLING_R;
						type = _NOTHING;
						count = 5;
						count2 = 150;
						frame = 0;
					}
					count = 2;
				} else count--;
			}
			break;
	}
}


void ENEMYfox::putSprite(void)
{
    iFox->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	POINT temppos;
	temppos.x = x - mapmanager->layer2.x;
	temppos.y = y - mapmanager->layer2.y-16;

	if(x+isize.x - mapmanager->layer2.x>=0 && x-mapmanager->layer2.x < 640 && y + isize.y - mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
	{
		iFox->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,9);
		InScreen = 1;
	} else InScreen = 0;
	

}


void ENEMYfox::SendState(int j)
{
	int j2 = j & 0xFF;
	int dam = j >> 16;
	switch(j2)
	{
		case SEND_DAMAGE:
			if(status == _OUT_LAND)
			{
				life-=dam;
				status = DAMAGEX;
				motion = _DAMAGE_LAND_R;
				type = _NOTHING;
				frame = 0;
				count = 2;
				ClearMyTiles(0,0);
			    this->checkbl = foxmask[0];
				foxsound[1]->Play(0);

/*
				if(life<0)
				{
					status = _DOWN_LAND;
					motion = _FALLING_R;
					type = _NOTHING;
					count = 5;
					count2 = 150;
					frame = 0;
				}
*/
			}
		break;
	}
}