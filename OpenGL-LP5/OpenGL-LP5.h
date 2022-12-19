
// OpenGL-LP5.h : main header file for the OpenGL-LP5 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COpenGLLP5App:
// See OpenGL-LP5.cpp for the implementation of this class
//

class COpenGLLP5App : public CWinApp
{
public:
	COpenGLLP5App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGLLP5App theApp;
