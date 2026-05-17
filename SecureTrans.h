// SecureTrans.h : main header file for the SECURETRANS application
//

#if !defined(AFX_SECURETRANS_H__2AC2C039_4A96_41F9_8503_460906BE24B4__INCLUDED_)
#define AFX_SECURETRANS_H__2AC2C039_4A96_41F9_8503_460906BE24B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSecureTransApp:
// See SecureTrans.cpp for the implementation of this class
//

class CSecureTransApp : public CWinApp
{
public:
	CSecureTransApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecureTransApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSecureTransApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECURETRANS_H__2AC2C039_4A96_41F9_8503_460906BE24B4__INCLUDED_)
