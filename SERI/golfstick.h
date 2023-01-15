#ifndef __GOLFSTICK_H__
#define __GOLFSTICK_H__

#include"stdafx.h"
#include"superobjectclass.h"


class  Golfstick: public ObjectBase 
{
	private:
		int hole;
		int frame;
		int x;
		int y;
		int count;
		int layer;
		int addy;
		int endy;

	public:
		static pIMAGE *image[2];

		Golfstick(int where);
		void action();
		void putSprite();
		void SendState(int j);
		void frontDelete() {};
};

#endif
