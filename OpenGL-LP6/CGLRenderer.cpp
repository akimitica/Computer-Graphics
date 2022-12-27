#include "pch.h"
#include "CGLRenderer.h"
#include "DImage.h"
#include "GL\gl.h"
#include "GL\glu.h"
#define SQUARE 0.0625

CGLRenderer::CGLRenderer()
{
	this->m_hrc = NULL;
	eyeDistance = 20;
	lookingAtY = 2;
	upVectorY = 1;
	viewAngle[0] = 25 * TO_RADIAN;
	viewAngle[1] = 45 * TO_RADIAN;

	SetEyePosition();
}

CGLRenderer::~CGLRenderer()
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0)
		return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (bResult == FALSE)
		return false;

	this->m_hrc = wglCreateContext(pDC->m_hDC);

	if (this->m_hrc == NULL)
		return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	groundTexture = LoadTexture("ground.jpg");
	texture = LoadTexture("texture.png");

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, this->m_hrc);
	wglMakeCurrent(NULL, NULL);

	if (this->m_hrc != NULL)
	{
		wglDeleteContext(this->m_hrc);
		this->m_hrc = NULL;
	}
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, this->m_hrc);

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	SetEyePosition(); 

	GLfloat lmodel_ambient[] = { 0.9, 0.9, 0.9, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	ViewPointLight();
	DrawAxis(10);
	glEnable(GL_LIGHTING);

	DrawGround();
	DrawTruck();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::SetEyePosition()
{
	eyePosition[0] = eyeDistance * cos(viewAngle[0]) * cos(viewAngle[1]);
	eyePosition[1] = eyeDistance * sin(viewAngle[0]);
	eyePosition[2] = eyeDistance * cos(viewAngle[0]) * sin(viewAngle[1]);

	upVectorY = cos(viewAngle[0]) < 0 ? -1 : 1;

	gluLookAt(eyePosition[0], eyePosition[1], eyePosition[2], 0, lookingAtY, 0, 0, upVectorY, 0);
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

void CGLRenderer::DrawAxis(double width)
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(width, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, width, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, width);
	glEnd();
}

void CGLRenderer::DrawGround()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, groundTexture);

	glBegin(GL_QUADS);
	{
		glNormal3f(0.0, 1.0, 0.0);

		glTexCoord2f(0.0, 2.0);
		glVertex3f(-20.0, -1.5, 20.0);

		glTexCoord2f(2, 2);
		glVertex3f(20.0, -1.5, 20.0);

		glTexCoord2f(2, 0.0);
		glVertex3f(20.0, -1.5, -20.0);

		glTexCoord2f(0, 0);
		glVertex3f(-20.0, -1.5, -20.0);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawTruckSide()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Kabina
	glBegin(GL_POLYGON);
	{
		glTexCoord2f(0, 5 * SQUARE);
		glVertex3f(0, 2, 0);

		glTexCoord2f(0, 2 * SQUARE);
		glVertex3f(0, 5, 0);

		glTexCoord2f(2 * SQUARE, 0);
		glVertex3f(2, 7, 0);

		glTexCoord2f(4 * SQUARE, 0);
		glVertex3f(4, 7, 0);

		glTexCoord2f(4 * SQUARE, 4 * SQUARE);
		glVertex3f(4, 3, 0);

		glTexCoord2f(13 * SQUARE, 4 * SQUARE);
		glVertex3f(13, 3, 0);

		glTexCoord2f(13 * SQUARE, 5 * SQUARE);
		glVertex3f(13, 2, 0);
	}
	glEnd();

	//Prednje krilo
	{
		glBegin(GL_POLYGON);
		{
			glTexCoord2f(0, 7 * SQUARE);
			glVertex3f(0, 0, 0);

			glTexCoord2f(1 * SQUARE, 7 * SQUARE);
			glVertex3f(1, 0, 0);

			glTexCoord2f(1 * SQUARE, 6 * SQUARE);
			glVertex3f(1, 1, 0);

			glTexCoord2f(2 * SQUARE, 5 * SQUARE);
			glVertex3f(2, 2, 0);

			glTexCoord2f(0, 5 * SQUARE);
			glVertex3f(0, 2, 0);
		}
		glEnd();

		glBegin(GL_POLYGON);
		{
			glTexCoord2f(5 * SQUARE, 7 * SQUARE);
			glVertex3f(5, 0, 0);

			glTexCoord2f(4 * SQUARE, 7 * SQUARE);
			glVertex3f(4, 0, 0);

			glTexCoord2f(4 * SQUARE, 6 * SQUARE);
			glVertex3f(4, 1, 0);

			glTexCoord2f(3 * SQUARE, 5 * SQUARE);
			glVertex3f(3, 2, 0);

			glTexCoord2f(5 * SQUARE, 5 * SQUARE);
			glVertex3f(5, 2, 0);
		}
		glEnd();
	}

	//Zadnje krilo
	{
		glBegin(GL_POLYGON);
		{
			glTexCoord2f(5 * SQUARE, 7 * SQUARE);
			glVertex3f(5, 0, 0);

			glTexCoord2f(6 * SQUARE, 7 * SQUARE);
			glVertex3f(6, 0, 0);

			glTexCoord2f(6 * SQUARE, 6 * SQUARE);
			glVertex3f(6, 1, 0);

			glTexCoord2f(7 * SQUARE, 5 * SQUARE);
			glVertex3f(7, 2, 0);

			glTexCoord2f(5 * SQUARE, 5 * SQUARE);
			glVertex3f(5, 2, 0);
		}
		glEnd();

		glBegin(GL_POLYGON);
		{
			glTexCoord2f(10 * SQUARE, 7 * SQUARE);
			glVertex3f(10, 0, 0);

			glTexCoord2f(9 * SQUARE, 7 * SQUARE);
			glVertex3f(9, 0, 0);

			glTexCoord2f(9 * SQUARE, 6 * SQUARE);
			glVertex3f(9, 1, 0);

			glTexCoord2f(8 * SQUARE, 5 * SQUARE);
			glVertex3f(8, 2, 0);

			glTexCoord2f(10 * SQUARE, 5 * SQUARE);
			glVertex3f(10, 2, 0);
		}
		glEnd();
	}

	//Ostatak
	glBegin(GL_POLYGON);
	{
		glTexCoord2f(10 * SQUARE, 7 * SQUARE);
		glVertex3f(10, 0, 0);

		glTexCoord2f(13 * SQUARE, 7 * SQUARE);
		glVertex3f(13, 0, 0);

		glTexCoord2f(13 * SQUARE, 5 * SQUARE);
		glVertex3f(13, 2, 0);

		glTexCoord2f(10 * SQUARE, 5 * SQUARE);
		glVertex3f(10, 2, 0);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawTruckCover()
{
	glBegin(GL_QUAD_STRIP);
	{
		glNormal3b(-1, 0, 0);
		glVertex3d(-6.5, 0, -2.5);
		glVertex3d(-6.5, 5, -2.5);
		glVertex3d(-6.5, 0, 2.5);
		glVertex3d(-6.5, 5, 2.5);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		glNormal3d(-cos(PI / 4), sin(PI / 4), 0);
		glVertex3d(-6.5, 5, -2.5);
		glVertex3d(-4.5, 7, -2.5);
		glVertex3d(-4.5, 7, 2.5);
		glVertex3d(-6.5, 5, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(0, 1, 0);
		glVertex3d(-4.5, 7, -2.5);
		glVertex3d(-2.5, 7, -2.5);
		glVertex3d(-2.5, 7, 2.5);
		glVertex3d(-4.5, 7, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(1, 0, 0);
		glVertex3d(-2.5, 7, -2.5);
		glVertex3d(-2.5, 3, -2.5);
		glVertex3d(-2.5, 3, 2.5);
		glVertex3d(-2.5, 7, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(0, 1, 0);
		glVertex3d(-2.5, 3, -2.5);
		glVertex3d(6.5, 3, -2.5);
		glVertex3d(6.5, 3, 2.5);
		glVertex3d(-2.5, 3, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(1, 0, 0);
		glVertex3d(6.5, 3, -2.5);
		glVertex3d(6.5, 0, -2.5);
		glVertex3d(6.5, 0, 2.5);
		glVertex3d(6.5, 3, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(0, -1, 0);
		glVertex3d(6.5, 0, -2.5);
		glVertex3d(2.5, 0, -2.5);
		glVertex3d(2.5, 0, 2.5);
		glVertex3d(6.5, 0, 2.5);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		glNormal3d(-1, 0, 0);
		glVertex3d(2.5, 0, -2.5);
		glVertex3d(2.5, 1, -2.5);
		glVertex3d(2.5, 1, 2.5);
		glVertex3d(2.5, 0, 2.5);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		glNormal3d(-cos(PI / 4), -sin(PI / 4), 0);
		glVertex3d(2.5, 1, -2.5);
		glVertex3d(1.5, 2, -2.5);
		glVertex3d(1.5, 2, 2.5);
		glVertex3d(2.5, 1, 2.5);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		glNormal3d(0, -1, 0);
		glVertex3d(1.5, 2, -2.5);
		glVertex3d(0.5, 2, -2.5);
		glVertex3d(0.5, 2, 2.5);
		glVertex3d(1.5, 2, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(cos(PI / 4), -sin(PI / 4), 0);
		glVertex3d(0.5, 2, -2.5);
		glVertex3d(-0.5, 1, -2.5);
		glVertex3d(-0.5, 1, 2.5);
		glVertex3d(0.5, 2, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(1, 0, 0);
		glVertex3d(-0.5, 1, -2.5);
		glVertex3d(-0.5, 0, -2.5);
		glVertex3d(-0.5, 0, 2.5);
		glVertex3d(-0.5, 1, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(0, -1, 0);
		glVertex3d(-0.5, 0, -2.5);
		glVertex3d(-2.5, 0, -2.5);
		glVertex3d(-2.5, 0, 2.5);
		glVertex3d(-0.5, 0, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(1, 0, 0);
		glVertex3d(-2.5, 0, -2.5);
		glVertex3d(-2.5, 1, -2.5);
		glVertex3d(-2.5, 1, 2.5);
		glVertex3d(-2.5, 0, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(-cos(PI / 4), -sin(PI / 4), 0);
		glVertex3d(-2.5, 1, -2.5);
		glVertex3d(-3.5, 2, -2.5);
		glVertex3d(-3.5, 2, 2.5);
		glVertex3d(-2.5, 1, 2.5);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		glNormal3d(0, -1, 0);
		glVertex3d(-3.5, 2, -2.5);
		glVertex3d(-4.5, 2, -2.5);
		glVertex3d(-4.5, 2, 2.5);
		glVertex3d(-3.5, 2, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(cos(PI / 4), -sin(PI / 4), 0);
		glVertex3d(-4.5, 2, -2.5);
		glVertex3d(-5.5, 1, -2.5);
		glVertex3d(-5.5, 1, 2.5);
		glVertex3d(-4.5, 2, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(1, 0, 0);
		glVertex3d(-5.5, 1, -2.5);
		glVertex3d(-5.5, 0, -2.5);
		glVertex3d(-5.5, 0, 2.5);
		glVertex3d(-5.5, 1, 2.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glNormal3d(0, -1, 0);
		glVertex3d(-6.5, 0, 2.5);
		glVertex3d(-5.5, 0, 2.5);
		glVertex3d(-5.5, 0, -2.5);
		glVertex3d(-6.5, 0, -2.5);
	}
	glEnd();
	glDisable(GL_LIGHTING);
}

void CGLRenderer::DrawWheel(float tX, float tY, float tZ, float rotAngle)
{
	float h = 1;
	float r = 1.5;
	float nSeg = 16;
	float alpha = 2 * PI / nSeg;
	float x, z;
	
	glColor3f(.82, .82, .7);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glPushMatrix();
	glTranslatef(tX, tY, tZ);
	glRotatef(rotAngle, 1, 0, 0);

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++) {
		x = r * cos(alpha * i);
		z = r * sin(alpha * i);
		glTexCoord2f((x + 6) * SQUARE, (z + 1.5) * SQUARE);
		glVertex3f(x, 0, z);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++) {
		x = r * cos(alpha * i);
		z = r * sin(alpha * i);
		glTexCoord2f((x + 6) * SQUARE, (z + 1.5) * SQUARE);
		glVertex3f(x, h, z);
	}
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSeg + 1; i++) {
		x = r * cos(alpha * i);
		z = r * sin(alpha * i);
		glVertex3f(x, 0, z);

		x = r * cos(alpha * i);
		z = r * sin(alpha * i);
		glVertex3f(x, h, z);
	}
	glEnd();

	glPopMatrix();
}

void CGLRenderer::DrawTrunk()
{
	glEnable(GL_LIGHTING);
	double r = 3;
	double x, y, z;
	double nx, ny, nz;
	double alpha = PI / 2;
	double beta = 0;
	double alphaStep, betaStep;

	x = y = z = 0;
	nx = ny = nz = 0;
	alphaStep = betaStep = PI / 15;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUAD_STRIP);
	while (alpha > -PI / 2)
	{
		while (beta < 360)
		{
			x = r * cos(alpha) * cos(beta);
			y = r * sin(alpha);
			z = r * cos(alpha) * sin(beta);
			nx = x / r;
			ny = y / r;
			nz = z / r;

			double tx1 = atan2(nx, nz) / (2 * PI) + 0.5;
			double ty1 = asin(ny) / PI + 0.5;

			glNormal3d(nx, ny, nz);
			glTexCoord2f(tx1, (16 - ty1) * 0.5);
			glVertex3d(x, y, z);

			alpha -= alphaStep;
			x = r * cos(alpha) * cos(beta);
			y = r * sin(alpha);
			z = r * cos(alpha) * sin(beta);
			nx = x / r;
			ny = y / r;
			nz = z / r;

			double tx = atan2(nx, nz) / (2. * PI) + 0.5;
			double ty = asin(ny) / PI + .5;

			if (tx < 0.75 && tx1 > 0.75)
				tx += 1.0;
			else if (tx > 0.75 && tx1 < 0.75)
				tx -= 1.0;

			glNormal3d(nx, ny, nz);
			glTexCoord2f(tx, (16 - ty) * 0.5);
			glVertex3d(x, y, z);

			alpha += alphaStep;
			beta += betaStep;
		}
		alpha -= alphaStep;
		beta = 0;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_LIGHTING);
}

void CGLRenderer::DrawTruck()
{
	//Strane
	glPushMatrix();
	{
		glTranslated(-6.5, 0, -2.5);
		DrawTruckSide();

		glTranslated(0, 0, 5);
		DrawTruckSide();
	}
	glPopMatrix();

	//Omotac
	DrawTruckCover();

	//Prednji
	DrawWheel(-4, 0, 1, 90);
	DrawWheel(-4, 0, -2, 90);

	//Zadnji
	DrawWheel(1, 0, 1, 90);
	DrawWheel(1, 0, -2, 90);

	//Elipsoid
	glPushMatrix();
	{
		glTranslatef(2, 5, 0);
		glScalef(1.5, 0.75, 1);
		glRotatef(90, 0, 1, 0);
		DrawTrunk();
	}
	glPopMatrix();
}

void CGLRenderer::ViewPointLight()
{

	float mat[] = { 0.5, 0.5, 0.5, 1.0 };

	GLfloat light_position[] = { 13, 39, 13, 0 };


	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT1, GL_AMBIENT, mat);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, mat);
	glLightfv(GL_LIGHT1, GL_SPECULAR, mat);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180);
	GLfloat spot_direction[] = { 0, -1, 0 };
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);

	//Aktiviranje
	glEnable(GL_LIGHT1);

}