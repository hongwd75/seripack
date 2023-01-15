// audio.h

#ifndef AUDIO_H
#define AUDIO_H
#include"stdafx.h"
#include<dsound.h>


// InitDirectSound()�Լ��� ���ϰ�
#define INITAUDIO_FAILURE		(0)
#define INITAUDIO_SUCCESS		(1)
#define INITAUDIO_NODRIVER		(2)

class CAudio
{
public:
	CAudio() ;
	~CAudio() ;

	LONG InitDirectSound(HWND hWnd) ;
	void RemoveDirectSound() ;

	LONG GetMasterVolume() ;
// ������ ���� ����. (0 ~ 100) ... ������ �Ŀ� ���� ���� �����ϴ� �Ҹ����� ����ǹǷ� ����.
	void SetMasterVolume(LONG lVolume) ;
	
	BOOL CreateSoundBuffer(LPWAVEFORMATEX lpWaveFormatEx, DWORD dwBufferSize, LPDIRECTSOUNDBUFFER* lplpDSbuffer) ;
	BOOL CreateSoundBuffer2(LPWAVEFORMATEX lpWaveFormatEx, DWORD dwBufferSize, LPDIRECTSOUNDBUFFER* lplpDSbuffer);
	BOOL DuplicateSoundBuffer(LONG lTotalChannelNum, LPDIRECTSOUNDBUFFER* lplpDSbuffer) ;

private:
	LPDIRECTSOUND m_lpDirectSound ;
	LONG          m_lMasterVolume ;
} ;


#define SOUNDPLAY_LOOP			(DSBPLAY_LOOPING)
#define SOUNDPLAY_ONCE			(0)

// CSound::Play()�Լ����� ������ �߻������� ����.
#define SOUNDPLAY_ERROR			(-1)
// CSound::Play()�Լ����� ��� ���۰� �������̹Ƿ� ���ְ� �Ұ��� �Ҷ� ����.
#define SOUNDPLAY_BUFFERFULL	(-2)

class CSound
{
public:
	CSound() ;
	~CSound() ;

	BOOL Initialize(LPSTR lpFileName, LONG lTotalChannelNum,int types = 0) ;
	BOOL Initialize(HMMIO files, LONG lTotalChannelNum,int types = 0);
	void Remove() ;

// �������� ä�ι�ȣ�� ����.
	LONG Play(DWORD dwRepeatMode, LONG lVolume = 100, LONG lPanning = 0) ;
// �������� ä���� ������ ����. (0 ~ 100)
	BOOL SetVolume(LONG lChannelNum, LONG lVolume) ;
// �������� ä���� �дװ� ����. (-10000 ~ 10000)
	BOOL SetPanning(LONG lChannelNum, LONG lPanning) ;
// ���� ����.
	BOOL Stop(LONG lChannelNum) ;
// ��� ����.
	BOOL Pause(LONG lChannelNum);
// �翬��.
	BOOL RePlay(LONG lChannelNum);
	BOOL IsPlaying(LONG lChannelNum);
// ��� ä���� ���� ����.
	BOOL Stop() ;

private:
	LPDIRECTSOUNDBUFFER* m_lplpDSbuffer ;
	LONG                 m_lTotalChannelNum ;

	LPBYTE LoadWave(LPSTR lpFileName, LPWAVEFORMATEX lpWaveFormatEx, LPDWORD lpWaveLength) ;
	LPBYTE LoadWave(HMMIO files, LPWAVEFORMATEX lpWaveFormatEx, LPDWORD lpWaveLength) ;
	BOOL   CopyWave(LPBYTE lpWaveData, DWORD dwWaveLength) ;
} ;

#endif
