
// 17805.h : main header file for the 17805 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy17805App:
// See 17805.cpp for the implementation of this class
//

class CMy17805App : public CWinApp
{
public:
	CMy17805App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy17805App theApp;
