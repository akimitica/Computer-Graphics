
// OpenGL-2020.h : main header file for the OpenGL-2020 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COpenGL2020App:
// See OpenGL-2020.cpp for the implementation of this class
//

class COpenGL2020App : public CWinApp
{
public:
	COpenGL2020App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGL2020App theApp;
