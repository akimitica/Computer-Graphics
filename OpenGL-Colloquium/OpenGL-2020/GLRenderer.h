#pragma once
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <gl\gl.h>
#include <gl\GLU.h>
#include <math.h>

#define _USE_MATH_DEFINES
#define PI 3.1415926535897932384626433832795
#define TO_RADIAN 0.0174533

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawAxes();

	UINT LoadTexture(char* filename);

	void DrawEnvCube(double a);

	void SetEyePosition();

	void DrawCone(int n, double r, double h);

	void DrawSphere(double r, int segA, int segB);

	void Zoom(bool out);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	UINT spider, env[6];
public:
	float eyePosition[3];
	float viewAngle[2];
	float eyeDistance;
	float lookingAtY;
	float upVectorY;

};
