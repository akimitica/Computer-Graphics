
// GDI-LP1.h : main header file for the GDI-LP1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGDILP1App:
// See GDI-LP1.cpp for the implementation of this class
//

class CGDILP1App : public CWinApp
{
public:
	CGDILP1App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGDILP1App theApp;
