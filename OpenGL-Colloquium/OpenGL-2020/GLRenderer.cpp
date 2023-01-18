#include "pch.h"
#include "GLRenderer.h"
#include "DImage.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{

	this->m_hrc = NULL;
	eyeDistance = 20;
	lookingAtY = 2;
	upVectorY = 1;
	viewAngle[0] = 25 * TO_RADIAN;
	viewAngle[1] = 45 * TO_RADIAN;

	SetEyePosition();
}

CGLRenderer::~CGLRenderer(void)
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
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);

	spider = LoadTexture("spider.png");
	env[0] = LoadTexture("front.jpg");
	env[1] = LoadTexture("back.jpg");
	env[2] = LoadTexture("left.jpg");
	env[3] = LoadTexture("right.jpg");
	env[4] = LoadTexture("top.jpg");
	env[5] = LoadTexture("bot.jpg");
	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	SetEyePosition();

	DrawAxes();
	glPushMatrix();
	glTranslatef(0, 50, 0);
	DrawEnvCube(100);
	glPopMatrix();


	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}


void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	glDeleteTextures(1, &spider);
	glDeleteTextures(6, env);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}



void CGLRenderer::DrawAxes()
{
	glLineWidth(2.0);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	{
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(50, 0, 0);
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 50, 0);
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 50);
	}
	glEnd();
}


UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texId;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texId;
}


void CGLRenderer::DrawEnvCube(double a)
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, env[0]);
	glBegin(GL_QUADS);
	{
	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3d(-a / 2, a / 2, -a / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3d(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3d(a / 2, -a / 2, -a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3d(a / 2, a / 2, -a / 2);

	}
	glEnd();


	glBindTexture(GL_TEXTURE_2D, env[1]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3d(-a / 2, a / 2, a / 2);
		glTexCoord2f(1.0, 1.0);
		glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(0.0, 1.0);
		glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(0.0, 0.0);
		glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();


	glBindTexture(GL_TEXTURE_2D, env[2]);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3d(a / 2, a / 2, -a / 2);
		glTexCoord2f(0, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, env[2]);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3d(a / 2, a / 2, -a / 2);
		glTexCoord2f(0, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();


	glBindTexture(GL_TEXTURE_2D, env[2]);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3d(a / 2, a / 2, -a / 2);
		glTexCoord2f(0, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();

	//============== F L O O R ======================

	glBindTexture(GL_TEXTURE_2D, env[5]);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 0);
		glVertex3f(a / 2, -a / 2, -a / 2);
	}
	glEnd();
}


void CGLRenderer::SetEyePosition()
{
	eyePosition[0] = eyeDistance * cos(viewAngle[0]) * cos(viewAngle[1]);
	eyePosition[1] = eyeDistance * sin(viewAngle[0]);
	eyePosition[2] = eyeDistance * cos(viewAngle[0]) * sin(viewAngle[1]);

	upVectorY = cos(viewAngle[0]) < 0 ? -1 : 1;

	gluLookAt(eyePosition[0], eyePosition[1], eyePosition[2], 0, lookingAtY, 0, 0, upVectorY, 0);
}