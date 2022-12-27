
// OpenGL-LP6.h : main header file for the OpenGL-LP6 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COpenGLLP6App:
// See OpenGL-LP6.cpp for the implementation of this class
//

class COpenGLLP6App : public CWinApp
{
public:
	COpenGLLP6App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGLLP6App theApp;
