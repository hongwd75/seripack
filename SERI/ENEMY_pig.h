#ifndef __PIG_H__
#define __PIG_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYpig : public ObjectBase 
{
		private:
			int frame;
			int count;
			int life;
			int status;
			int status2;
			int lookright;
			int fade;
			int count2;

		public:
			ENEMYpig(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};

#endif 