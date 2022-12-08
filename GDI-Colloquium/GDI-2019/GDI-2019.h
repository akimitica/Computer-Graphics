
// GDI-2019.h : main header file for the GDI-2019 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGDI2019App:
// See GDI-2019.cpp for the implementation of this class
//

class CGDI2019App : public CWinApp
{
public:
	CGDI2019App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGDI2019App theApp;
