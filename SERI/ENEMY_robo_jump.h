#ifndef __ROBO_JUMP_H__
#define __ROBO_JUMP_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYrobo2  : public ObjectBase 
{
		private:
			int frame;
			int count;
			int count2;
			int firedelay;
			int status;
			int life;
			int addy;
			int lookright;

		public:
			ENEMYrobo2(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() { };
};

#endif 