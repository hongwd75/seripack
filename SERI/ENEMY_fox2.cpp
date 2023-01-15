#include"stdafx.h"
#include"superobjectclass.h"


#define FOX2LIFE			20
#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))


extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
extern ObjectBase *Seripoint;

CSound   *foxsound2[2];
mChar	 *iFox2;
BYTE	 *foxmask2[2];
RECT      Foxrect2;

void GetFox2Sprite()
{

	iFox2   = new mChar(Videomanager);
	foxsound2[0]  = new CSound();
	foxsound2[0]->Initialize(SeriROM.SetFilePositionMMF("burrowup.wav"),1);
	foxsound2[1]  = new CSound();
	foxsound2[1]->Initialize(SeriROM.SetFilePositionMMF("burrowdn.wav"),1);

	iFox2->mImageLoad(SeriROM.SetFilePosition("fox2.res"));
	Foxrect2.top = 41;
	Foxrect2.left = 44;
	Foxrect2.bottom = 77;
	Foxrect2.right = 46;
	foxmask2[1] = GetMask(NULL,SeriROM.SetFilePosition("fox.msk"));
	foxmask2[0] = GetMask(NULL,SeriROM.SetFilePosition("fox2.msk"));

}

// 이미지 프리
void DropFox2Sprite()
{
	delete iFox2;
	delete foxmask2[0];
	delete foxmask2[1];
	delete foxsound2[0];
	delete foxsound2[1];

}


ENEMYfox2::ENEMYfox2(int px,int py)
{
	type = _NOTHING;

	status = _IN_LAND;

	life = FOX2LIFE;
	x = px;
	y = py;
	frame = 0;
	count2 = 2;
	count = getrandom(40,50);
	invincibility = 0;
	motion = _STAND_R;
	
	this->LoadMask(NULL,SeriROM.SetFilePosition("fox.msk"));
	delete checkbl;
	checkbl = foxmask2[0];
	iFox2->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	isize = iFox2->GetImageSize(); 
	this->cliprect = (RECT *)&Foxrect2;
}

void ENEMYfox2::action(void)
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
					if(iFox2->GetMotionFrame()<=frame)
					{
						frame = 0;
						if(count2--<=0) 
						{
							count = 2;
							frame = 0;
							count2 = 4;
							motion = _JUMP_R;
							status = _UPUP_LAND;
							if(InScreen)foxsound2[0]->Play(0);
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
				  if(iFox2->GetMotionFrame()<=frame)
				  {
					  type   = _ENEMY_FOX2;
					  status = _OUT_LAND;
					  motion = _WALK_R;
					  count=getrandom(20,30);
					  frame = 0;
					 // ClearMyTiles(0,0);
					  checkbl = foxmask2[1];
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
					count = 3;
					frame = 0;
					ClearMyTiles(0,0);
				    checkbl = foxmask2[0];
					count2 = 7;
					if(InScreen)foxsound2[1]->Play(0);
				}
			}
			break;
		case _ATTACK_LAND:
			{
				
				if(count <=0)
				{
					frame++;
					if(iFox2->GetMotionFrame() <= frame)
					{
						count = 2;
						InsertObject(new ENEMYeggshoot(x+getrandom(10,100),y+110+getrandom(-5,5),4,3));	
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
				  if(iFox2->GetMotionFrame()<=frame)
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
				  if(iFox2->GetMotionFrame()<=frame)
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
					if(iFox2->GetMotionFrame()<=frame)
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


void ENEMYfox2::putSprite(void)
{
    iFox2->iSendMessage(_dolRESETMOTION,NULL,motion,frame);
	POINT temppos;
	temppos.x = x - mapmanager->layer2.x;
	temppos.y = y - mapmanager->layer2.y-16;

	if(x+isize.x - mapmanager->layer2.x>=0 && x-mapmanager->layer2.x < 640 && y + isize.y - mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
	{
		iFox2->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,9);
		InScreen = 1;
	} else InScreen = 0;
}


void ENEMYfox2::SendState(int j)
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
			    this->checkbl = foxmask2[0];
				foxsound2[1]->Play(0);
			}
		break;
	}
}