#include"stdafx.h"
#include"superobjectclass.h"
#include"filemanager.h"

#define getrandom(min,max) ((rand() % (int)(((max)+1) - (min)))+(min))

extern ImagePorcessClass    *Videomanager;
extern clFileManager		 SeriROM;
extern clFileManager		 MiniROM;



pIMAGE *Golfstick::image[2];

extern ObjectBase *Begin,*End;

void GetGolfSprite()
{
  Golfstick::image[0] = new pIMAGE();
  Golfstick::image[0]->Load(NULL,MiniROM.SetFilePosition("clup_n.spr")); 	
  Golfstick::image[1] = new pIMAGE();
  Golfstick::image[1]->Load(NULL,MiniROM.SetFilePosition("clup_d.spr")); 	
}


void DropGolfSprite()
{
	delete  Golfstick::image[0];
	delete  Golfstick::image[1];
}


Golfstick::Golfstick(int where)
{
	type = 2;
	hole = where;
	switch(where)
	{
		case 0:
			x = 22;
			y = -192;
			layer = 4;
			this->endy = 97;
			break;
		case 1:
			x = 230;
			y = -192;
			layer = 4;
			this->endy = 97;
			break;
		case 2:
			x = 438;
			y = -192;
			layer = 4;
			this->endy = 97;
			break;

		case 3:
			x = 22;
			y = -97;
			layer = 6;
			this->endy = 192;
			break;
		case 4:
			x = 230;
			y = -97;
			layer = 6;
			this->endy = 192;
			break;
		case 5:
			x = 438;
			y = -97;
			layer = 6;
			this->endy = 192;
			break;

		case 6:
			x = 22;
			y = 0;
			layer = 8;
			this->endy = 288;
			break;
		case 7:
			x = 230;
			y = 0;
			layer = 8;
			this->endy = 288;
			break;
		case 8:
			x = 438;
			y = 0;
			layer = 8;
			this->endy = 288;
		    break;
	}
	motion = 1;
	frame = 0;
	checkbl = NULL;
	addy = 8;
}

void Golfstick::action()
{
	switch(motion)
	{
		case 1:
			{
				for (ObjectBase *object = Begin; object != NULL; object = object -> next) 
				{
					if (object->type == 1 && object->motion == 2 && object->maxgo == hole) 
					{
						object->SendState(SEND_DAMAGE);
						frame = 1;
					}
				}
				motion = 2;
			}
		break;

		case 2:
			{
				if(endy > 0)
				{
					frame = 0;
					y-=addy;
					endy-=addy;
					if(addy<20) addy+=5;
				} else 	if(y<0) DisappearObject(SELF);
			}
		break;
	}
}


void Golfstick::putSprite()
{
	Videomanager->AddSpr(x,y,image[frame],_ANTIMODE,layer);
}


void Golfstick::SendState(int j)
{

}