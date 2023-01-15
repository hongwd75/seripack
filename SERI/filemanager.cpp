#include"stdafx.h"
#include"filemanager.h"


BOOL clFileManager::Init(LPSTR filename)
{
	FILE *fp;
	char filename2[_MAX_PATH];

	wsprintf(ROMFile,"%s.ROM",filename);
	wsprintf(filename2,"%s.LST",filename);

	fp  = fopen(filename2,"rb");

	if(fp == NULL) return FALSE;

	fread(&this->Items,4,1,fp);
	int i = 0;
	while(i < this->Items)
	{
		fread((void *)this->DATA[i].filename,15,1,fp);
		fread(&this->DATA[i].filepos,4,1,fp);
		i++;
	}
	fclose(fp);
	return TRUE;
}



////////////////////////////////////////////////////////////////
BOOL clFileManager::Close(void)
{
	return TRUE;
}


////////////////////////////////////////////////////////////////
clFileManager::clFileManager(void)
{
	
}


////////////////////////////////////////////////////////////////
clFileManager::~clFileManager(void)
{
	this->Close();
}



////////////////////////////////////////////////////////////////
FILE* clFileManager::SetFilePosition(LPSTR filename)
{

	file = NULL;
	char lowfilename[15],lowdata[15];

	wsprintf(lowfilename,"%s",_strlwr(filename));
   
	for(int i = 0; i < this->Items; i++)
	{
		wsprintf(lowdata,"%s",_strlwr(this->DATA[i].filename));	
		if(strcmp(lowfilename, lowdata) == 0)
		{
			file = fopen(ROMFile,"rb");
			if(file == NULL) return NULL;

			fseek(file,this->DATA[i].filepos,SEEK_SET);
			return file;
		}
	}
	
	return NULL;
}



////////////////////////////////////////////////////////////////
FILE* clFileManager::SetFilePosition(int index)
{
	if(index >= this->Items) return NULL;
	file = fopen(ROMFile,"rb");
	if(file == NULL) return NULL;
	fseek(file,this->DATA[index].filepos,SEEK_SET);
	return file;
}



HMMIO clFileManager::SetFilePositionMMF(LPSTR filename)
{

	HMMIO hmfr;
	char lowfilename[15],lowdata[15];


	wsprintf(lowfilename,"%s",_strlwr(filename));
   
	for(int i = 0; i < this->Items; i++)
	{
		wsprintf(lowdata,"%s",_strlwr(this->DATA[i].filename));	
		if(strcmp(lowfilename, lowdata) == 0)
		{

			hmfr = mmioOpen(ROMFile, NULL, MMIO_READ | MMIO_ALLOCBUF) ;
			if (hmfr == NULL) return NULL ;
			mmioSeek(hmfr, this->DATA[i].filepos,SEEK_SET);
			return hmfr;
		}
	}
	return NULL;
}

tagSeriRELOADSystem tpSERISYSTEM;


void initSystemFile(void)
{
	int i;
	for(i = 0; i < 15; i++)	tpSERISYSTEM.CARD.CardHave[i] = 0;
	tpSERISYSTEM.addpower = 0;
	for(i = 0; i < 5; i++) tpSERISYSTEM.BattleCard[i] = 0;
	tpSERISYSTEM.life = 0;
	tpSERISYSTEM.score = 0;
	tpSERISYSTEM.stageswitch = -1;		// Not Saved
}

void SaveSystemFile(int stage,int life,int maxlife,int score,int addpower)
{
	FILE *fp;
	tpSERISYSTEM.life = life;
	tpSERISYSTEM.maxlife = maxlife;
	tpSERISYSTEM.addpower = addpower;
	tpSERISYSTEM.score = score;
	tpSERISYSTEM.stageswitch = stage;
	fp = fopen("C:\\SERI.DAT","wb");
	fwrite((BYTE *)&tpSERISYSTEM,sizeof(tagSeriRELOADSystem),1,fp);
	fclose(fp);
}

void OpenSystemFile(void)
{
	FILE *fp;

	fp = fopen("C:\\SERI.DAT","rb");
	if(fp != NULL)
	{
		fread((void *)&tpSERISYSTEM,sizeof(tagSeriRELOADSystem),1,fp);
		fclose(fp);
	}
	for(int i = 0; i < 5; i++) tpSERISYSTEM.BattleCard[i] = 0;
}