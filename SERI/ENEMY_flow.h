#ifndef __FLOW_H__
#define __FLOW_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYflow : public ObjectBase 
{
		private:
			int frame;
			int count;
			int life;
			int status;
			int lookright;
			int fade;

		public:
			ENEMYflow(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() { };
};

#endif 