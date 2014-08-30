// ACDetect.h : main header file for the ACDETECT application
//

#if !defined(AFX_ACDETECT_H__B2740552_595E_4835_9387_0344CD1606EF__INCLUDED_)
#define AFX_ACDETECT_H__B2740552_595E_4835_9387_0344CD1606EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CACDetectApp:
// See ACDetect.cpp for the implementation of this class
//

class CACDetectApp : public CWinApp
{
public:
	CACDetectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACDetectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CACDetectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACDETECT_H__B2740552_595E_4835_9387_0344CD1606EF__INCLUDED_)
