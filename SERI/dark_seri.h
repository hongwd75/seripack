#ifndef __DARKSERICLASS_H__
#define __DARKSERICLASS_H__


#include"stdafx.h"
#include"superobjectclass.h"


//////////////////////////////////////////////////////
// ���� 
class DarkSeriObject : public ObjectBase 
{
	private:
		WORD	status;
		WORD    status2;
		int		lookright;
		int		oldstatus;
		int		oldlook;
		int		tick1;
		int		WeaponDelay;
		int		life;
		int		level;
		int		count;
		int		frame;
        int     deltaH;
		int     AddSpeed;
		int     AddPower;
		int     sleep;
		int     motionran;
		int     motionran2;
		int     fade;
		
	public:
		static BYTE    *mask[2];
		int     AddMotionX;					// ������ ���� �߰��Ǵ� ���ΰ� 

	    DarkSeriObject(int px,int py);
		void ViewEnegy(void);
	    void action();
		void putSprite();
		void SendState(int j);
		void frontDelete() {};
};

#endif