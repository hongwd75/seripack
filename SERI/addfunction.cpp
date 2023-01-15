#include"stdafx.h"
#include"video.h"
//#include"defineheader.h"

BOOL ImageClicked(int x,int y,POINT mpos,pIMAGE *img,WORD	biN)
{
	int posx,posy;

	posx = mpos.x - x;
	posy = mpos.y - y;

	if(posx <0 || posx > img->width) 
	{
		return FALSE;
	}
	if(posy <0 || posy >= img->height) 
	{
		return FALSE;
	}

	if(img->ImageType == _IMAGE_NORMAL)
	{
		if(img->image[posy * img->width + posx] != biN) return TRUE;
	} else
	{
		int address = img->Yarray[posy];
		int pattern = img->image[address++];
		int compx=0;
		int jump;
		while(pattern--)
		{
			jump = img->image[address++];
			if(compx<=posx && compx+jump>=posx) return FALSE;
			compx+=jump;

			jump = img->image[address++];
			if(compx<=posx && compx+jump>=posx) return TRUE;
			address+=jump;
			compx+=jump;
		}
	}
	return FALSE;
}