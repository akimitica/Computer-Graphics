
// GDI-LP3.h : main header file for the GDI-LP3 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGDILP3App:
// See GDI-LP3.cpp for the implementation of this class
//

class CGDILP3App : public CWinApp
{
public:
	CGDILP3App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGDILP3App theApp;
