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

#include "system.h"

#ifdef HAS_GL
#include "../xbmc/Settings.h"
#include "WindowingFactory.h"
#include "FrameBufferObject.h"
#include "utils/log.h"

//////////////////////////////////////////////////////////////////////
// CFrameBufferObject
//////////////////////////////////////////////////////////////////////

CFrameBufferObject::CFrameBufferObject()
{
  m_fbo = 0;
  m_valid = false;
  m_supported = false;
  m_bound = false;
  m_texid = 0;
}

CFrameBufferObject::~CFrameBufferObject()
{
  Cleanup();
}

bool CFrameBufferObject::IsSupported()
{
  if(g_Windowing.IsExtSupported("GL_EXT_framebuffer_object"))
    m_supported = true;
  else
    m_supported = false;
  return m_supported;
}

bool CFrameBufferObject::Initialize()
{
  if (!IsSupported())
    return false;

  Cleanup();

  glGenFramebuffersEXT(1, &m_fbo);
  VerifyGLState();

  if (!m_fbo)
    return false;

  m_valid = true;
  return true;
}

void CFrameBufferObject::Cleanup()
{
  if (!IsValid())
    return;

  if (m_fbo)
    glDeleteFramebuffersEXT(1, &m_fbo);

  if (m_texid)
    glDeleteTextures(1, &m_texid);

  m_texid = 0;
  m_fbo = 0;
  m_valid = false;
  m_bound = false;
}

bool CFrameBufferObject::CreateAndBindToTexture(GLenum target, int width, int height, GLenum format,
                                                GLenum filter, GLenum clampmode)
{
  if (!IsValid())
    return false;

  if (m_texid)
    glDeleteTextures(1, &m_texid);

  glGenTextures(1, &m_texid);
  glBindTexture(target, m_texid);
  glTexImage2D(target, 0, (format==GL_BGRA)?GL_RGBA:format,  width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(target, GL_TEXTURE_WRAP_S, clampmode);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, clampmode);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
  glBindTexture(target, 0);
 
  return BindToTexture(target, m_texid);
}

void CFrameBufferObject::SetFiltering(GLenum target, GLenum mode)
{
  glBindTexture(target, m_texid);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mode);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, mode);
}

bool CFrameBufferObject::BindToTexture(GLenum target, GLuint texid)
{
  if (!IsValid())
    return false;

  m_bound = false;
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
  glBindTexture(target, texid);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, target, texid, 0);
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
  {
    CLog::Log(LOGERROR,"**** FAILED to create fbo. status: %x", status);
    return false;
  }
  m_bound = true;
  return true;
}

#endif