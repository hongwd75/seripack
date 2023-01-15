#ifndef __ZERG_H__
#define __ZERG_H__

#include"stdafx.h"
#include"superobjectclass.h"

class ENEMYegg  : public ObjectBase 
{
		private:
			int life;
			int frame;
			int count;
			int status;
			int motion;
			int wherelook;

		public:
			static pIMAGE *egg[20];

			ENEMYegg(int px,int py,int where);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};

#endif 