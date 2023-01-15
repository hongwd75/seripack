#ifndef __CHIKEN_H__
#define __CHIKEN_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYchiken : public ObjectBase 
{
		private:
			int frame;
			int count;
			int life;
			int status;
			int lookright;
			int count2;
			int fade;

		public:
			ENEMYchiken(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};

#endif 