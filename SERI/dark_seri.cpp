#include"stdafx.h"
#include"SuperObjectClass.h"


mChar			*DarkSerichar;
extern ObjectBase *Seripoint;
SeriObject *SERIPOINT;

#define _SERI_STAND					1
#define _SERI_WALK					2
#define _SERI_RUN					4
//#define _SERI_DAMAGE				8

#define _JUMPFRAME					3			// 점프시 프레임

#define _MAXJUMPCOUNT				10
#define _JUMPADD					10


#define SERI_DAMAGE					100

#define NOMOVE						7
#define NOTHING						0
#define ATTACKREADY					1
#define ATTACK						2
#define DAMAGEREADY					3
#define DAMAGE						4
#define ATTACKREADY2				5
#define ATTACK2						6

#define FADEMODE1					12			
#define FADEMODE2					13
#define RENOTHING					10


#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))
extern int GateRet;
extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
extern tagSeridata	oldSeriState;

RECT   DARKSERIRECT[10];

CSound   *darksound[10];
extern CSound *goreasound;

pIMAGE  *Gage;
pIMAGE  *Gagebar;
LONG    Darkplaying;
BYTE    *DarkSeriObject::mask[2];
void GetDarkSeriFile()
{
	DarkSerichar   = new mChar(Videomanager);
	DarkSerichar->mImageLoad(SeriROM.SetFilePosition("darkseri.res"));
	Gage = new pIMAGE();
	Gagebar = new pIMAGE();
	Gage->Load(NULL,SeriROM.SetFilePosition("lastgage.spr"));
	Gagebar->Load(NULL,SeriROM.SetFilePosition("lastgage_b.spr"));

	for(int i =0; i < 7; i++)
	darksound[i]  = new CSound();
	darksound[0]->Initialize(SeriROM.SetFilePositionMMF("bat.wav"),1);			// 공격 2
	darksound[1]->Initialize(SeriROM.SetFilePositionMMF("kara_m.wav"),1);		// 공격 1
	darksound[2]->Initialize(SeriROM.SetFilePositionMMF("c_iyaaa.wav"),1);	// 공격할때 내는 소리
	darksound[3]->Initialize(SeriROM.SetFilePositionMMF("damage.wav"),1);	// damage
	darksound[4]->Initialize(SeriROM.SetFilePositionMMF("ro_out.wav"),1);		// 죽었다.
	darksound[5]->Initialize(SeriROM.SetFilePositionMMF("gardck.wav"),1);		// 애들 때렸다..
	darksound[6]->Initialize(MiniROM.SetFilePositionMMF("bonk.wav"),1);

}


void DropDarkSeriFile()
{
	for(int i =0; i < 7; i++)
	{
		darksound[i]->Stop();
		delete darksound[i];
	}
	delete DarkSerichar;
	delete Gage;
	delete Gagebar;

}	

//////////////////////////////////////////////////////////////////////////////
void DarkSeriObject::ViewEnegy(void)
{
	Videomanager->AddSpr(50,100,Gagebar,_ANTIMODE,255);
	if(life<58)
	{
		if(life<0) life = 0;
		for(int i = 0; i < 58-life;i++) Videomanager->AddSpr(53+232-(i<<2),105,Gage,_ANTIMODE,255);
	}
}


//////////////////////////////////////////////////////////////////////////////
int savestatus;
DarkSeriObject::DarkSeriObject(int sx,int sy)
{
		type = _SERI_DARK;
		x = sx;
		y = sy;
		AddSpeed = 0;
		AddPower = 0;
		AddMotionX = 0;
		WeaponDelay = 0;
		invincibility = 0;

		life   = 58;


		status2 = NOTHING;
		oldlook = lookright = 1;
		sleep = 0;

		count = 0;
		frame = 0;

///////////////////////
		savestatus = oldstatus = status = FALLING;
		deltaH = 0;
		count = 10;
////////////////////////
		DarkSerichar->iSendMessage(_dolINIT,NULL,0,0);
		DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_FALLING_L-lookright,0);

		isize = DarkSerichar->GetImageSize();
		isize.x = isize.x >> 1;
		LoadMask(NULL,SeriROM.SetFilePosition("seri.msk"));

		DARKSERIRECT[0].top = 65;
		DARKSERIRECT[0].bottom = 110;
		DARKSERIRECT[0].left = 167 - isize.x;
		DARKSERIRECT[0].right = 56;

		DARKSERIRECT[1].top = 65;
		DARKSERIRECT[1].bottom = 110;
		DARKSERIRECT[1].left = 29;
		DARKSERIRECT[1].right = 60;

		cliprect = (RECT *)&DARKSERIRECT[lookright];
		motionran = getrandom(1,4);
		motionran2 = 0;
		SERIPOINT = (SeriObject *)Seripoint;

}

void DarkSeriObject::action()
{

	int temp1;
	int ress=0,ress2=0;
	int attackstart = getrandom(0,3);		// 4 이상이면 공격
	int mt2 = motionran2 &0xFF;	

	if(attackstart >=3) attackstart = getrandom(1,2);
	else attackstart = 0;

	// 1: 일반공격, 2: 고래공격형, 3: 폭탄 공격형
	if(Seripoint->x > x - 100 && Seripoint->x < x + 128 && getrandom(0,4) == 3)
	{
		attackstart = 1;				   // 일반 찌르기
	}

  if(life>0)
  {
	if(motionran<=0)
	{
		if(Seripoint->x < x+AddMotionX) 
		{
			lookright = 0;
			AddMotionX = -isize.x;
		}
		else if(Seripoint->x> x+AddMotionX) 
		{
			lookright = 1;
			AddMotionX = 0;
		}
		motionran = getrandom(5,20);
		motionran2 = ( getrandom(0,4) << 8 ) + getrandom(0,4);
	}
	motionran--;


	if(status2 == ATTACK)
	{
		POINT re = DarkSerichar->GetMotionNumber();
		if(re.y == 1 && re.x == 0) 
		{
			darksound[1]->Play(0);
		//	sound[2]->Play(0);
		}
		if(re.y == 2 && re.x == 0)
		{
			if(attackstart==2)
			{
				goreasound->Play(0);
				if(lookright)
				{
					InsertObject(new ENEMYeggshoot(x+110+getrandom(-4,12),y+37+getrandom(-3,3),6,5));
					InsertObject(new ENEMYeggshoot(x+110+getrandom(-4,12),y+37+getrandom(-3,3),6,5));
					InsertObject(new ENEMYeggshoot(x+110+getrandom(-4,12),y+37+getrandom(-3,3),6,5));
				} else 
				{
					InsertObject(new ENEMYeggshoot(x+16+getrandom(-12,4),y+37+getrandom(-3,3),5,5));
					InsertObject(new ENEMYeggshoot(x+16+getrandom(-12,4),y+37+getrandom(-3,3),5,5));
					InsertObject(new ENEMYeggshoot(x+16+getrandom(-12,4),y+37+getrandom(-3,3),5,5));
				}
			}

			ObjectBase *object = Seripoint;
			{
				{
					if(lookright)
					{
						if( CheckCrash( OBJ_X, OBJ_X + OBJ_XL, OBJ_Y, OBJ_Y + OBJ_YL,
					                x+174 , x+174+74,y+103,y+103+34))
						{
							object->SendState(SEND_DAMAGE+((10+AddPower) << 16));
							darksound[5]->Play(0);
						}
					} else 
					{
						if( CheckCrash( OBJ_X, OBJ_X + OBJ_XL, OBJ_Y, OBJ_Y + OBJ_YL,
					                x+6+AddMotionX , x+6+76+AddMotionX,y+105,y+105+34))
						{
							object->SendState(SEND_DAMAGE+((10+AddPower) << 16));
							darksound[5]->Play(0);
						}

					}
				}
			}
		}

		if(DarkSerichar->iSendMessage(_dolNEXTFRAME,NULL,0,0)) 
		{
			status2 = RENOTHING;
			oldstatus = -1;
		}
	}


    if(AddSpeed>10) AddSpeed=10;
	if((mt2 ==1 || mt2==2)  && status2 == NOTHING)
	{
		if (status == ON_LAND_RUN ) 
		{
			ress = this->IsMove(MOVE_LEFT,11); // x-=9;
			AddSpeed++;
		}
			else ress = this->IsMove(MOVE_LEFT,7); // x-=6;
		if(status == JUMPING || status == FALLING) ress = this->IsMove(MOVE_LEFT,(AddSpeed >> 1));
		if(lookright) 
		{
			lookright = 0;
			AddSpeed = 0;
			cliprect = (RECT *)&DARKSERIRECT[lookright];
			AddMotionX = -isize.x;
		}
	}

	if((mt2 == 3 || mt2 == 4) && status2 == NOTHING)
	{
		if (status == ON_LAND_RUN ) 
		{
			ress = this->IsMove(MOVE_RIGHT,11);
			AddSpeed++;
		}
		else ress = this->IsMove(MOVE_RIGHT,7);
		if(status == JUMPING || status == FALLING) ress = this->IsMove(MOVE_RIGHT,AddSpeed >> 1);
		if(lookright ==0)
		{
			cliprect = (RECT *)&DARKSERIRECT[lookright];
			lookright = 1;
			AddSpeed = 0;
			AddMotionX = 0;
		}
	}
	if((SERIPOINT->status2 == ATTACK || SERIPOINT->status2 == ATTACK2) && status2 == NOTHING)
	{
		temp1 = getrandom(1,2);
		if(temp1 == 2)
		{
			if(SERIPOINT->lookright == 1)
			{
				type = _NOTHING;
				status = FADEMODE1;
				status2 = FADEMODE1;
				this->fade = 30;
			}
		}
	}

	if(status == FADEMODE1)
	{
		fade-=3;
		if(fade<=0) 
		{
			fade = 0;
			status = FADEMODE2;
			ClearMyTiles();
			x = getrandom(200,600);
			y = getrandom(10,250);
		}
	}

	if(status == FADEMODE2)
	{
		fade+=3;
		if(fade>=30) 
		{
			fade = 31;
			status = FALLING;
			status2 = NOTHING;
			deltaH = 0;
			count = 10;
			type = _SERI_DARK;
			DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_FALLING_L-lookright,0);
		}
	}

	if(status2 == DAMAGE)
	{
		if(lookright)
		{
			this->IsMove(MOVE_LEFT,14);
		} else this->IsMove(MOVE_RIGHT,14);
		if(DarkSerichar->iSendMessage(_dolNEXTFRAME,NULL,0,0))  
		{
			status2 = RENOTHING;
			oldstatus = -1;
		}
	}


	switch(status)
	{
		case ON_LAND: case ON_LAND_WALK: case ON_LAND_RUN:
			if(!status2)
			{
				
				if(mt2 > 0 && mt2 < 5)	// 1,2 : LEFT 3,4:RIGHT
				{
					sleep = 0 ;
					if(mt2 == 2 || mt2 == 4)
					{
						status = ON_LAND_RUN;
					} else status = ON_LAND_WALK;
				} else status = ON_LAND;

				if(motionran2 >> 10)
				{
					status = JUMPREADY;
					count = 3;
				}
				ress2 = IsMove(MOVEOK_DOWN,3);
				if(!ress2)
				{
					status = FALLING;
					count = 4;
					deltaH = 4;
				}
				if(status != ON_LAND || sleep > 200) 
				{
					int ik;
					ik = DarkSerichar->iSendMessage(_dolNEXTFRAME,NULL,0,0);
					if(sleep>200 && ik == _RESETFRAME) DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_STAND_L-lookright,1);
					sleep = 201;
				} else
				{
					sleep++;
				}
			}
			break;

		case JUMPREADY:
			{
				deltaH = getrandom(3,6)*8;
				status = JUMPING;
				count = _MAXJUMPCOUNT;
			}
			break;

		case JUMPING:
			{
				if(deltaH<=0)
				{
					status = FALLING;
					count = 10;
					deltaH=4;
					break;
				}

				int addyy,addyy2=0;
				if(deltaH > 15) 
				{
					addyy=15;
					addyy2 = deltaH - 15;
					if(addyy2>10) addyy2 = 10;
				} else addyy=deltaH;
				deltaH-=4;
				ress2 = this->IsMove(MOVE_UP,addyy);
				if(!ress2) 	ress2 = this->IsMove(MOVE_UP, addyy2);//ress2 = this->IsMove(MOVE_UP,addyy+addyy2);
				if(ress2)
				{
					status = FALLING;
					deltaH = 0;
					count = 10;
				}
			}
			break;

		case FALLED:
			{
				count++;
				if(count>2)
				{
					status = ON_LAND;
					status2 = NOTHING;
					count = 0 ;
					AddSpeed = 0;
				}
			}
			break;
		case FALLING:
			{
				int addyy,addyy2=0;
				deltaH+=3;
				if(deltaH>15) 
				{
					addyy=15;
					addyy2 = deltaH - 15;
					if(addyy2>10) addyy2 = 10;
				} else addyy=deltaH;
				ress2 = this->IsMove(MOVEOK_DOWN,addyy);
				if(!ress2) 	ress2 = this->IsMove(MOVEOK_DOWN,addyy+addyy2);
					else addyy2 = 0;
				int resxx = this->IsMove(MOVE_DOWN,addyy);
				int resx = this->IsMove(MOVE_DOWN,addyy2);
				if(resx || resxx)
				{
					status = FALLED;
					status2 = NOTHING;
					count = 0 ;
					AddSpeed = 0;
				}
			}
			break;
	}
	if( attackstart && status2==NOTHING && WeaponDelay <= 0)
	{
		if(status !=JUMPREADY)
		{
			WeaponDelay = 8;
			status2 = ATTACKREADY;
			frame = 0;
		}
	}	
	} else
	{
	  if(!darksound[4]->IsPlaying(Darkplaying))
	  {
		DisappearObject(SELF);
		SeriData->gameend = 10;
	  }
	}

	if(WeaponDelay>0) WeaponDelay--;
	if(invincibility) invincibility--;

}



void DarkSeriObject::putSprite()
{
	if((oldstatus != status || oldlook != lookright) && (status2 == RENOTHING || status2 == NOTHING))
	{
		oldstatus = status;
		oldlook  = lookright;
		status2 = NOTHING;

		switch(status)
		{
			case ON_LAND:
				DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_STAND_L-lookright,0);
				sleep = 0;
				break;
			case ON_LAND_WALK:
				DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_WALK_L-lookright,0);
				break;
			case ON_LAND_RUN:
				DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_RUN_L-lookright,0);
				break;
			case ON_LAND_DAMAGE: //_DAMAGE_LAND_R
				DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_DAMAGE_LAND_L-lookright,0);
				break;
			case JUMPREADY:
				DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_JUMP_L-lookright,0);
				break;
			 case JUMPING: case JUMP_I:
				DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_JUMP_L-lookright,1);
				break;
			case FALLING:
				DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_FALLING_L-lookright,0);
				break;
			case FALLED:
				{
					DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_FALLING_L-lookright,1);
					status2=NOMOVE;
				}
				break;
		}
	}

	if(status2 == ATTACKREADY || status2 == DAMAGEREADY || status2 == ATTACKREADY2)
	{
		switch(status2)
		{
			case ATTACKREADY:
				if(status == ON_LAND || status == ON_LAND_WALK || status == ON_LAND_RUN)
					DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_ATTACK1_L-lookright,0);
				else DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_JUMPATTACK_L-lookright,0);
				status2 = ATTACK;
			break;

			case ATTACKREADY2:
				if(status == ON_LAND || status == ON_LAND_WALK || status == ON_LAND_RUN)
					DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_ATTACK2_L-lookright,0);
				else DarkSerichar->iSendMessage(_dolRESETMOTION,NULL,_JUMPATTACK_L-lookright,0);
				status2 = ATTACK2;
				
			break;

			case DAMAGEREADY:
				if(status == ON_LAND || status == ON_LAND_WALK || status == ON_LAND_RUN)
					DarkSerichar->iSendMessage(_dolRESETMOTION, NULL, _DAMAGE_LAND_L - lookright, 0);
				else DarkSerichar->iSendMessage(_dolRESETMOTION, NULL, _DAMAGE_SKY_L - lookright, 0);
				status2 = DAMAGE;
				count = 0;
				darksound[3]->Play(0);
			break;
		}
	}

	POINT temppos;
	temppos.x = x - mapmanager->layer2.x+AddMotionX; 
	temppos.y = y - mapmanager->layer2.y-15;

	if(status == FADEMODE1 || status == FADEMODE2)
	{
		DarkSerichar->iSendMessage(_dolSHOW_ALPHA,(void *)&temppos,fade,11);
	} else 
	{
		if(invincibility)
		{
			if(invincibility & 1) DarkSerichar->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,10);
		} else DarkSerichar->iSendMessage(_dolSHOW,(void *)&temppos,_ANTIMODE,10);
	}

	ViewEnegy();
}


void DarkSeriObject::SendState(int j)
{
	int j2 = j & 0xFF;
	int dam = j >> 16;
	switch(j2)
	{
		case SEND_DAMAGE:
			if(!invincibility && status2 != DAMAGE)
			{
				status2 = DAMAGEREADY;
				life-=dam;
				if(life<=0) 
				{
					Darkplaying = darksound[4]->Play(0);
				}
				invincibility = 20;
				count = 0;
			}
		break;

		case SEND_POWERUP:
				AddPower+=dam;
		break;
		case SEND_LIFEADD:
			{
				life+=dam;
				if(life>58) life = 58;
			}
		break;
		case SEND_LIFEFULL:
			{
				life = 58;
			}
		break;

	}
}