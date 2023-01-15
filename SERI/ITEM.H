#ifndef __ITEM_H__
#define __ITEM_H__

#include"stdafx.h"
#include"superobjectclass.h"

#define ITEM_SIZE				4

class ITEM : public ObjectBase 
{
		private:
			int number;
			int status;
			int count;
			int add;
			int up;

		public:
			static pIMAGE *item[ITEM_SIZE];
			ITEM(int px,int py,int what);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};

#endif 