#ifndef __DOOR_H__
#define __DOOR_H__

#include"stdafx.h"
#include"superobjectclass.h"



class DOOR : public ObjectBase 
{
		public:
			static pIMAGE *idoor;
			DOOR(int px,int py,int ijump);
			void action();
			void putSprite();
			void SendState(int j);
			void frontDelete() {};
};

#endif 