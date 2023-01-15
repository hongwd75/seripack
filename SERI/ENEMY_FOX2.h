#ifndef __FOX2_H__
#define __FOX2_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYfox2  : public ObjectBase 
{
		private:
			int frame;
			int count;
			int count2;
			int status;
			int life;

		public:
			ENEMYfox2(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() { this->checkbl = NULL;};
};

#endif 