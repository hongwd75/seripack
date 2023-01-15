#ifndef __GHOST_H__
#define __GHOST_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYghost  : public ObjectBase 
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
			ENEMYghost(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};

#endif 