#ifndef DVDPLAYER_CODEC_H_
#define DVDPLAYER_CODEC_H_

/*
 *      Copyright (C) 2005-2008 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "ICodec.h"
#include "FileSystem/File.h"

#include "DVDDemuxers/DVDDemux.h"
#include "DVDCodecs/Audio/DVDAudioCodec.h"
#include "DVDInputStreams/DVDInputStream.h"

class DVDPlayerCodec : public ICodec
{
public:
  DVDPlayerCodec();
  virtual ~DVDPlayerCodec();

  virtual bool Init(const CStdString &strFile, unsigned int filecache);
  virtual void DeInit();
  virtual bool CanSeek();
  virtual __int64 Seek(__int64 iSeekTime);
  virtual int ReadPCM(BYTE *pBuffer, int size, int *actualsize);
  virtual int ReadData(BYTE *pBuffer, int size, int *actualsize);
  virtual bool CanInit();

  virtual bool UsePassthrough() const { return m_bPassthrough; }
  virtual bool UseHWDecoding()  const { return m_bHWDecode; }

  virtual enum PCMChannels* GetChannelMap() { return m_pAudioCodec ? m_pAudioCodec->GetChannelMap() : NULL; };
  virtual AudioMediaFormat GetDataFormat();

  void SetContentType(const CStdString &strContent);

private:
  void SetAudioFormat(int codec, unsigned char flags);

  BYTE m_probeBuffer[2560];
  int m_probeBufferLen, m_probeBufferPos;

  CDVDDemux* m_pDemuxer;
  CDVDInputStream* m_pInputStream;
  CDVDAudioCodec* m_pAudioCodec;

  CStdString m_strContentType;
  AudioMediaFormat m_audioFormat;

  int m_nAudioStream;

  int m_audioPos;
  DemuxPacket* m_pPacket ;

  BYTE *m_decoded;
  int  m_nDecodedLen;

  bool m_bPassthrough;
  bool m_bHWDecode;
};

#endif
