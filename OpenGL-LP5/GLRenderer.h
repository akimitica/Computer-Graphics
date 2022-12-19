#pragma once
#include "CGLMaterial.h"

struct Vertex
{
	float x, y, z;
};

class CGLRenderer
{
public:
	void CalculatePosition();
	float get_x(float radius, float angle);
	float get_y(float radius, float angle);
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void DrawAxis();
	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawHalfSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg, bool hollow);
	void DrawCone(double h, double r, int nSeg);
	void DrawNormals();
	void DrawBox(double radius, double height, int sides = 4);
	void DrawStand();
	void DrawFigure();
	void Zoom(bool out);
	void DrawRoom();
	void DrawVase();
	void DrawRing(float radius1, float radius2, float height);
	void IncRing(float step);
	void DecRing(float step);
	Vertex CalculateNormal(Vertex A, Vertex B, Vertex C);
	Vertex SubstractVectors(Vertex A, Vertex B);
	Vertex CrossProductVectors(Vertex A, Vertex B);
	Vertex Normalise(Vertex A);
	float Scalar(Vertex A);

	void AmbientLight();
	void ViewPointLight();

	void Light0();
	void Light2();
	void Light1();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

public:
	double lookingAt[3];
	double viewPosition[3];
	double upVector[3];
	double viewAngle[2];
	double viewR;
	double lookAtHeight;
	bool light1, light2, light0;
	bool showNorm;
	bool showAxis = true;
	CGLMaterial blue, turquoise;
	CGLMaterial matBase;
	CGLMaterial roomBase;
	CGLMaterial magentaSphere;
	CGLMaterial blueSphere;
	CGLMaterial redSphere;
	float radius1, radius2;
};
