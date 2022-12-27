#pragma once
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL\gl.h>
#include <GL\glu.h>
#include <math.h>

#define _USE_MATH_DEFINES
#define PI 3.14159
#define TO_RADIAN 0.0174533 

class CGLRenderer
{
private:
	HGLRC m_hrc;
	UINT groundTexture, texture;
public:
	float eyePosition[3];
	float viewAngle[2];
	float eyeDistance;
	float lookingAtY;
	float upVectorY;

public:
	CGLRenderer();
	~CGLRenderer();

	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);

	void SetEyePosition();
	UINT LoadTexture(char* fileName);

	void DrawAxis(double width);
	void DrawGround();
	void DrawTruckSide();
	void DrawTruckCover();
	void DrawWheel(float tX, float tY, float tZ, float rotAngle);
	void DrawTrunk();
	void DrawTruck();
	void ViewPointLight();
};

