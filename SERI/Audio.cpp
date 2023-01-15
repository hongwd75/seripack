// audio.cpp
#include"stdafx.h"
#include"audio.h"
//#include<mmsystem.h>

extern  CAudio Audio;

CAudio::CAudio()
{
	m_lpDirectSound = NULL ;
	m_lMasterVolume = 100 ;
}

CAudio::~CAudio()
{
	RemoveDirectSound() ;
}

LONG CAudio::InitDirectSound(HWND hWnd)
{
	RemoveDirectSound() ;

	HRESULT hResult ;

	hResult = DirectSoundCreate(NULL, &m_lpDirectSound, NULL) ;
	if (hResult == DSERR_NODRIVER) return INITAUDIO_NODRIVER ;
	if (hResult != DS_OK         ) return INITAUDIO_FAILURE ;

	hResult = m_lpDirectSound->SetCooperativeLevel(hWnd, DSSCL_NORMAL) ;
	if (hResult != DS_OK) return INITAUDIO_FAILURE ;

	return INITAUDIO_SUCCESS ;
}

void CAudio::RemoveDirectSound()
{
	if (m_lpDirectSound != NULL)
	{
		m_lpDirectSound->Release() ;
		m_lpDirectSound = NULL ;
	}
}

LONG CAudio::GetMasterVolume()
{
	return m_lMasterVolume ;
}

void CAudio::SetMasterVolume(LONG lVolume)
{
	if (lVolume > 100) lVolume = 100 ;
	if (lVolume <   0) lVolume =   0 ;

	m_lMasterVolume = lVolume ;
}

BOOL CAudio::CreateSoundBuffer(LPWAVEFORMATEX lpWaveFormatEx, DWORD dwBufferSize, LPDIRECTSOUNDBUFFER* lplpDSbuffer)
{
	DSBUFFERDESC  dsBufferDesc ;
	HRESULT 	  hResult ;

	memset(&dsBufferDesc, 0, sizeof(DSBUFFERDESC)) ;
	dsBufferDesc.dwSize        = sizeof(DSBUFFERDESC) ;
	dsBufferDesc.dwFlags       = DSBCAPS_STATIC |  DSBCAPS_CTRLDEFAULT |DSBCAPS_GETCURRENTPOSITION2;//DSBCAPS_CTRLDEFAULT ;
	dsBufferDesc.dwBufferBytes = dwBufferSize ;
	dsBufferDesc.lpwfxFormat   = lpWaveFormatEx ;

	hResult = m_lpDirectSound->CreateSoundBuffer(&dsBufferDesc, lplpDSbuffer, NULL) ;

	return (hResult != DS_OK) ? FALSE : TRUE ;
}

BOOL CAudio::CreateSoundBuffer2(LPWAVEFORMATEX lpWaveFormatEx, DWORD dwBufferSize, LPDIRECTSOUNDBUFFER* lplpDSbuffer)
{
	DSBUFFERDESC  dsBufferDesc ;
	HRESULT 	  hResult ;

	memset(&dsBufferDesc, 0, sizeof(DSBUFFERDESC)) ;
	dsBufferDesc.dwSize        = sizeof(DSBUFFERDESC) ;
	dsBufferDesc.dwFlags       = DSBCAPS_LOCHARDWARE | DSBCAPS_CTRLDEFAULT |DSBCAPS_GETCURRENTPOSITION2;//DSBCAPS_CTRLDEFAULT ;
	dsBufferDesc.dwBufferBytes = dwBufferSize ;
	dsBufferDesc.lpwfxFormat   = lpWaveFormatEx ;

	hResult = m_lpDirectSound->CreateSoundBuffer(&dsBufferDesc, lplpDSbuffer, NULL) ;
	
	if(hResult != DS_OK)
	{
		memset(&dsBufferDesc, 0, sizeof(DSBUFFERDESC)) ;
		dsBufferDesc.dwSize        = sizeof(DSBUFFERDESC) ;
		dsBufferDesc.dwFlags       = DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLDEFAULT | DSBCAPS_GETCURRENTPOSITION2;//DSBCAPS_CTRLDEFAULT ;
		dsBufferDesc.dwBufferBytes = dwBufferSize ;
		dsBufferDesc.lpwfxFormat   = lpWaveFormatEx ;
		hResult = m_lpDirectSound->CreateSoundBuffer(&dsBufferDesc, lplpDSbuffer, NULL) ;

	}

	return (hResult != DS_OK) ? FALSE : TRUE ;
}


BOOL CAudio::DuplicateSoundBuffer(LONG lTotalChannelNum, LPDIRECTSOUNDBUFFER* lplpDSbuffer)
{
	HRESULT hResult ;

	for (LONG loop = 1; loop < lTotalChannelNum; loop++)
	{
		hResult = m_lpDirectSound->DuplicateSoundBuffer(lplpDSbuffer[0], &lplpDSbuffer[loop]) ;
		if (hResult != DS_OK) return FALSE ;
	}

	return TRUE ;
}


CSound::CSound()
{
	m_lplpDSbuffer     = NULL ;
	m_lTotalChannelNum = 0 ;
}

CSound::~CSound()
{
	Remove() ;
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
BOOL CSound::Initialize(LPSTR lpFileName, LONG lTotalChannelNum, int types)
{
	Remove() ;

	WAVEFORMATEX waveFormatEx ;
	LPBYTE       waveData ;
	DWORD        waveLength ;
	BOOL         result ;

	m_lTotalChannelNum = lTotalChannelNum ;
	m_lplpDSbuffer     = new LPDIRECTSOUNDBUFFER [m_lTotalChannelNum] ;

	waveData = LoadWave(lpFileName, &waveFormatEx, &waveLength) ;
	if (waveData == NULL) return FALSE ;

	if(types==1)
	   result = Audio.CreateSoundBuffer(&waveFormatEx, waveLength, &m_lplpDSbuffer[0]);
	else 
		result = Audio.CreateSoundBuffer2(&waveFormatEx, waveLength, &m_lplpDSbuffer[0]);
	if (result == FALSE) return FALSE ;

	result = CopyWave(waveData, waveLength) ;
	if (result == FALSE) return FALSE ;

	result = Audio.DuplicateSoundBuffer(m_lTotalChannelNum, m_lplpDSbuffer) ;
	if (result == FALSE) return FALSE ;

	return TRUE ;
}

///////////////////////////////////////////////////////////

BOOL CSound::Initialize(HMMIO files, LONG lTotalChannelNum, int types)
{
	Remove() ;

	WAVEFORMATEX waveFormatEx ;
	LPBYTE       waveData ;
	DWORD        waveLength ;
	BOOL         result ;

	m_lTotalChannelNum = lTotalChannelNum ;
	m_lplpDSbuffer     = new LPDIRECTSOUNDBUFFER [m_lTotalChannelNum] ;

	waveData = LoadWave(files, &waveFormatEx, &waveLength) ;
	if (waveData == NULL) return FALSE ;

	if(types==1)
	   result = Audio.CreateSoundBuffer(&waveFormatEx, waveLength, &m_lplpDSbuffer[0]);
	else 
		result = Audio.CreateSoundBuffer2(&waveFormatEx, waveLength, &m_lplpDSbuffer[0]);
	if (result == FALSE) return FALSE ;

	result = CopyWave(waveData, waveLength) ;
	if (result == FALSE) return FALSE ;

	result = Audio.DuplicateSoundBuffer(m_lTotalChannelNum, m_lplpDSbuffer) ;
	if (result == FALSE) return FALSE ;

	return TRUE ;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void CSound::Remove()
{
	if (m_lplpDSbuffer != NULL)
	{
		for (LONG channel = m_lTotalChannelNum - 1; channel >= 0; channel--)
		{
			if (m_lplpDSbuffer[channel] != NULL)
			{
				m_lplpDSbuffer[channel]->Release() ;
				m_lplpDSbuffer[channel] = NULL ;
			}
		}

		delete m_lplpDSbuffer ;
		m_lplpDSbuffer = NULL ;
	}
}

#define LOADWAVE_ERROR()		{ mmioClose(hmfr, 0) ; return NULL ; }

LPBYTE CSound::LoadWave(LPSTR lpFileName, LPWAVEFORMATEX lpWaveFormatEx, LPDWORD lpWaveLength)
{
	HMMIO    hmfr ;
	MMCKINFO parent, child ;

	parent.ckid         = (FOURCC)0 ;
	parent.cksize       = 0 ;
	parent.fccType      = (FOURCC)0 ;
	parent.dwDataOffset = 0 ;
	parent.dwFlags      = 0 ;

	child = parent ;

	hmfr = mmioOpen(lpFileName, NULL, MMIO_READ | MMIO_ALLOCBUF) ;
	if (hmfr == NULL) return NULL ;

	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E') ;
	if (mmioDescend(hmfr, &parent, NULL, MMIO_FINDRIFF))
		LOADWAVE_ERROR() ;

	child.ckid = mmioFOURCC('f', 'm', 't', ' ') ;
	if (mmioDescend(hmfr, &child, &parent, 0))
		LOADWAVE_ERROR() ;

	if (mmioRead(hmfr, (char *)lpWaveFormatEx, sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX))
		LOADWAVE_ERROR() ;

	if (lpWaveFormatEx->wFormatTag != WAVE_FORMAT_PCM)
		LOADWAVE_ERROR() ;

	if (mmioAscend(hmfr, &child, 0))
		LOADWAVE_ERROR() ;

	child.ckid = mmioFOURCC('d', 'a', 't', 'a') ;
	if (mmioDescend(hmfr, &child, &parent, MMIO_FINDCHUNK))
		LOADWAVE_ERROR() ;

	LPBYTE waveData = new BYTE [child.cksize] ;
	if ((DWORD)mmioRead(hmfr, (char *)waveData, child.cksize) != child.cksize)
	{
		delete waveData ;
		LOADWAVE_ERROR() ;
	}

	mmioClose(hmfr, 0) ;

	*lpWaveLength = child.cksize ;
	return waveData ;
}


LPBYTE CSound::LoadWave(HMMIO files, LPWAVEFORMATEX lpWaveFormatEx, LPDWORD lpWaveLength)
{
	HMMIO    hmfr = files;
	MMCKINFO parent, child ;

	parent.ckid         = (FOURCC)0 ;
	parent.cksize       = 0 ;
	parent.fccType      = (FOURCC)0 ;
	parent.dwDataOffset = 0 ;
	parent.dwFlags      = 0 ;

	child = parent ;

	//hmfr = mmioOpen(lpFileName, NULL, MMIO_READ | MMIO_ALLOCBUF) ;
	if (hmfr == NULL) return NULL ;

	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E') ;
	if (mmioDescend(hmfr, &parent, NULL, MMIO_FINDRIFF))
		LOADWAVE_ERROR() ;

	child.ckid = mmioFOURCC('f', 'm', 't', ' ') ;
	if (mmioDescend(hmfr, &child, &parent, 0))
		LOADWAVE_ERROR() ;

	if (mmioRead(hmfr, (char *)lpWaveFormatEx, sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX))
		LOADWAVE_ERROR() ;

	if (lpWaveFormatEx->wFormatTag != WAVE_FORMAT_PCM)
		LOADWAVE_ERROR() ;

	if (mmioAscend(hmfr, &child, 0))
		LOADWAVE_ERROR() ;

	child.ckid = mmioFOURCC('d', 'a', 't', 'a') ;
	if (mmioDescend(hmfr, &child, &parent, MMIO_FINDCHUNK))
		LOADWAVE_ERROR() ;

	LPBYTE waveData = new BYTE [child.cksize] ;
	if ((DWORD)mmioRead(hmfr, (char *)waveData, child.cksize) != child.cksize)
	{
		delete waveData ;
		LOADWAVE_ERROR() ;
	}

	mmioClose(hmfr, 0) ;

	*lpWaveLength = child.cksize ;
	return waveData ;
}



#define COPYWAVE_ERROR()		{ delete lpWaveData ; return FALSE ; }

BOOL CSound::CopyWave(LPBYTE lpWaveData, DWORD dwWaveLength)
{
	HRESULT hResult ;
	LPVOID  write1 ;
	LPVOID  write2 ;
	DWORD   length1 ;
	DWORD   length2 ;

	hResult = m_lplpDSbuffer[0]->Lock(0, dwWaveLength, &write1, &length1, &write2, &length2, 0) ;
	if (hResult == DSERR_BUFFERLOST)
	{
		m_lplpDSbuffer[0]->Restore() ;
		hResult = m_lplpDSbuffer[0]->Lock(0, dwWaveLength, &write1, &length1, &write2, &length2, 0) ;
	}
	if (hResult != DS_OK) COPYWAVE_ERROR() ;

	memcpy(write1, lpWaveData, length1) ;
	if (write2 != NULL) memcpy(write2, lpWaveData + length1, length2) ;

	hResult = m_lplpDSbuffer[0]->Unlock(write1, length1, write2, length2) ;
	if (hResult != DS_OK) COPYWAVE_ERROR() ;

	delete lpWaveData ;
	return TRUE ;
}

LONG CSound::Play(DWORD dwRepeatMode, LONG lVolume, LONG lPanning)
{
	HRESULT hResult ;
	BOOL    result ;
	DWORD   status ;

	for (LONG channelNum = 0; channelNum < m_lTotalChannelNum; channelNum++)
	{
		m_lplpDSbuffer[channelNum]->GetStatus(&status) ;
		if (status) continue ;

		result = SetVolume(channelNum, lVolume) ;
		if (result != TRUE) return SOUNDPLAY_ERROR ;

		result = SetPanning(channelNum, lPanning) ;
		if (result != TRUE) return SOUNDPLAY_ERROR ;

		hResult = m_lplpDSbuffer[channelNum]->Play(0, 0, dwRepeatMode) ;
		if (hResult != DS_OK) return SOUNDPLAY_ERROR ;

		return channelNum ;
	}

	return SOUNDPLAY_BUFFERFULL ;
}


BOOL CSound::Pause(LONG lChannelNum)
{
	HRESULT hResult;
	hResult = m_lplpDSbuffer[lChannelNum]->Stop();
	if (hResult != DS_OK) return FALSE ;
	return TRUE;
}


BOOL CSound::Stop(LONG lChannelNum)
{
	HRESULT hResult ;

	hResult = m_lplpDSbuffer[lChannelNum]->Stop() ;  
	if (hResult != DS_OK) return FALSE ;

	hResult = m_lplpDSbuffer[lChannelNum]->SetCurrentPosition(0) ;
	if (hResult != DS_OK) return FALSE ;

	return TRUE ;
}

BOOL CSound::Stop()
{
	BOOL result ;

	for (LONG channelNum = 0; channelNum < m_lTotalChannelNum; channelNum++)
	{
		result = Stop(channelNum) ;
		if (result != TRUE) return FALSE ;
	}
	return TRUE ;
}

BOOL CSound::SetVolume(LONG lChannelNum, LONG lVolume)
{
	if (lVolume > 100) lVolume = 100 ;
	if (lVolume <   0) lVolume =   0 ;

	lVolume = (lVolume * Audio.GetMasterVolume()) - 10000 ;		// -10000 ~ 0

	return (m_lplpDSbuffer[lChannelNum]->SetVolume(lVolume) != DS_OK) ? FALSE : TRUE ;
}

BOOL CSound::SetPanning(LONG lChannelNum, LONG lPanning)
{
	if (lPanning >  10000) lPanning =  10000 ;
	if (lPanning < -10000) lPanning = -10000 ;

	return (m_lplpDSbuffer[lChannelNum]->SetPan(lPanning) != DS_OK) ? FALSE : TRUE ;
}

BOOL CSound::IsPlaying(LONG lChannelNum)
{
	DWORD   status ;
	m_lplpDSbuffer[lChannelNum]->GetStatus(&status) ;
	if(status == DSBSTATUS_PLAYING || status == DSBSTATUS_LOOPING) return TRUE;
	return FALSE;
}