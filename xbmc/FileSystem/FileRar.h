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

// FileRar.h: interface for the CFileRar class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FILERAR_H__C6E9401A_3715_11D9_8185_0050FC718317__INCLUDED_)
#define AFX_FILERAR_H__C6E9401A_3715_11D9_8185_0050FC718317__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IFile.h"
#include "lib/UnrarXLib/rar.hpp"
#include "utils/Thread.h"
#ifdef HAS_RAR
#endif

namespace XFILE
{	
#ifdef HAS_RAR
  class CFileRarExtractThread : public CThread
  {
  public:
    CFileRarExtractThread();
    ~CFileRarExtractThread();
    
    void Start(Archive* pArc, CommandData* pCmd, CmdExtract* pExtract, int iSize); 
    
    virtual void OnStartup();
     virtual void OnExit();
     virtual void Process();

     HANDLE hRunning;
     HANDLE hRestart;
     HANDLE hQuit;
  protected:
    Archive* m_pArc;
    CommandData* m_pCmd;
    CmdExtract* m_pExtract;
    int m_iSize;
  };
#endif

  class CFileRar : public IFile  
	{
	public:
		CFileRar();
    CFileRar(bool bSeekable); // used for caching files
		virtual ~CFileRar();
    virtual int64_t        GetPosition();
    virtual int64_t        GetLength();
    virtual bool          Open(const CURI& url);
		virtual bool					Exists(const CURI& url);
		virtual int						Stat(const CURI& url, struct __stat64* buffer);
    virtual unsigned int  Read(void* lpBuf, int64_t uiBufSize);
    virtual int            Write(const void* lpBuf, int64_t uiBufSize);
    virtual int64_t        Seek(int64_t iFilePosition, int iWhence=SEEK_SET);
    virtual void					Close();
		virtual void          Flush();

    virtual bool          OpenForWrite(const CURI& url);
    unsigned int          Write(void *lpBuf, int64_t uiBufSize);
		
	protected:
		CStdString	m_strCacheDir;
		CStdString	m_strRarPath;
		CStdString m_strPassword;
		CStdString m_strPathInRar;
    CStdString m_strUrl;
		BYTE m_bRarOptions;
		BYTE m_bFileOptions;
    void Init();
		void InitFromUrl(const CURI& url);
    bool OpenInArchive();
    void CleanUp();
    
    int64_t m_iFilePosition;
    int64_t m_iFileSize;
    // rar stuff
    bool m_bUseFile;
    bool m_bOpen;
    bool m_bSeekable;
    CFile m_File; // for packed source
#ifdef HAS_RAR
    Archive* m_pArc;
    CommandData* m_pCmd;
    CmdExtract* m_pExtract;
    CFileRarExtractThread* m_pExtractThread;
#endif
    int m_iSize; // header size
    byte* m_szBuffer;
    byte* m_szStartOfBuffer;
    int64_t m_iDataInBuffer;
    int64_t m_iBufferStart;
	};

}
#endif // !defined(AFX_FILERAR_H__C6E9401A_3715_11D9_8185_0050FC718317__INCLUDED_)
