#ifndef __GOREA_H__
#define __GOREA_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYgorea  : public ObjectBase 
{
		private:
			int frame;
			int count;
			int count2;
			int firedelay;
			int status;
			int life;
			int lookright;
			int fade;
			int maxgo;

		public:
			ENEMYgorea(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() { };
};

#endif 