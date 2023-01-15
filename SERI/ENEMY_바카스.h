#ifndef __BAKA_H__
#define __BAKA_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYbaka  : public ObjectBase 
{
		private:
			int frame;
			int count;
			int count2;
			int count3;
			int life;
			int status;
			int lookright;
			int addx;
			int addy;
			int fade;

		public:
			ENEMYbaka(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};

#endif 