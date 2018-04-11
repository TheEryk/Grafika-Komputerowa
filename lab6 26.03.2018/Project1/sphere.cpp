#include "sphere.h"

//--------------------------------------------------------------------------------------------
// domyslny konstruktor 
glSphere::glSphere(float R, char *TextureFile)
{
	tex = NULL;
	tex = new glTexture(TextureFile);

	// przygotuj geometrie obiektu 
	float alpha = 0.0; 
	int slices = 50; 
	float dalpha = 360.0/float(slices-1); 

	BeginObject(GL_TRIANGLE_STRIP, tex->Bind());
			
	SetColor(1.0,1.0,1.0);
	SetNormal(1.0, 0.0, 0.0);

	float u = 0.0; 
	float du = 1.0/float(slices-1);
	
	float v = 0.0; 
	float dv = 1.0/float(slices-1);

	float phi = -90.0;
	float dphi = 180.0/float(slices-1);	

	while (phi <= 90)
	{
		alpha = 0.0;
		u = 0.0;
		while (alpha <= 360.0)
		{
			SetNormal(cos(2 * 3.14*(phi / 360))*cos(2 * 3.14*(alpha / 360)), sin(2 * 3.14*(phi / 360)), cos(2 * 3.14*(phi / 360))*sin(2 * 3.14*(alpha / 360)));
			AddVertex(R*cos(2 * 3.14*(phi / 360))*cos(2 * 3.14*(alpha / 360)), R*sin(2 * 3.14*(phi / 360)), R*cos(2 * 3.14*(phi / 360))*sin(2 * 3.14*(alpha / 360)), alpha / 360, 0.5+phi/180);
			SetNormal(cos(2 * 3.14*((phi + dphi) / 360))*cos(2 * 3.14*(alpha / 360)), sin(2 * 3.14*((phi + dphi) / 360)), cos(2 * 3.14*((phi + dphi) / 360))*sin(2 * 3.14*(alpha / 360)));
			AddVertex(R*cos(2 * 3.14*((phi+dphi) / 360))*cos(2 * 3.14*(alpha / 360)), R*sin(2 * 3.14*((phi + dphi) / 360)), R*cos(2 * 3.14*((phi + dphi) / 360))*sin(2 * 3.14*(alpha / 360)), alpha / 360, 0.5 + (phi+dphi) / 180);
			alpha += dalpha;
			u+=du;
		}
		phi += dphi;
		v += dv;
	}	
	EndObject();
}
//--------------------------------------------------------------------------------------------
// domyslny destruktor 
glSphere::~glSphere()
{	
	if (tex) delete tex;
}
//--------------------------------------------------------------------------------------------
// the end 
