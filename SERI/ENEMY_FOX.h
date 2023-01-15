#ifndef __FOX_H__
#define __FOX_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYfox  : public ObjectBase 
{
		private:
			int frame;
			int count;
			int count2;
			int status;
			int life;

		public:
			ENEMYfox(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() { this->checkbl = NULL;};
};

#endif 