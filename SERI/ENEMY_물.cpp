#include"stdafx.h"
#include"superobjectclass.h"


extern ObjectBase *Begin,*End;
extern ImagePorcessClass    *Videomanager;
extern ObjectBase *Seripoint;

pIMAGE *ENEMYwater::water[3];

void GetwaterSprite()
{
	ENEMYwater::water[0] = new pIMAGE();
	ENEMYwater::water[0]->Load(NULL,SeriROM.SetFilePosition("water_00.spr"));
	ENEMYwater::water[1] = new pIMAGE();
	ENEMYwater::water[1]->Load(NULL,SeriROM.SetFilePosition("water_01.spr"));
	ENEMYwater::water[2] = new pIMAGE();
	ENEMYwater::water[2]->Load(NULL,SeriROM.SetFilePosition("water_02.spr"));
}

void DropwaterSprite()
{
	delete ENEMYwater::water[0];
	delete ENEMYwater::water[1];
	delete ENEMYwater::water[2];
}




ENEMYwater::ENEMYwater(int px,int py)
{
	type = _ENEMY_WATER;

	x      = px;
	y      = mapmanager->height[1] << 5;
	frame  = 1;
	count  = 0;
}


void ENEMYwater::action(void)
{

	count--;
	if(count<=0)
	{
		count=2;
		frame = 1 - frame;
		y-=8;
		if(y < 0) y = 0;
	}
	if(Seripoint->y > y) Seripoint->SendState(SEND_DAMAGE+(300 << 16));
}

void ENEMYwater::putSprite(void)
{
	int sy=32;
	if(y-mapmanager->layer2.y>0 && y-mapmanager->layer2.y<480)
		Videomanager->AddSpr(0, y-mapmanager->layer2.y,water[frame+1],_HALFTONEMODE,254);

	while(y-mapmanager->layer2.y+sy>0 && y-mapmanager->layer2.y+sy<480)
	{
		Videomanager->AddSpr(0, y-mapmanager->layer2.y+sy,water[0],_HALFTONEMODE,254);
		sy+=32;
	}
		
}


void ENEMYwater::SendState(int j)
{

}