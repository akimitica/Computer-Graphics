#include "pch.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#pragma comment(lib, "GL\\glut32.lib")
#pragma comment(lib, "GL\\GLU32.lib")

#define PI 3.1415926535897932384626433832795

void CGLRenderer::CalculatePosition()
{
	viewPosition[0] = viewR * cos(viewAngle[0]) * cos(viewAngle[1]);
	viewPosition[1] = viewR * sin(viewAngle[0]);
	viewPosition[2] = viewR * cos(viewAngle[0]) * sin(viewAngle[1]);


	lookingAt[0] = 0;
	lookingAt[1] = lookAtHeight;
	lookingAt[2] = 0;
}

CGLRenderer::CGLRenderer()
{
	this->m_hrc = NULL;
	viewR = 15;
	viewAngle[0] = PI * 30 / 180;
	viewAngle[1] = PI * 45 / 180;
	
	CalculatePosition();

	upVector[0] = 0;
	upVector[1] = 1;
	upVector[2] = 0;
}

CGLRenderer::~CGLRenderer()
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(0.5, 0.75, 1, 0.0);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	CalculatePosition();
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	gluLookAt(this->viewPosition[0], this->viewPosition[1], this->viewPosition[2], this->lookingAt[0], this->lookingAt[1], this->lookingAt[2], this->upVector[0], this->upVector[1], this->upVector[2]);

	DrawAxis();
	DrawGrid(10, 10, 10, 10);
	DrawFigure(angle_cx);
	
	
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	double stepA = 2 * PI / nSegAlpha;
	double stepB = 2 * PI / nSegAlpha;

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSegAlpha; i++) {
		for (int j = 0; j < nSegBeta; j++) {
			glVertex3f(r * cos(stepA * i) * cos(stepB * j), r + r * sin(stepA * i), r * cos(stepA * i) * sin(stepB * j));
			glVertex3f(r * cos(stepA * (i + 1)) * cos(stepB * j), r + r * sin(stepA * (i + 1)), r * cos(stepA * (i + 1)) * sin(stepB * j));
		}
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	float step = 2 * PI / nSeg;

	//Baza 1
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++)
	{
		glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
	}
	glEnd();

	//Baza 2
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++)
		glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
	glEnd();

	//Omotac

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSeg + 1; i++) 
	{
		glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
		glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
	}
	glEnd();
}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	float step = 2 * PI / nSeg;
	//Baza
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++) 
	{
		glVertex3f(cos(step * i) * r, 0, sin(step * i) * r);
	}
	glEnd();

	//Omotac
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, h, 0);
	for (int i = 0; i < nSeg + 1; i++) 
	{
		glVertex3f(cos(step * i) * r, 0, sin(step * i) * r);
	}
	glEnd();
}

void CGLRenderer::DrawGrid(double w, double h, int nSegW, int nSegH)
{
	double startX = -w / 2;
	double startY = -h / 2;

	double stepX = w / nSegW;
	double stepY = h / nSegH;

	glBegin(GL_LINES);

	glColor3f(1, 1, 1);
	for (int i = 0; i < nSegW + 1; i++) {
		glVertex3f(startX + stepX * i, 0, startY);
		glVertex3f(startX + stepX * i, 0, startY + h);
	}
	for (int i = 0; i < nSegH + 1; i++) {
		glVertex3f(startX, 0, startY + stepY * i);
		glVertex3f(startX + w, 0, startY + stepY * i);
	}
	glEnd();
}

void CGLRenderer::DrawAxis()
{
	glLineWidth(1.0);

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(10, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10);
	glEnd();
}

void CGLRenderer::DrawFigure(double angle)
{
	glPushMatrix();

	//nacrtaj saksiju
	glColor3f(1, 0.6, 0);
	DrawCylinder(1.8, 1, 1.5, 8);
	glTranslatef(0, 1.6, 0);
	DrawCylinder(0.4, 1.5, 1.5, 8);

	glPushMatrix();

	glColor3f(0, 1, 0);
	DrawBranch(0, false);
	{
		//levo dete
		glPushMatrix();
		glColor3f(0, 1, 0);
		DrawBranch(45, false);
		glColor3f(0, 1, 0);
		DrawBranch(0, true);
		glPopMatrix();
		//srednje dete
		glPushMatrix();
		glColor3f(0, 1, 0);
		DrawBranch(0, false);
		{
			glPushMatrix();
			glColor3f(1, 1, 0);
			DrawBranch(45 + angle, false);
			glColor3f(0, 1, 0);
			DrawBranch(0, true);
			glPopMatrix();
		}
		{
			glPushMatrix();
			glColor3f(0, 1, 0);
			DrawBranch(-45, false);
			glColor3f(0, 1, 0);
			DrawBranch(0, false);
			glPopMatrix();
		}
		glPopMatrix();
		{
			glPushMatrix();
			glColor3f(0, 1, 0);
			DrawBranch(-45, false);
			glColor3f(0, 1, 0);
			DrawBranch(0, true);
			glColor3f(0, 1, 0);
			DrawBranch(0, true);
			glPopMatrix();
		}
		
	}
	glPopMatrix();

	glPopMatrix();
}


void CGLRenderer::DrawBranch(double angle, bool isCone)
{
	glRotatef(angle, 0.0, 0.0, 1.0);
	glTranslatef(0, 0.2, 0);
	if (isCone) {
		DrawCone(1, 0.3, 6);
	}
	else {
		DrawCylinder(1, 0.3, 0.3, 6);
	}
	glTranslatef(0, 1, 0);
	glColor3f(0, 0.5, 0);
	DrawSphere(0.2, 15, 15);
	glTranslatef(0, 0.2, 0);
}
