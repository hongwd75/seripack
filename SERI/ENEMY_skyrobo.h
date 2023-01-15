#ifndef __SKY_ROBO_H__
#define __SKY_ROBO_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYskyrobo  : public ObjectBase 
{
		private:
			int frame;
			int count;
			int life;
			int status;
			int lookright;
			int fade;

			int addx;
			int addy;
			int count2;

		public:
			ENEMYskyrobo(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};

#endif 