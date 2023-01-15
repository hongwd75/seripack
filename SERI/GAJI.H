#ifndef __GAJI_H__
#define __GAJI_H__

#include"stdafx.h"
#include"superobjectclass.h"


#define GAJIUP					2
#define GAJIDOWN				5
#define GAJIDAMAGE				7

#define GAJIMERONG				4

class  Gaji: public ObjectBase 
{
	private:
		int hole;
		int frame;
		int x;
		int y;
		int count;
		int layer;
		int delays;

	public:
		static pIMAGE *up[GAJIUP];
		static pIMAGE *down[GAJIDOWN];
		static pIMAGE *damage[GAJIDAMAGE];
		static pIMAGE *merong[GAJIMERONG];

		Gaji(int where,int delay = 0);
		void action();
		void putSprite();
		void SendState(int j);
		void frontDelete() {};
};

#endif
