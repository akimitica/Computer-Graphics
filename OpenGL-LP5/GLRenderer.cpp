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

float CGLRenderer::get_x(float radius, float angle)
{
	return radius * cosf(angle);
}

float CGLRenderer::get_y(float radius, float angle)
{
	return radius * sinf(angle);
}

CGLRenderer::CGLRenderer()
{
	m_hrc = NULL;
	lookAtHeight = 10;
	viewR = 30;
	viewAngle[0] = 0;// PI * 30 / 180;
	viewAngle[1] = 0;// PI * 45 / 180;

	CalculatePosition();

	upVector[0] = 0;
	upVector[1] = 1;
	upVector[2] = 0;

	light0 = true;
	light1 = true;
	light2 = true;
	showNorm = true;

	blue.SetAmbient(0, 0, 0.2, 0.0);
	blue.SetDiffuse(0, 0, 1, 0.0);
	blue.SetSpecular(0.5, 0.5, 0.5, 0.0);
	blue.SetEmission(0, 0, 0.25, 0);
	blue.SetShininess(5.0);

	turquoise.SetAmbient(0, 0.2, 0.2, 0.0);
	turquoise.SetDiffuse(0, 1, 1, 0.0);
	turquoise.SetSpecular(0.5, 0.5, 0.5, 0.0);
	turquoise.SetEmission(0, 0.25, 0.25, 0);
	turquoise.SetShininess(5.0);

	matBase.SetAmbient(0.15, 0.15, 0.15, 0.0);
	matBase.SetDiffuse(0.7, 0.7, 0.7, 0.0);
	matBase.SetSpecular(0.5, 0.9, 0.5, 0.0);
	matBase.SetShininess(64.0);

	roomBase.SetAmbient(0.1, 0.1, 0.1, 1.0);
	roomBase.SetDiffuse(0.8, 0.8, 0.8, 1.0);
	roomBase.SetSpecular(0.2, 0.2, 0.2, 1.0);
	roomBase.SetEmission(0.1, 0.1, 0.1, 1.0);
	roomBase.SetShininess(0.0);


	magentaSphere.SetAmbient(0.8, 0.0, 0.0, 1);
	magentaSphere.SetDiffuse(0.8, 0.0, 0.0, 1);
	magentaSphere.SetSpecular(1, 0, 0, 1);
	magentaSphere.SetEmission(1, 0, 0, 1);
	magentaSphere.SetShininess(0);

	blueSphere.SetAmbient(0.2, 0.2, 0.9, 1);
	blueSphere.SetDiffuse(0.2, 0.2, 0.8, 1);
	blueSphere.SetSpecular(0, 1, 1, 1);
	blueSphere.SetEmission(0, 1, 1, 1);
	blueSphere.SetShininess(0);

	redSphere.SetAmbient(0.8, 0.0, 0.0, 1);
	redSphere.SetDiffuse(0.8, 0.0, 0.0, 1);
	redSphere.SetSpecular(1, 0, 0, 1);
	redSphere.SetEmission(1, 0, 0, 1);
	redSphere.SetShininess(0);
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
	glClearColor(0.07, 0.08, 0.09, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	AmbientLight();
	glEnable(GL_LIGHTING);
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

	gluLookAt(viewPosition[0], viewPosition[1], viewPosition[2],
		lookingAt[0], lookingAt[1], lookingAt[2],
		upVector[0], upVector[1], upVector[2]);

	AmbientLight();
	ViewPointLight();



	glDisable(GL_LIGHTING);

	DrawAxis();

	glEnable(GL_LIGHTING);

	Light0();
	Light2();
	Light1();

	glPushMatrix();

	roomBase.Select();
	DrawRoom();
	glScalef(1.5, 1.5, 1.5);
	matBase.Select();
	DrawStand();

	DrawVase();

	glPopMatrix();

	glFlush();
	//DrawAxis();
	//DrawFigure();
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
	gluPerspective(50, (double)w / (double)h, 15, 100);
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

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	double stepA = 2 * PI / nSegAlpha;
	double stepB = 2 * PI / nSegAlpha;

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSegAlpha; i++) {
		for (int j = 0; j < nSegBeta; j++) {
			glVertex3f(r * cos(stepA * i) * cos(stepB * j), r * sin(stepA * i), r * cos(stepA * i) * sin(stepB * j));
			glVertex3f(r * cos(stepA * (i + 1)) * cos(stepB * j), r * sin(stepA * (i + 1)), r * cos(stepA * (i + 1)) * sin(stepB * j));
		}
	}
	glEnd();
}

void CGLRenderer::DrawHalfSphere(double r, int nSegAlpha, int nSegBeta)
{
	double stepA = 2 * PI / nSegAlpha;
	double stepB = 2 * PI / nSegAlpha;

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSegAlpha/2; i++) {
		for (int j = 0; j < nSegBeta; j++) {
			glVertex3f(r * cos(stepA * i) * cos(stepB * j), r * sin(stepA * i), r * cos(stepA * i) * sin(stepB * j));
			glVertex3f(r * cos(stepA * (i + 1)) * cos(stepB * j), r * sin(stepA * (i + 1)), r * cos(stepA * (i + 1)) * sin(stepB * j));
		}
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg, bool hollow)
{
	float step = 2 * PI / nSeg;
	if (!hollow)
	{
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
	}

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

}

void CGLRenderer::DrawNormals()
{

}

void CGLRenderer::DrawBox(double radius, double height, int sides)
{
	float t_height = height / 2;

	glPushMatrix();
	glRotatef(45, 0, 1, 0);
	float step = (2 * PI) / sides;
	//Baza 1
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < sides; i++) {
		glNormal3f(0, 1, 0);
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));
	}
	glEnd();

	//Baza 2
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < sides; i++) {
		glNormal3f(0, -1, 0);
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
	}
	glEnd();

	//Omotac

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= sides; i++) {
		glNormal3f(get_x(1, i * step), 0, get_y(1, i * step));
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));

		glNormal3f(get_x(1, i * step), 0, get_y(1, i * step));
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
	}
	glEnd();

	glPopMatrix();
}

void CGLRenderer::DrawStand()
{
	glColor3f(0.6, 0.6, 0.6);
	DrawHalfSphere(2, 16, 16);
	glTranslatef(0, 2 * cos(sqrt(1 - 1 / 2)), 0);
	DrawCylinder(2, 1, 1, 8, false);
	glTranslatef(0, 2, 0);
	DrawBox(4, 1);
	glTranslatef(0, 0.25, 0);
}

void CGLRenderer::Zoom(bool out)
{
	viewR += out ? 1 : -1;

	double dwXY = PI * viewAngle[0] / 180;
	double dwXZ = PI * viewAngle[1] / 180;

	viewPosition[0] = viewR * cos(dwXY) * cos(dwXZ);
	viewPosition[1] = viewR * sin(dwXY);
	viewPosition[2] = viewR * cos(dwXY) * sin(dwXZ);
}

void CGLRenderer::DrawRoom()
{
	float a = 40;

	glBegin(GL_QUADS);

	float step = 1;
	//bottom
	//if (angle_h < 35)
	{
		glNormal3f(0, 1, 0);
		for (float i = -a / 2; i < a / 2; i += step)
		{
			for (float j = -a / 2; j < a / 2; j += step)
			{
				glVertex3f(i, 0, j);
				glVertex3f(i, 0, j + step);
				glVertex3f(i + step, 0, j + step);
				glVertex3f(i + step, 0, j);
			}
		}
		glEnd();
	}

	//front
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(a / 2, i, j);
			glVertex3f(a / 2, i, j + step);
			glVertex3f(a / 2, i + step, j + step);
			glVertex3f(a / 2, i + step, j);
		}
	}
	glEnd();


	//back

	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(-a / 2, i, j);
			glVertex3f(-a / 2, i, j + step);
			glVertex3f(-a / 2, i + step, j + step);
			glVertex3f(-a / 2, i + step, j);
		}
	}
	glEnd();



	glBegin(GL_QUADS);
	//left
	glNormal3f(0, 0, 1);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(j, i, -a / 2);
			glVertex3f(j + step, i, -a / 2);
			glVertex3f(j + step, i + step, -a / 2);
			glVertex3f(j, i + step, -a / 2);
		}
	}
	glEnd();



	//right
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(j, i, a / 2);
			glVertex3f(j + step, i, a / 2);
			glVertex3f(j + step, i + step, a / 2);
			glVertex3f(j, i + step, a / 2);
		}
	}
	glEnd();

}

void CGLRenderer::DrawVase()
{
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	float step = 1;
	radius1 = 4;
	radius2 = 5;
	float height = 1;
	float pom;
	bool color = false;


	int niz[14] = { -3, -3, -2, 0, 1, 3, -1, -1, -1, -1, -1, -3, -1, 3 };
	for (int i = 0; i < 14; i++)
	{
		switch (niz[i])
		{
		case 3:
			IncRing(step);
			break;
		case -3:
			DecRing(step);
			break;
		case 2:
			radius2 += step;
			break;
		case -2:
			radius2 -= step;
			break;
		case 1:
			radius1 += step;
			break;
		case -1:
			pom = radius1;
			radius1 = radius2;
			radius2 = pom;
			break;
		default:

			break;
		}
		if (color)
			turquoise.Select();
		else
			blue.Select();
		color = !color;
		glTranslatef(0, step, 0);
		DrawRing(radius1, radius2, height);
	}


	glPopMatrix();

}

void CGLRenderer::DrawRing(float radius1, float radius2, float height)
{
	float step = PI / 180 * 2;
	float half = height / 2.0;
	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 180; i++)
	{
		float x1 = get_x(radius1, i * step);
		float y1 = half;
		float z1 = get_y(radius1, i * step);


		float x2 = get_x(radius1, (i + 1) * step);
		float y2 = -half;
		float z2 = get_y(radius1, (i + 1) * step);

		float x3 = get_x(radius2, i * step);
		float z3 = get_y(radius2, i * step);

		float x4 = get_x(radius2, (i + 1) * step);
		float z4 = get_y(radius2, (i + 1) * step);

		Vertex A = { radius1 * cos(i * step), half, radius1 * sin(i * step) };
		Vertex B = { radius2 * cos(i * step), -half, radius2 * sin(i * step) };
		Vertex vecraz = SubstractVectors(A, B);
		float hip = Scalar(vecraz);
		float stranica = radius2 - radius1;
		float ugao = acos(stranica / hip);


		glNormal3f(cos(i * step), cos(ugao), sin(i * step));
		glVertex3f(x1, y1, z1);
		glNormal3f(cos(i * step), cos(ugao), sin(i * step));
		glVertex3f(x2, y1, z2);

		glNormal3f(cos(i * step), cos(ugao), sin(i * step));
		glVertex3f(x3, y2, z3);
		glNormal3f(cos(i * step), cos(ugao), sin(i * step));
		glVertex3f(x4, y2, z4);

	}
	glEnd();


	if (showNorm)
	{
		glDisable(GL_LIGHTING);

		step = PI / 180 * 10;
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);
		for (int i = 0; i <= 36; i++) {

			Vertex A = { radius1 * cos(i * step), half, radius1 * sin(i * step) };
			Vertex B = { radius2 * cos(i * step), -half, radius2 * sin(i * step) };
			Vertex vecraz = SubstractVectors(A, B);
			float hip = Scalar(vecraz);
			float stranica = radius2 - radius1;
			float ugao = acos(stranica / hip);

			glVertex3f(radius1 * cos(i * step), half, radius1 * sin(i * step));
			glVertex3f(radius1 * cos(i * step) + cos(i * step), half + cos(ugao), radius1 * sin(i * step) + sin(i * step));

			glVertex3f(radius2 * cos(i * step), -half, radius2 * sin(i * step));
			glVertex3f(radius2 * cos(i * step) + cos(i * step), -half + cos(ugao), radius2 * sin(i * step) + sin(i * step));
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
}

void CGLRenderer::IncRing(float step)
{
	radius1 += step;
	radius2 += step;
}

void CGLRenderer::DecRing(float step)
{
	radius1 -= step;
	radius2 -= step;
}

Vertex CGLRenderer::SubstractVectors(Vertex A, Vertex B)
{
	return Vertex() = { A.x - B.x, A.y - B.y, A.z - B.z };
}

Vertex CGLRenderer::CrossProductVectors(Vertex A, Vertex B)
{
	return Vertex() = { A.y * B.z - A.z * B.y,
						A.z * B.x - A.x * B.z,
						A.x * B.y - A.y * B.x };
}

float CGLRenderer::Scalar(Vertex A)
{
	return sqrtf(A.x * A.x + A.y * A.y + A.z * A.z);
}

Vertex CGLRenderer::Normalise(Vertex A)
{
	float sum = A.x + A.y + A.z;
	return Vertex() = { A.x / sum , A.y / sum ,A.z / sum };
}

Vertex CGLRenderer::CalculateNormal(Vertex A, Vertex B, Vertex C)
{
	Vertex X = SubstractVectors(B, A);
	Vertex Y = SubstractVectors(C, A);
	Vertex N = CrossProductVectors(X, Y);
	return Normalise(N);
}

void CGLRenderer::AmbientLight()
{

	GLfloat lmodel_ambient[] = { 0.9, 0.9, 0.9, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

}

void CGLRenderer::ViewPointLight()
{

	float mat[] = { 0.5, 0.5,0.5, 1.0 };

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

void CGLRenderer::Light0()
{

	float light_ambient[] = { 0.5, 0.0, 0.0, 1.0 };
	float light_diffuse[] = { 0.75,0.0, 0.0, 1.0 };
	float light_specular[] = { 1.0, 0, 0, 1.0 };
	GLfloat light_position[] = { 20, 15, 0,1 };


	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT2, GL_POSITION, light_position);

	GLfloat spot_direction[] = { -1, 0, 0 };
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
	//Slabljenje
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);

	//Usmerenje izvora
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20);
	//glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 7.0);

	if (light0) {
		glPushMatrix();
		redSphere.Select();
		glTranslatef(18, 15, 0);
		DrawSphere(1, 32, 32);
		glPopMatrix();
		glEnable(GL_LIGHT2);
	}
	else
		glDisable(GL_LIGHT2);
}

void CGLRenderer::Light1()
{
	float light_ambient[] = { 0.0, 0.75,0.75, 1.0 };
	float light_diffuse[] = { 0.0, 0.75,0.75, 1.0 };
	float light_specular[] = { 0, 1.0, 1.0, 1.0 };

	GLfloat light_position[] = { 0, 40, 0, 1 };

	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT4, GL_POSITION, light_position);

	GLfloat spot_direction[] = { 0, -1, 0.0 };

	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
	//Slabljenje
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1);

	//Usmerenje izvora
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 20);
	//glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 7.0);

	if (light2) {
		glPushMatrix();
		blueSphere.Select();
		glTranslatef(0, 38, 0);
		DrawSphere(1.5, 32, 32);
		glPopMatrix();
		glEnable(GL_LIGHT4);
	}
	else
		glDisable(GL_LIGHT4);
}

void CGLRenderer::Light2()
{
	float light_ambient[] = { 0.5, 0.0, 0.0, 1.0 };
	float light_diffuse[] = { 0.75,0.0, 0.0, 1.0 };
	float light_specular[] = { 1.0, 0, 0, 1.0 };
	GLfloat light_position[] = { -20, 15, 0, 1 };

	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT3, GL_POSITION, light_position);
	GLfloat spot_direction[] = { 1, 0, 0.0 };
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	//Slabljenje
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0);

	//Usmerenje izvora
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 20);
	//glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0);



	if (light1) {
		glPushMatrix();
		magentaSphere.Select();
		glTranslatef(-18, 15, 0);
		DrawSphere(1.5, 32, 32);
		glPopMatrix();
		glEnable(GL_LIGHT3);
	}
	else
		glDisable(GL_LIGHT3);
}