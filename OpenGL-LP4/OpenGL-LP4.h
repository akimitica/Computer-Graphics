
// OpenGL-LP4.h : main header file for the OpenGL-LP4 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COpenGLLP4App:
// See OpenGL-LP4.cpp for the implementation of this class
//

class COpenGLLP4App : public CWinApp
{
public:
	COpenGLLP4App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGLLP4App theApp;
