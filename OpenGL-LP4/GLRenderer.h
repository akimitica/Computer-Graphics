#pragma once
#include <gl\GL.h>
#include <gl\GLU.h>

class CGLRenderer
{
public:
	void CalculatePosition();
	CGLRenderer();
	virtual ~CGLRenderer();
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawCone(double h, double r, int nSeg);
	void DrawGrid(double w, double h, int nSegW, int nSegH);
	void DrawAxis();
	void DrawFigure(double angle);
	void DrawBranch(double angle = 0 , bool isCone = false);


protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
public:
	float angle_cx = 0;
	double lookingAt[3];
	double viewPosition[3];
	double upVector[3];
	double viewAngle[2];
	double viewR;
	double lookAtHeight = 2;
	bool showAxis = true;
	bool showGrid = true;
};
