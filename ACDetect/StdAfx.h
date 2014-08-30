// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__BF155F97_8771_4B8D_A4D7_C7036BA8D954__INCLUDED_)
#define AFX_STDAFX_H__BF155F97_8771_4B8D_A4D7_C7036BA8D954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_STDAFX_H__BF155F97_8771_4B8D_A4D7_C7036BA8D954__INCLUDED_)


#pragma comment(lib, "GDI//gdiplus.lib")
#define ULONG_PTR ULONG //防止MFC中的ULONG_PTR和GDI+的ULONG_PTR冲突
#include "GDI//GdiPlus.h"
using namespace Gdiplus;