#ifndef __ENEMYSHOOT_H__
#define __ENEMYSHOOT_H__

#include"stdafx.h"
#include"superobjectclass.h"



#define  ENEMY_SPR					20
//  ������ �ִ� ���������� ���� �Ѿ� 
class ENEMYeggshoot  : public ObjectBase 
{
		private:
			int frame;
			int count;
			int count2;
			int count3;
			int life;
			int status;
			int name;
			int addx;
			int addy;

		public:
			static pIMAGE *iShoot[ENEMY_SPR];
			ENEMYeggshoot(int px,int py,int wh,int power);
			ENEMYeggshoot(int px,int py,int adx,int ady,int wh,int power);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};
#endif 