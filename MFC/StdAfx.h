//
// StdAfx.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//
// stdafx.h : standard system and project include files that rarely change.
//

#if !defined(AFX_STDAFX_H__11D0ECD6_2844_11D2_AB51_00E029217E15__INCLUDED_)
#define AFX_STDAFX_H__11D0ECD6_2844_11D2_AB51_00E029217E15__INCLUDED_

#define _WIN32_WINNT _WIN32_WINNT_MAXVER

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used definitions from Windows headers.

#ifdef _AFXDLL
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows common controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio inserts additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__11D0ECD6_2844_11D2_AB51_00E029217E15__INCLUDED_)
