#ifndef __EMOUSE_H__
#define __EMOUSE_H__

#include"stdafx.h"
#include"superobjectclass.h"
class ENEMYmouse  : public ObjectBase 
{
		private:
			int frame;
			int count;
			int status;
			int motion;
			int leftlook;
			int life;

		public:
			static pIMAGE *mouse[6];
			ENEMYmouse(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};
#endif 