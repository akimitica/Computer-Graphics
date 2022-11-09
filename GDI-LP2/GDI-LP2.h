
// GDI-LP2.h : main header file for the GDI-LP2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGDILP2App:
// See GDI-LP2.cpp for the implementation of this class
//

class CGDILP2App : public CWinApp
{
public:
	CGDILP2App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGDILP2App theApp;
