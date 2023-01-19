UINT CGLRenderer::Loadtexture(char* fileName)
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

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texId;
}



void CGLRenderer::CalculatePosition()
{
	viewPosition[0] = viewR * cos(viewAngle[0]) * cos(viewAngle[1]);
	viewPosition[1] = viewR * sin(viewAngle[0]);
	viewPosition[2] = viewR * cos(viewAngle[0]) * sin(viewAngle[1]);


	lookingAt[0] = 0;
	lookingAt[1] = lookAtHeight;
	lookingAt[2] = 0;
}

gluLookAt(viewPosition[0], viewPosition[1], viewPosition[2],
		lookingAt[0], lookingAt[1], lookingAt[2],
		upVector[0], upVector[1], upVector[2]);



void CGLRenderer::AmbientLight()
{

	GLfloat lmodel_ambient[] = { 0.9, 0.9, 0.9, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

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