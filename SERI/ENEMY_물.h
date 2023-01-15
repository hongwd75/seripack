#ifndef __EWATER_H__
#define __EWATER_H__

#include"stdafx.h"
#include"superobjectclass.h"
class ENEMYwater  : public ObjectBase 
{
		private:
			int frame;
			int count;

		public:
			static pIMAGE *water[3];
			ENEMYwater(int px,int py);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};
#endif 