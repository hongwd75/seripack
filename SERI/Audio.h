// audio.h

#ifndef AUDIO_H
#define AUDIO_H
#include"stdafx.h"
#include<dsound.h>


// InitDirectSound()함수의 리턴값
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
// 마스터 볼륨 지정. (0 ~ 100) ... 지정한 후에 새로 연주 시작하는 소리에만 적용되므로 주의.
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

// CSound::Play()함수에서 에러가 발생했을때 리턴.
#define SOUNDPLAY_ERROR			(-1)
// CSound::Play()함수에서 모든 버퍼가 연주중이므로 연주가 불가능 할때 리턴.
#define SOUNDPLAY_BUFFERFULL	(-2)

class CSound
{
public:
	CSound() ;
	~CSound() ;

	BOOL Initialize(LPSTR lpFileName, LONG lTotalChannelNum,int types = 0) ;
	BOOL Initialize(HMMIO files, LONG lTotalChannelNum,int types = 0);
	void Remove() ;

// 연주중인 채널번호를 리턴.
	LONG Play(DWORD dwRepeatMode, LONG lVolume = 100, LONG lPanning = 0) ;
// 연주중인 채널의 볼륨값 지정. (0 ~ 100)
	BOOL SetVolume(LONG lChannelNum, LONG lVolume) ;
// 연주중인 채널의 패닝값 지정. (-10000 ~ 10000)
	BOOL SetPanning(LONG lChannelNum, LONG lPanning) ;
// 연주 중지.
	BOOL Stop(LONG lChannelNum) ;
// 잠시 중지.
	BOOL Pause(LONG lChannelNum);
// 재연주.
	BOOL RePlay(LONG lChannelNum);
	BOOL IsPlaying(LONG lChannelNum);
// 모든 채널을 연주 중지.
	BOOL Stop() ;

private:
	LPDIRECTSOUNDBUFFER* m_lplpDSbuffer ;
	LONG                 m_lTotalChannelNum ;

	LPBYTE LoadWave(LPSTR lpFileName, LPWAVEFORMATEX lpWaveFormatEx, LPDWORD lpWaveLength) ;
	LPBYTE LoadWave(HMMIO files, LPWAVEFORMATEX lpWaveFormatEx, LPDWORD lpWaveLength) ;
	BOOL   CopyWave(LPBYTE lpWaveData, DWORD dwWaveLength) ;
} ;

#endif
