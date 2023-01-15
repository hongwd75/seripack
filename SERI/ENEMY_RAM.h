#ifndef __RAM_H__
#define __RAM_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYram  : public ObjectBase 
{
		private:
			int frame;
			int count;
			int life;
			int status;
			int lookright;
			int addx;
			int fade;

		public:
			ENEMYram(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};

#endif 